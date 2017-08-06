EESchema Schematic File Version 2
LIBS:SNES_controller_debugger_1-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:switches
LIBS:74xgxx
LIBS:ac-dc
LIBS:actel
LIBS:allegro
LIBS:Altera
LIBS:analog_devices
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos_ieee
LIBS:dc-dc
LIBS:diode
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:ir
LIBS:Lattice
LIBS:leds
LIBS:logo
LIBS:maxim
LIBS:mechanical
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:modules
LIBS:motor_drivers
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:Oscillators
LIBS:powerint
LIBS:Power_Management
LIBS:pspice
LIBS:references
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:transf
LIBS:triac_thyristor
LIBS:ttl_ieee
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:zetex
LIBS:Zilog
LIBS:SNES_controller_debugger_1-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATmega32U4-RESCUE-SNES_controller_debugger_1 U1
U 1 1 5919C3A0
P 2750 2950
F 0 "U1" H 1850 4650 50  0000 C CNN
F 1 "ATmega32U4" H 2100 1400 50  0000 C CNN
F 2 "Housings_QFP:TQFP-44_10x10mm_Pitch0.8mm" V 2500 2550 50  0000 C CNN
F 3 "" H 3950 4050 50  0000 C CNN
	1    2750 2950
	1    0    0    1   
$EndComp
NoConn ~ 6800 2550
NoConn ~ 6700 2550
NoConn ~ 6600 2550
NoConn ~ 6500 2550
$Comp
L POT RV1
U 1 1 591B92F6
P 7200 3200
F 0 "RV1" V 7025 3200 50  0000 C CNN
F 1 "POT" V 7100 3200 50  0000 C CNN
F 2 "Megaris_fixed_footprints:Potentiometer_Trimmer_Vishay_TS53YL" H 7200 3200 50  0001 C CNN
F 3 "" H 7200 3200 50  0001 C CNN
	1    7200 3200
	0    1    -1   0   
$EndComp
Text GLabel 3950 4100 2    50   BiDi ~ 0
LCD_D4
Text GLabel 3950 2600 2    50   BiDi ~ 0
LCD_D5
Text GLabel 6400 2550 3    50   BiDi ~ 0
LCD_D4
Text GLabel 6300 2550 3    50   BiDi ~ 0
LCD_D5
Text GLabel 6200 2550 3    50   BiDi ~ 0
LCD_D6
Text GLabel 6100 2550 3    50   BiDi ~ 0
LCD_D7
Text GLabel 3950 2700 2    50   BiDi ~ 0
LCD_D6
Text GLabel 3950 2900 2    50   BiDi ~ 0
LCD_D7
Text GLabel 3950 1800 2    50   Output ~ 0
LCD_RS
Text GLabel 3950 1700 2    50   Output ~ 0
LCD_RW
Text GLabel 3950 1600 2    50   Output ~ 0
LCD_E
Text GLabel 6900 2550 3    50   Input ~ 0
LCD_E
Text GLabel 7000 2550 3    50   Input ~ 0
LCD_RW
Text GLabel 7100 2550 3    50   Input ~ 0
LCD_RS
Text GLabel 3950 3600 2    50   Output ~ 0
CLK_OUT
Text GLabel 3950 4000 2    50   Output ~ 0
LATCH_OUT
$Comp
L CONN_02X03 J1
U 1 1 591BC56F
P 6200 4800
F 0 "J1" H 6200 5000 50  0000 C CNN
F 1 "ICSP" H 6200 4600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 6200 3600 50  0001 C CNN
F 3 "" H 6200 3600 50  0001 C CNN
	1    6200 4800
	1    0    0    -1  
