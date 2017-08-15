#include <yaal/io/ports.hh>
#include <yaal/devices/lcd_hd44780.hh>
#include <yaal/types/pinset.hh>

using namespace yaal;

enum SNES_button {
    BUTTON_B      = 0,
    BUTTON_Y      = 1,
    BUTTON_SELECT = 2,
    BUTTON_START  = 3,
    BUTTON_UP     = 4,
    BUTTON_DOWN   = 5,
    BUTTON_LEFT   = 6,
    BUTTON_RIGHT  = 7,
    BUTTON_A      = 8,
    BUTTON_X      = 9,
    BUTTON_L      = 10,
    BUTTON_R      = 11,
    BUTTON_NONE1  = 12,
    BUTTON_NONE2  = 13,
    BUTTON_NONE3  = 14,
    BUTTON_NONE4  = 15,
    VALID_BUTTON_MASK = (1 << BUTTON_NONE1) - 1,
};

volatile uint16_t buttons = 0;
volatile uint8_t clock_count = 0;

volatile uint16_t idle_count = 0;
#define IDLE_MAX 600

// Called on the rising edge of the latch pulse.
// Nothing here (yet?).
ISR(TIMER1_COMPA_vect) {
    ;
}

// Called on the falling edge of the latch pulse.
// This is when the controller starts outputting the state of the first button.
ISR(TIMER1_OVF_vect) {
    // Start the timer for the clock in non-inverting fast PWM mode,
    // with no prescaler.
    TCCR3A |= _BV(COM3A1);
    TCCR3B |= _BV(CS30);

    buttons = 0;
}

// Called on the falling edge of the clock.
// This is when we sample the buttons.
ISR(TIMER3_COMPA_vect) {
    // Read the controller.
    buttons |= (!(PINC & _BV(PINC7))) << clock_count;
}

// Called on the rising edge of the clock.
ISR(TIMER3_OVF_vect) {
    if (++clock_count == 16) {
        // Stop the timer and reset everything.
        TCCR3A &= ~(_BV(COM3A1));
        TCCR3B &= ~(_BV(CS30));
        TCNT3 = 0;

        clock_count = 0;

        if (idle_count < IDLE_MAX && !(buttons & VALID_BUTTON_MASK))
            idle_count++;
        else if (buttons & VALID_BUTTON_MASK)
            idle_count = 0;
    }
}

static void setup_latch() {
    // Set OC1A as an output and set its state to low.
    // OC1A is PB5 on atmega2560. In Arduino terms, digital pin 11.
    // OC1A is PB5 on atmega32u4. In Arduino terms, digital pin 9.
    DDRB |= _BV(DDB5);
    PORTB &= ~(_BV(PORTB5));

    // Set fast PWM in inverting mode, using ICR1 as TOP.
    // Keep the timer stopped for now.
    TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(WGM13);
    TCNT1 = 0;

    // Enable the Timer 1 Output Compare A Match and Overflow interrupts.
    // This way we get an interrupt on both the rising and the falling edge of
    // the latch pulse.
    TIMSK1 = _BV(OCIE1A) | _BV(TOIE1);

    /*
     * Input the parameters for 60 Hz with a pulse width of 12 µs, using an
     * appropriate prescaler. Setting the prescaler starts the timer.
     *
     * The formula to find out TOP:
     *     f_PWM = f_CLK_IO/(N * (1 + TOP))
     * <=> (1 + TOP) = f_CLK_IO/(N * f_PWM)
     * <=> TOP = f_CLK_IO/(N * f_PWM) - 1
     *
     * Let f_CLK_IO = 16000000,
     *     f_PWM = 60:
     *
     * TOP = 1600000/(6 * N) - 1
     *     = 800000/(3 * N) - 1
     *
     * TOP can be at most 65535. N is in {1, 8, 64, 256, 1024}.
     *
     * If N = 1:
     * TOP = 800000/3 - 1
     *    ~= 266666
     *     > 65535
     *
     * If N = 8:
     * TOP = 800000/24 - 1
     *    ~= 33333
     *    <= 65535
     *
     * N = 8 is first available option and it has the most precision of all.
     *
     * Now we have to find out what timer value corresponds to a 12 µs pulse
     * width.
     *
     * T_period = (TOP + 1)/(f_CLK_IO/N) s
     *          = 33334/2000000 s
     *          = 33334 * 1000000/2000000 µs
     *          = 33334/2 µs
     *          = 16667 µs
     *
     * timer_value = TOP - (TOP + 1) * (12 µs)/T_period
     *             = 33333 - 33334 * (12 µs)/(33334/2 µs)
     *             = 33333 - 24
     *             = 33309
     *
     */
    ICR1 = 33333;
    OCR1A = 33309;
}

