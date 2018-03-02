#include <yaal/io/ports.hh>
#include <yaal/devices/lcd_hd44780.hh>
#include <yaal/types/pinset.hh>

using namespace yaal;

template<typename PIN, bool polarity = true>
class PowerPin {
    PIN pin;

public:
    PowerPin() {
        pin.mode = OUTPUT;
        pin = polarity; // Initially on
    }

    void power(bool on) {
        /*
         * Truth table:
         *
         * polarity |  on | polarity ^ on | pin | power
         * ---------+-----+---------------+-----+------
         *     T    |  T  |       F       |  T  |  ON
         *     T    |  F  |       T       |  F  |  OFF
         *     F    |  T  |       T       |  F  |  ON
         *     F    |  F  |       F       |  T  |  OFF
         *
         */
        pin = !(polarity ^ on);
    }
};

template<bool polarity>
class PowerPin<NullPin, polarity> {
public:
    void power(bool) { }
};

class Backlight {
    uint8_t value;
    PortD0 pin;

    void reset(uint8_t val) {
        pin.mode = OUTPUT;

        // Fast PWM, non-inverting mode on OC0B (PortD0).
        // Use a prescaler of 64 to get a bit shy of 1000 Hz.
        TCCR0A = _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
        TCCR0B = 0;
        TCNT0 = 0;
        value = val;
        OCR0B = val;
        TCCR0B = _BV(CS00) | _BV(CS01);
    }
public:
    Backlight(uint8_t val = 0xff) : value(val) {
        reset(value);
    }

    void set(uint8_t val) {
        if (val == 0) { // Avoid the single spike
            TCCR0A = 0;
            TCCR0B = 0;
            pin = false;
            value = val;
        }
        else if (value == 0)
            reset(val);
        else {
            value = val;
            OCR0B = val;
        }
    }
};

enum common_button {
    BUTTON_SELECT = 2,
    BUTTON_START  = 3,
    BUTTON_UP     = 4,
    BUTTON_DOWN   = 5,
    BUTTON_LEFT   = 6,
    BUTTON_RIGHT  = 7,
};

enum SNES_button {
    SNES_BUTTON_B      = 0,
    SNES_BUTTON_Y      = 1,
    SNES_BUTTON_A      = 8,
    SNES_BUTTON_X      = 9,
    SNES_BUTTON_L      = 10,
    SNES_BUTTON_R      = 11,
    SNES_BUTTON_NONE1  = 12,
    SNES_BUTTON_NONE2  = 13,
    SNES_BUTTON_NONE3  = 14,
    SNES_BUTTON_NONE4  = 15,
    SNES_VALID_BUTTON_MASK = (1 << SNES_BUTTON_NONE1) - 1,
};

enum NES_button {
    NES_BUTTON_A        = 0,
    NES_BUTTON_B        = 1,
    NES_VALID_BUTTON_MASK = (1 << (BUTTON_RIGHT + 1)) - 1,
};

#define MODEL_SWITCH_BUTTONS_1 ((1<<BUTTON_SELECT)|(1<<NES_BUTTON_A))
#define MODEL_SWITCH_BUTTONS_2 \
    (MODEL_SWITCH_BUTTONS_1|(1<<SNES_BUTTON_L)|(1<<SNES_BUTTON_R)| \
        (1<<SNES_BUTTON_X)|(1<<SNES_BUTTON_A))
enum Model : bool {
    MODEL_NES = false,
    MODEL_SNES = true,
};
volatile Model model = MODEL_SNES;
volatile uint16_t valid_button_mask = SNES_VALID_BUTTON_MASK;

volatile uint16_t buttons = 0;

#define CLOCK_LIMIT_SNES 16;
#define CLOCK_LIMIT_NES   8;
volatile uint8_t clock_limit = CLOCK_LIMIT_SNES;
volatile uint8_t clock_count = 0;
volatile bool clock_running = false;

enum Region : bool {
    REGION_NTSC = false,
    REGION_PAL  = true,
};
volatile Region region;

// Dim LCD backlight after 10 seconds, turn it off after 30 seconds.
volatile uint16_t idle_count = 0;
#define IDLE_DIM 600
#define IDLE_MAX 1800