$EndComp
Text GLabel 650  3400 3    50   UnSpc ~ 0
RESET
Text GLabel 5950 4900 0    50   UnSpc ~ 0
RESET
Text GLabel 3950 4200 2    50   BiDi ~ 0
MISO
Text GLabel 3950 4300 2    50   BiDi ~ 0
MOSI
Text GLabel 3950 4400 2    50   Output ~ 0
SCLK
Text GLabel 5950 4800 0    50   Input ~ 0
SCLK
Text GLabel 5950 4700 0    50   BiDi ~ 0
MISO
Text GLabel 6450 4800 2    50   BiDi ~ 0
MOSI
$Comp
L Crystal Y1
U 1 1 591BE397
P 1250 2550
F 0 "Y1" H 1250 2700 50  0000 C CNN
F 1 "16MHz" H 1250 2400 50  0000 C CNN
F 2 "Crystals:Crystal_SMD_5032-2pin_5.0x3.2mm_HandSoldering" H 1250 2550 50  0001 C CNN
F 3 "" H 1250 2550 50  0001 C CNN
	1    1250 2550
	0    1    1    0   
$EndComp
$Comp
L CONN_01X07 J2
U 1 1 591C5A5F
P 6200 5650
F 0 "J2" H 6200 6050 50  0000 C CNN
F 1 "CONTROLLER_OUT" V 6300 5650 50  0000 C CNN
F 2 "SNES_Controller_Port:SNES_controller_port" H 6200 5650 50  0001 C CNN
F 3 "" H 6200 5650 50  0001 C CNN
	1    6200 5650
	1    0    0    -1  
$EndComp
NoConn ~ 6000 5750
NoConn ~ 6000 5850
Text GLabel 6000 5450 0    50   Input ~ 0
CLK_OUT
Text GLabel 6000 5550 0    50   Input ~ 0
LATCH_OUT
Text GLabel 6000 5650 0    50   Output ~ 0
DATA_IN
$Comp
L C_Small C2
U 1 1 591C64AD
P 950 2400
F 0 "C2" H 960 2470 50  0000 L CNN
F 1 "22pF" H 960 2320 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 950 2400 50  0001 C CNN
F 3 "" H 950 2400 50  0001 C CNN
	1    950  2400
	0    1    1    0   
$EndComp
$Comp
L C_Small C3
U 1 1 591C650A
P 950 2700
F 0 "C3" H 960 2770 50  0000 L CNN
F 1 "22pF" H 960 2620 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 950 2700 50  0001 C CNN
F 3 "" H 950 2700 50  0001 C CNN
	1    950  2700
	0    1    1    0   
$EndComp
$Comp
L R_Small R1
U 1 1 591C6B76
P 650 3000
F 0 "R1" H 680 3020 50  0000 L CNN
F 1 "10k" H 680 2960 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 650 3000 50  0001 C CNN
F 3 "" H 650 3000 50  0001 C CNN
	1    650  3000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C1
U 1 1 591C798D
P 850 4200
F 0 "C1" H 860 4270 50  0000 L CNN
F 1 "0.1µF" H 860 4120 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 850 4200 50  0001 C CNN
F 3 "" H 850 4200 50  0001 C CNN
	1    850  4200
	1    0    0    -1  
$EndComp
Text GLabel 1400 3900 0    50   Input ~ 0
UVCC
Text GLabel 8050 4850 2    50   Output ~ 0
UVCC
$Comp
L R_Small R2
U 1 1 591C87EF
P 7750 5050
F 0 "R2" H 7780 5070 50  0000 L CNN
F 1 "22" H 7780 5010 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7750 5050 50  0001 C CNN
F 3 "" H 7750 5050 50  0001 C CNN
	1    7750 5050
	0    -1   -1   0   
$EndComp
$Comp
L R_Small R3
U 1 1 591C8997
P 7750 5300
F 0 "R3" H 7780 5320 50  0000 L CNN
F 1 "22" H 7780 5260 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 7750 5300 50  0001 C CNN
F 3 "" H 7750 5300 50  0001 C CNN
	1    7750 5300
	0    -1   -1   0   