static void start_latch() {
    // Start the timer with a prescaler of 8, as chosen above.
    TCCR1B |= _BV(CS11);
}

static void setup_clock() {
    // Set OC3A as an output and set its state to high.
    // OC3A is:
    //     - PE3 on atmega2560. In Arduino terms, digital pin 5.
    //     - PC6 on atmega32u4. In Arduino terms, digital pin 5.
    DDRC |= _BV(DDC6);
    PORTC |= _BV(PORTC6);

    // Set fast PWM, using ICR3 as TOP.
    // Keep the timer stopped for now.
    TCCR3A = _BV(WGM31);
    TCCR3B = _BV(WGM32) | _BV(WGM33);
    TCNT3 = 0;

    // Enable the Timer 3 Output Compare A Match and Overflow interrupts.
    // This way we get an interrupt on both the rising and the falling edge of
    // the clock pulse.
    TIMSK3 = _BV(OCIE3A) | _BV(TOIE3);

    /*
     * Input the parameters for:
     *     - Period: 12 µs
     *       => Frequency: 83333.33333... Hz
     *     - Pulse width: 6 µs
     *       => duty cycle: 50%
     *
     * The formula to find out TOP:
     *     f_PWM = f_CLK_IO/(N * (1 + TOP))
     * <=> (1 + TOP) = f_CLK_IO/(N * f_PWM)
     * <=> TOP = f_CLK_IO/(N * f_PWM) - 1
     *
     * Let f_CLK_IO = 16000000,
     *     f_PWM = 83333.33333...:
     *
     * TOP = 16000000/(83333.33333... * N) - 1
     *     = 192/N - 1
     *
     * TOP can be at most 65535. N is in {1, 8, 64, 256, 1024}. We can easily
     * just use N = 1:
     * TOP = 191
     *
     * Now we have to find out what timer value corresponds to a 6 µs pulse
     * width.
     *
     * T_period = (TOP + 1)/(f_CLK_IO/N) s
     *          = 192/16000000 s
     *          = 192 * 1000000/16000000 µs
     *          = 192/16 µs
     *          = 12 µs
     *
     * timer_value = (TOP + 1) * (6 µs)/T_period - 1
     *             = 192 * (6 µs)/(12 µs) - 1
     *             = 95
     *
     */
    ICR3 = 191;
    OCR3A = 95;
}

// D4-D7
typedef Pinset<PortB4, PortD7, PortD6, PortD4> FourBitSet;
// RS, RW, E
typedef PortF5 RSPin;
typedef PortF6 RWPin;
typedef PortF7 EPin;
typedef LCDInterface<RSPin, RWPin, EPin, FourBitSet> FourBitInterface;
// Use an LCD screen with 4 lines, 5x8 font
typedef LiquidCrystalHD44780<FourBitInterface, 4, false> LCD;

// Control the supply of power to the LCD.
// Note that the logic for PortF1 is inverted because of using a P-MOSFET.
static void lcd_power(bool power) {
    if (power) {
        PORTF &= ~(_BV(PORTF1));
    } else {
        // Cut power to the LCD.
        PORTF |= _BV(PORTF1);

        // Make sure the LCD is not powered by any of the I/O pins.
        RSPin rs;
        RWPin rw;
        EPin e;
        FourBitSet data;
        rs.mode = OUTPUT;
        rw.mode = OUTPUT;
        e.mode = OUTPUT;
        data.set_output();
        rs = false;
        rw = false;
        e = false;
        data = 0x00;
    }
}

static void lcd_backlight(bool backlight) {
    if (backlight)
        PORTD |= _BV(PORTD0);
    else
        PORTD &= ~(_BV(PORTD0));
}