/*
 * Some notes:
 * In a PAL controller, there are two pull-up resistors: R1 and R2.
 * R1 pulls the data clock high. About 1.1 kohm.
 * R2 pulls the data latch high. About 2.45 kohm.
 * There are also two "strips resistors" (for the lack of a better term) through
 * which the data clock and data latch go through to the shift register. These
 * seem to be about 240-280 ohm. Let's call them RX and RY.
 * Let's call the clock and latch inputs of the shift register CHIP_CLK_IN and
 * CHIP_LATCH_IN, respectively.
 *
 *  CHIP_CLK_IN        CHIP_LATCH_IN
 *            |        |
 *            RX       RY
 *            |        |
 * 5V--+--R1--+--CLK   |
 *     |               |
 *     |      +--------+
 *     |      |
 *     +--R2--+--LATCH
 *
 * On the console side we have as follows:
 * - PAL SNES
 *   - clock and latch can *not* be pulled up by the console (diodes prevent
 *     this)
 *   - clock and latch *can* be pulled down by the console
 *   - PAL controllers work fine
 *     - When latch or clock is high on the console side, the controller side
 *       pull-up resistors pull it high on the controller side
 *     - When latch or clock is low on the console side, the console pulls them
 *       down on the controller side as well
 *   - NTSC controllers do not work because they do not pull anything up
 *     - When the latch or clock is high on the console side, both appear as
 *       tri-state/low on the controller side
 *     - When the latch or clock is low on the console side, it is also low on
 *       the controller side
 * - NTSC SNES
 *   - clock and latch can be pulled both up and down by the console
 *   - PAL controllers work fine
 *     - When the latch or clock is high on the console side, both the console
 *       and the controller are pulling it up
 *     - When the latch or clock is low on the console side, the controller can
 *       not pull it up
 *   - NTSC controllers work fine
 *     - The console completely controls whether the latch or clock is high or
 *       low
 */

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
    // Be sure to to enable the pull-up before changing the direction
    // of the pin, as otherwise the clock will glitch and confuse the
    // controller.
    PORTC |= _BV(PORTC6);
    DDRC |= _BV(DDC6);
    TCCR3A |= _BV(COM3A1);
    TCCR3B |= _BV(CS30);
    clock_running = true;

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
    if (++clock_count >= clock_limit) {
        // Stop the timer and reset everything.
        TCCR3A &= ~(_BV(COM3A1));
        TCCR3B &= ~(_BV(CS30));
        TCNT3 = 0;

        // We need to set the pin low before setting the direction
        // to input, for some reason - otherwise subsequent reads
        // from PINC will just report a high logic level even if
        // nothing is driving the pin high!
        PORTC &= ~_BV(PORTC6);
        DDRC &= ~_BV(DDC6);

        clock_running = false;
        clock_count = 0;

        if (idle_count < IDLE_MAX && !(buttons & valid_button_mask))
            idle_count++;
        else if (buttons & valid_button_mask) {
            if (idle_count > 0) {
                idle_count = 0;
            }
        }
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
Backlight backlight;
PowerPin<PortF1, false> power; // The polarity is inverted because of using a P-MOSFET.
typedef LCDInterface<RSPin, RWPin, EPin, FourBitSet> FourBitInterface;
// Use an LCD screen with 4 lines, 5x8 font
typedef LiquidCrystalHD44780<FourBitInterface, 4, false> LCD;

// Control the supply of power to the LCD.
static void lcd_power(bool on) {
    power.power(on);

    if (!on) {
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

static void lcd_backlight(uint8_t val) {
    backlight.set(val);
}

void main() {
    // Disable the USB controller. It generates a *lot* of interrupts after a
    // reset, for some reason.
    USBCON &= ~_BV(USBE);

    // Set PortC7 as input for reading the incoming controller data.
    // Enable pull-up resistors so that the readings show no buttons
    // pressed when there is no controller connected.
    DDRC &= ~(_BV(DDC7));
    PORTC |= _BV(PORTC7);

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

    // Now, draw the following (requires a 2004 LCD).
    //
    // For SNES:
    //    L  PAL  SFC  R
    //  ^                X
    // < > SELECT START Y A
    //  v                B
    //
    // For NES:
    //       PAL  NES
    //  ^
    // < > SELECT START B A
    //  v
    uint16_t old_buttons = 0;
    uint16_t buttons_saved;
    uint16_t idle_count_saved;
    enum {
        BL_ON,
        BL_DIMMED,
        BL_OFF,
    } backlight_state = BL_ON;
    while (1) {
        // Wait until we are in a period where the clock is not running.
        while (clock_running) ;

        cli();
        // Detect whether the controller has a pull-up resistor
        // for the clock line. If it does, it is a PAL controller.
        region = Region(!!(PINC & _BV(PINC6)));

        buttons_saved = buttons;
        idle_count_saved = idle_count;
        sei();
        buttons_saved &= valid_button_mask;

        // Dim the backlight and eventually turn it off if we're idle
        switch (backlight_state) {
        case BL_ON:
            if (idle_count_saved >= IDLE_DIM) {
                lcd_backlight(70);
                backlight_state = BL_DIMMED;
            }
            break;
        case BL_DIMMED:
            if (idle_count_saved == IDLE_MAX) {
                lcd.display_control(false, false, false);
                lcd_backlight(0);
                backlight_state = BL_OFF;
                break;
            }
            // Intentional fall-through
        case BL_OFF:
            if (idle_count_saved < IDLE_DIM) {
                lcd.display_control(true, false, false);
                lcd_backlight(255);
                backlight_state = BL_ON;
            }
            break;
        }

        if (backlight_state == BL_OFF)
            continue;

        if ((buttons_saved == MODEL_SWITCH_BUTTONS_1 ||
                buttons_saved == MODEL_SWITCH_BUTTONS_2) &&
                old_buttons != MODEL_SWITCH_BUTTONS_1 &&
                old_buttons != MODEL_SWITCH_BUTTONS_2) {
            cli();
            model = Model(!model);
            if (model == MODEL_SNES) {
                clock_limit = CLOCK_LIMIT_SNES;
                valid_button_mask = SNES_VALID_BUTTON_MASK;
            } else {
                clock_limit = CLOCK_LIMIT_NES;
                valid_button_mask = NES_VALID_BUTTON_MASK;
            }
            sei();
            lcd.clear();
        }
        old_buttons = buttons_saved;

        // First row:
        if (model == MODEL_SNES) {
            lcd.set_cursor_pos(0, 3);
            if (buttons_saved & (1 << SNES_BUTTON_L))
                lcd.put_char('L');
            else
                lcd.put_char(' ');
        }

        lcd.set_cursor_pos(0, 6);
        if (region == REGION_PAL)
            lcd.put_string("PAL  ");
        else
            lcd.put_string("NTSC ");

        if (model == MODEL_SNES)
            lcd.put_string("SFC");
        else
            lcd.put_string("NES");

        if (model == MODEL_SNES) {
            lcd.set_cursor_pos(0, 16);
            if (buttons_saved & (1 << SNES_BUTTON_R))
                lcd.put_char('R');
            else
                lcd.put_char(' ');
        }

        // Second row:
        lcd.set_cursor_pos(1, 1);
        if (buttons_saved & (1 << BUTTON_UP))
            lcd.put_char(up_arrow);
        else
            lcd.put_char(' ');

        if (model == MODEL_SNES) {
            lcd.set_cursor_pos(1, 18);
            if (buttons_saved & (1 << SNES_BUTTON_X))
                lcd.put_char('X');
            else
                lcd.put_char(' ');
        }

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
        if (model == MODEL_SNES) {
            if (buttons_saved & (1 << SNES_BUTTON_Y))
                lcd.put_char('Y');
            else
                lcd.put_char(' ');

            lcd.put_char(' ');
            if (buttons_saved & (1 << SNES_BUTTON_A))
                lcd.put_char('A');
            else
                lcd.put_char(' ');
        } else {
            if (buttons_saved & (1 << NES_BUTTON_B))
                lcd.put_char('B');
            else
                lcd.put_char(' ');

            lcd.put_char(' ');
            if (buttons_saved & (1 << NES_BUTTON_A))
                lcd.put_char('A');
            else
                lcd.put_char(' ');
        }

        // Fourth row:
        lcd.set_cursor_pos(3, 1);
        if (buttons_saved & (1 << BUTTON_DOWN))
            lcd.put_char(down_arrow);
        else
            lcd.put_char(' ');

        if (model == MODEL_SNES) {
            lcd.set_cursor_pos(3, 18);
            if (buttons_saved & (1 << SNES_BUTTON_B))
                lcd.put_char('B');
            else
                lcd.put_char(' ');
        }
    }
}