$EndComp
Text GLabel 8050 5300 2    50   BiDi ~ 0
D-
Text GLabel 8050 5050 2    50   BiDi ~ 0
D+
$Comp
L USB_OTG J3
U 1 1 591C956F
P 7200 5050
F 0 "J3" H 7000 5500 50  0000 L CNN
F 1 "MICRO_USB" H 7000 5400 50  0000 L CNN
F 2 "Megaris_fixed_footprints:Conn_USBmicro-B_Amphenol_10118194-0001LF" H 7350 5000 50  0001 C CNN
F 3 "" H 7350 5000 50  0001 C CNN
	1    7200 5050
	1    0    0    -1  
$EndComp
NoConn ~ 7500 5250
Text GLabel 1500 3700 0    50   BiDi ~ 0
D+
Text GLabel 1500 3600 0    50   BiDi ~ 0
D-
$Comp
L C_Small C4
U 1 1 591CC03D
P 1100 3500
F 0 "C4" H 1110 3570 50  0000 L CNN
F 1 "1µF" H 1110 3420 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1100 3500 50  0001 C CNN
F 3 "" H 1100 3500 50  0001 C CNN
	1    1100 3500
	0    1    1    0   
$EndComp
NoConn ~ 3950 1900
NoConn ~ 3950 2000
NoConn ~ 3950 2100
NoConn ~ 3950 2300
NoConn ~ 3950 2800
NoConn ~ 3950 3300
NoConn ~ 3950 3800
NoConn ~ 3950 3900
NoConn ~ 3950 4500
$Comp
L Earth #PWR01
U 1 1 591D4C41
P 7400 2550
F 0 "#PWR01" H 7400 2300 50  0001 C CNN
F 1 "Earth" H 7400 2400 50  0001 C CNN
F 2 "" H 7400 2550 50  0001 C CNN
F 3 "" H 7400 2550 50  0001 C CNN
	1    7400 2550
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR02
U 1 1 591D4E59
P 5900 2550
F 0 "#PWR02" H 5900 2300 50  0001 C CNN
F 1 "Earth" H 5900 2400 50  0001 C CNN
F 2 "" H 5900 2550 50  0001 C CNN
F 3 "" H 5900 2550 50  0001 C CNN
	1    5900 2550
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR03
U 1 1 591D50E7
P 7400 3250
F 0 "#PWR03" H 7400 3000 50  0001 C CNN
F 1 "Earth" H 7400 3100 50  0001 C CNN
F 2 "" H 7400 3250 50  0001 C CNN
F 3 "" H 7400 3250 50  0001 C CNN
	1    7400 3250
	1    0    0    -1  
$EndComp
$Comp
L LCD16X2 DS1
U 1 1 5919C64C
P 6650 2050
F 0 "DS1" H 5850 2450 50  0000 C CNN
F 1 "LCD20X4" H 7350 2450 50  0000 C CNN
F 2 "Megaris_fixed_footprints:LCD_2004_Pitch2.54mm" H 6650 2000 50  0001 C CIN
F 3 "" H 6650 2050 50  0000 C CNN
	1    6650 2050
	-1   0    0    -1  
$EndComp
$Comp
L Earth #PWR04
U 1 1 591D5F38
P 6450 4900
F 0 "#PWR04" H 6450 4650 50  0001 C CNN
F 1 "Earth" H 6450 4750 50  0001 C CNN
F 2 "" H 6450 4900 50  0001 C CNN
F 3 "" H 6450 4900 50  0001 C CNN
	1    6450 4900
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR05
U 1 1 591D6035
P 5900 5950
F 0 "#PWR05" H 5900 5700 50  0001 C CNN
F 1 "Earth" H 5900 5800 50  0001 C CNN
F 2 "" H 5900 5950 50  0001 C CNN
F 3 "" H 5900 5950 50  0001 C CNN
	1    5900 5950
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR06
U 1 1 591D683F
P 7200 5450
F 0 "#PWR06" H 7200 5200 50  0001 C CNN
F 1 "Earth" H 7200 5300 50  0001 C CNN
F 2 "" H 7200 5450 50  0001 C CNN
F 3 "" H 7200 5450 50  0001 C CNN
	1    7200 5450
	1    0    0    -1  