void main() {
    // Set PortC7 as input for reading the incoming controller data.
    // Enable pull-up resistors so that the readings show no buttons
    // pressed when there is no controller connected.
    DDRC &= ~(_BV(DDC7));
    PORTC |= _BV(PORTC7);

    // Set PortD0 as output for controlling the LCD backlight.
    DDRD |= _BV(DDD0);

    // Set PortF1 as output for controlling power to the LCD.
    DDRF |= _BV(DDF1);

    // Disable interrupts globally while we configure PWM.
    cli();

    // Configure and start the latch pulse generation.
    setup_latch();

    // Configure the clock generation. Note that the clock is not started here.
    setup_clock();

    // Start the latch pulse generation.
    start_latch();

    // Re-enable interrupts globally.
    sei();

    // Turn the LCD screen off and on to re-initialize it if the MCU was reset.
    lcd_power(false);
    _delay_ms(100);
    lcd_power(true);
    _delay_ms(100);

    // Turn the LCD backlight on.
    lcd_backlight(true);

    // Initialize the LCD screen.
    LCD lcd;
    lcd.setup();

    // Display on, cursor off, blink off.
    lcd.display_control(true, false, false);

    // Clear display.
    lcd.clear();

    // Set entry mode: increment, no shift.
    lcd.entry_mode(LCD::INCREMENT, false);

    // Create custom character glyphs for arrows representing the D-pad.
    constexpr uint8_t up_arrow = 0x00;
    constexpr uint8_t down_arrow = 0x01;
    constexpr uint8_t left_arrow = 0x02;
    constexpr uint8_t right_arrow = 0x03;
    const uint8_t arrow_glyphs[4][8] = {
        {
            0b00000,
            0b00100,
            0b01110,
            0b10101,
            0b00100,
            0b00100,
            0b00100,
            0b00000
        },
        {
            0b00000,
            0b00100,
            0b00100,
            0b00100,
            0b10101,
            0b01110,
            0b00100,
            0b00000
        },
        {
            0b00000,
            0b00100,
            0b01000,
            0b11111,
            0b01000,
            0b00100,
            0b00000,
            0b00000
        },
        {
            0b00000,
            0b00100,
            0b00010,
            0b11111,
            0b00010,
            0b00100,
            0b00000,
            0b00000
        }
    };

    // Set the first 4 glyphs in CGRAM to the arrows.
    for (uint8_t glyph = 0; glyph < 4; ++glyph) {
        lcd.set_cgram_addr(glyph * 8);
        for (uint8_t row = 0; row < 8; ++row)
            lcd.put_char(arrow_glyphs[glyph][row]);
    }

    // Now, draw the following (requires a 2004 LCD):
    //     L          R
    //  ^                X
    // < > SELECT START Y A
    //  v                B
    uint16_t buttons_saved;
    bool disp_off = false;
    while (1) {
        buttons_saved = buttons & VALID_BUTTON_MASK;

        // Turn the display driver and backlight off if we're idle
        // TODO: possibly dim the backlight after a smaller amount of time?
        if (idle_count == IDLE_MAX && !disp_off) {
            lcd.display_control(false, false, false);
            lcd_backlight(false);
            disp_off = true;
        }
        else if (idle_count < IDLE_MAX && disp_off) {
            lcd.display_control(true, false, false);
            lcd_backlight(true);
            disp_off = false;
        }

        if (disp_off)
            continue;

        // First row:
        lcd.set_cursor_pos(0, 4);
        if (buttons_saved & (1 << BUTTON_L))
            lcd.put_char('L');
        else
            lcd.put_char(' ');

        lcd.set_cursor_pos(0, 15);
        if (buttons_saved & (1 << BUTTON_R))
            lcd.put_char('R');
        else
            lcd.put_char(' ');

        // Second row:
        lcd.set_cursor_pos(1, 1);
        if (buttons_saved & (1 << BUTTON_UP))
            lcd.put_char(up_arrow);
        else
            lcd.put_char(' ');

        lcd.set_cursor_pos(1, 18);
        if (buttons_saved & (1 << BUTTON_X))
            lcd.put_char('X');
        else
            lcd.put_char(' ');

        // Third row:
        lcd.set_cursor_pos(2, 0);
        if (buttons_saved & (1 << BUTTON_LEFT))
            lcd.put_char(left_arrow);
        else
            lcd.put_char(' ');

        lcd.put_char(' ');
        if (buttons_saved & (1 << BUTTON_RIGHT))
            lcd.put_char(right_arrow);
        else
            lcd.put_char(' ');

        if (buttons_saved & (1 << BUTTON_SELECT))
            lcd.put_string(" SELECT ");
        else
            lcd.put_string("        ");

        if (buttons_saved & (1 << BUTTON_START))
            lcd.put_string("START ");
        else
            lcd.put_string("      ");

        lcd.set_cursor_pos(2, 17);
        if (buttons_saved & (1 << BUTTON_Y))
            lcd.put_char('Y');
        else
            lcd.put_char(' ');

        lcd.put_char(' ');
        if (buttons_saved & (1 << BUTTON_A))
            lcd.put_char('A');
        else
            lcd.put_char(' ');

        // Fourth row:
        lcd.set_cursor_pos(3, 1);
        if (buttons_saved & (1 << BUTTON_DOWN))
            lcd.put_char(down_arrow);
        else
            lcd.put_char(' ');

        lcd.set_cursor_pos(3, 18);
        if (buttons_saved & (1 << BUTTON_B))
            lcd.put_char('B');
        else
            lcd.put_char(' ');
    }
}