$EndComp
NoConn ~ 3950 3000
NoConn ~ 3950 3100
$Comp
L PWR_FLAG #FLG07
U 1 1 591D9550
P 7400 5450
F 0 "#FLG07" H 7400 5525 50  0001 C CNN
F 1 "PWR_FLAG" V 7500 5550 50  0000 C CNN
F 2 "" H 7400 5450 50  0001 C CNN
F 3 "" H 7400 5450 50  0001 C CNN
	1    7400 5450
	0    1    1    0   
$EndComp
$Comp
L Earth #PWR08
U 1 1 591D9AF5
P 650 2550
F 0 "#PWR08" H 650 2300 50  0001 C CNN
F 1 "Earth" H 650 2400 50  0001 C CNN
F 2 "" H 650 2550 50  0001 C CNN
F 3 "" H 650 2550 50  0001 C CNN
	1    650  2550
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR09
U 1 1 591D9C68
P 850 3700
F 0 "#PWR09" H 850 3450 50  0001 C CNN
F 1 "Earth" H 850 3550 50  0001 C CNN
F 2 "" H 850 3700 50  0001 C CNN
F 3 "" H 850 3700 50  0001 C CNN
	1    850  3700
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR010
U 1 1 591D9CB5
P 1250 2050
F 0 "#PWR010" H 1250 1800 50  0001 C CNN
F 1 "Earth" H 1250 1900 50  0001 C CNN
F 2 "" H 1250 2050 50  0001 C CNN
F 3 "" H 1250 2050 50  0001 C CNN
	1    1250 2050
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR011
U 1 1 591D9D04
P 850 4300
F 0 "#PWR011" H 850 4050 50  0001 C CNN
F 1 "Earth" H 850 4150 50  0001 C CNN
F 2 "" H 850 4300 50  0001 C CNN
F 3 "" H 850 4300 50  0001 C CNN
	1    850  4300
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG012
U 1 1 591DA503
P 7600 4750
F 0 "#FLG012" H 7600 4825 50  0001 C CNN
F 1 "PWR_FLAG" H 7600 4900 50  0000 C CNN
F 2 "" H 7600 4750 50  0001 C CNN
F 3 "" H 7600 4750 50  0001 C CNN
	1    7600 4750
	1    0    0    -1  
$EndComp
Text GLabel 3950 3500 2    50   Input ~ 0
DATA_IN
NoConn ~ 3950 3200
$Comp
L Polyfuse_Small F1
U 1 1 591F832C
P 7750 4850
F 0 "F1" V 7675 4850 50  0000 C CNN
F 1 "Polyfuse" V 7825 4850 50  0001 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 7800 4650 50  0001 L CNN
F 3 "" H 7750 4850 50  0001 C CNN
	1    7750 4850
	0    1    1    0   
$EndComp
$Comp
L SW_Push SW1
U 1 1 591F8680
P 1050 3300
F 0 "SW1" H 1100 3400 50  0000 L CNN
F 1 "Reset_switch" H 1050 3240 50  0001 C CNN
F 2 "Buttons_Switches_THT:SW_PUSH_6mm" H 1050 3500 50  0001 C CNN
F 3 "" H 1050 3500 50  0001 C CNN
	1    1050 3300
	1    0    0    -1  
$EndComp
Text GLabel 7300 2550 3    50   Input ~ 0
VCC
Text GLabel 6000 2550 3    50   Input ~ 0
VCC
Text GLabel 6950 3250 3    50   Output ~ 0
VCC
Text GLabel 750  4500 0    50   Input ~ 0
VCC
$Comp
L MBR0520 D1
U 1 1 591FB650
P 4100 5550
F 0 "D1" H 4100 5650 50  0000 C CNN
F 1 "MBR0520" H 4100 5450 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 4100 5375 50  0001 C CNN
F 3 "" H 4100 5550 50  0001 C CNN
	1    4100 5550
	-1   0    0    1   
$EndComp
Text Notes 3700 5350 0    60   ~ 0
+5V AUTO SELECTOR
Text GLabel 3950 5900 0    50   Input ~ 0
UVCC
$Comp
L MBR0520 D2
U 1 1 591FB9FA
P 4100 5900
F 0 "D2" H 4100 6000 50  0000 C CNN
F 1 "MBR0520" H 4100 5800 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 4100 5725 50  0001 C CNN
F 3 "" H 4100 5900 50  0001 C CNN
	1    4100 5900
	-1   0    0    1   
$EndComp
Text GLabel 6500 4700 2    50   Output ~ 0
IVCC
Text GLabel 3950 5550 0    50   Input ~ 0
IVCC
$Comp
L PWR_FLAG #FLG013
U 1 1 591FC2B2
P 6450 4650
F 0 "#FLG013" H 6450 4725 50  0001 C CNN
F 1 "PWR_FLAG" H 6450 4800 50  0000 C CNN
F 2 "" H 6450 4650 50  0001 C CNN
F 3 "" H 6450 4650 50  0001 C CNN
	1    6450 4650
	1    0    0    -1  
$EndComp
Text GLabel 4900 5750 2    50   Output ~ 0
VCC
Text GLabel 650  2900 1    50   Input ~ 0
VCC
Text GLabel 6000 5350 0    50   Input ~ 0
VCC
$Comp
L PWR_FLAG #FLG014
U 1 1 591FEDEE
P 4550 5700
F 0 "#FLG014" H 4550 5775 50  0001 C CNN
F 1 "PWR_FLAG" H 4550 5850 50  0000 C CNN
F 2 "" H 4550 5700 50  0001 C CNN
F 3 "" H 4550 5700 50  0001 C CNN
	1    4550 5700
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR015
U 1 1 59301178
P 4450 2400
F 0 "#PWR015" H 4450 2150 50  0001 C CNN
F 1 "Earth" H 4450 2250 50  0001 C CNN
F 2 "" H 4450 2400 50  0001 C CNN
F 3 "" H 4450 2400 50  0001 C CNN
	1    4450 2400
	1    0    0    -1  
$EndComp
$Comp
L C_Small C7
U 1 1 59301D5B
P 4750 5900
F 0 "C7" H 4760 5970 50  0000 L CNN
F 1 "1µF" H 4760 5820 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4750 5900 50  0001 C CNN
F 3 "" H 4750 5900 50  0001 C CNN
	1    4750 5900
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR016
U 1 1 593021E9
P 4750 6000
F 0 "#PWR016" H 4750 5750 50  0001 C CNN
F 1 "Earth" H 4750 5850 50  0001 C CNN
F 2 "" H 4750 6000 50  0001 C CNN
F 3 "" H 4750 6000 50  0001 C CNN
	1    4750 6000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C5
U 1 1 5930241B
P 950 4650
F 0 "C5" H 960 4720 50  0000 L CNN
F 1 "1µF" H 960 4570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 950 4650 50  0001 C CNN
F 3 "" H 950 4650 50  0001 C CNN
	1    950  4650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C6
U 1 1 5930259D
P 1200 4650
F 0 "C6" H 1210 4720 50  0000 L CNN
F 1 "0.1µF" H 1210 4570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1200 4650 50  0001 C CNN
F 3 "" H 1200 4650 50  0001 C CNN
	1    1200 4650
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR017
U 1 1 593033E6
P 950 4800
F 0 "#PWR017" H 950 4550 50  0001 C CNN
F 1 "Earth" H 950 4650 50  0001 C CNN
F 2 "" H 950 4800 50  0001 C CNN
F 3 "" H 950 4800 50  0001 C CNN
	1    950  4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4650 6450 4700
Wire Wire Line
	6450 4700 6500 4700
Connection ~ 7600 4850
Wire Wire Line
	7600 4750 7600 4850
Connection ~ 4350 5750
Wire Wire Line
	4350 5750 4900 5750
Wire Wire Line
	4350 5550 4250 5550
Wire Wire Line
	4350 5550 4350 5900
Wire Wire Line
	4350 5900 4250 5900
Wire Wire Line
	6950 3200 6950 3250
Connection ~ 7200 5450
Wire Wire Line
	7850 4850 8050 4850
Wire Wire Line
	7500 4850 7650 4850
Wire Wire Line
	1350 3300 1250 3300
Wire Wire Line
	650  3100 650  3400
Connection ~ 850  3400
Wire Wire Line
	7100 5450 7400 5450
Wire Wire Line
	1250 1800 1500 1800
Wire Wire Line
	1250 1700 1500 1700
Connection ~ 1250 1800
Wire Wire Line
	1250 1900 1500 1900
Connection ~ 1250 1900
Wire Wire Line
	1250 1600 1500 1600
Connection ~ 1250 1700
Wire Wire Line
	7200 3050 7200 2550
Wire Wire Line
	7350 3200 7400 3200
Wire Wire Line
	7400 3200 7400 3250
Wire Wire Line
	7050 3200 6950 3200
Wire Wire Line
	1250 1600 1250 2050
Wire Wire Line
	1050 2400 1500 2400
Wire Wire Line
	1050 2700 1500 2700
Wire Wire Line
	6000 5950 5900 5950
Connection ~ 1250 2400
Connection ~ 1250 2700
Wire Wire Line
	850  2400 750  2400
Wire Wire Line
	750  2400 750  2700
Wire Wire Line
	750  2700 850  2700
Wire Wire Line
	750  2550 650  2550
Connection ~ 750  2550
Wire Wire Line
	650  3100 1500 3100
Wire Wire Line
	1500 4200 1300 4200
Wire Wire Line
	1300 4200 1300 4500
Connection ~ 1300 4500
Wire Wire Line
	1300 4400 1500 4400
Connection ~ 1300 4400
Wire Wire Line
	1500 4300 1300 4300
Connection ~ 1300 4300
Wire Wire Line
	1500 4100 850  4100
Wire Wire Line
	1500 3800 1450 3800
Wire Wire Line
	1450 3800 1450 3900
Wire Wire Line
	1400 3900 1500 3900
Connection ~ 1450 3900
Wire Wire Line
	7500 5050 7650 5050
Wire Wire Line
	7500 5150 7600 5150
Wire Wire Line
	7600 5150 7600 5300
Wire Wire Line
	7600 5300 7650 5300
Wire Wire Line
	7850 5050 8050 5050
Wire Wire Line
	7850 5300 8050 5300
Wire Wire Line
	1500 3400 850  3400
Wire Wire Line
	850  3300 850  3700
Wire Wire Line
	1500 3500 1200 3500
Wire Wire Line
	850  3500 1000 3500
Connection ~ 850  3500
Wire Wire Line
	4550 5700 4550 5750
Connection ~ 4550 5750
Wire Wire Line
	3950 2400 4450 2400
Wire Wire Line
	4750 5800 4750 5750
Connection ~ 4750 5750
Wire Wire Line
	750  4500 1500 4500
Wire Wire Line
	1200 4500 1200 4550
Wire Wire Line
	950  4500 950  4550
Connection ~ 1200 4500
Connection ~ 950  4500
Wire Wire Line
	1200 4800 1200 4750
Wire Wire Line
	950  4800 950  4750
Wire Wire Line
	1200 4800 950  4800
Connection ~ 1350 3100
Connection ~ 650  3100
Wire Wire Line
	1350 3100 1350 3300
$EndSCHEMATC
