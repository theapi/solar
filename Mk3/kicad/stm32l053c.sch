EESchema Schematic File Version 2
LIBS:stm32l053c-rescue
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
LIBS:stm32
LIBS:adc
LIBS:lora
LIBS:theapi_voltage_regulators
LIBS:stm32l053c-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LoRa Garden Solar"
Date ""
Rev "v1.0.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Earth #PWR01
U 1 1 58021BE1
P 7400 1900
F 0 "#PWR01" H 7400 1650 50  0001 C CNN
F 1 "Earth" H 7400 1750 50  0001 C CNN
F 2 "" H 7400 1900 50  0000 C CNN
F 3 "" H 7400 1900 50  0000 C CNN
	1    7400 1900
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 58021E64
P 7400 1750
F 0 "C1" H 7425 1850 50  0000 L CNN
F 1 "100nF" H 7425 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7438 1600 50  0001 C CNN
F 3 "" H 7400 1750 50  0000 C CNN
	1    7400 1750
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR02
U 1 1 58021F75
P 9325 5925
F 0 "#PWR02" H 9325 5675 50  0001 C CNN
F 1 "GNDA" H 9325 5775 50  0000 C CNN
F 2 "" H 9325 5925 50  0000 C CNN
F 3 "" H 9325 5925 50  0000 C CNN
	1    9325 5925
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR03
U 1 1 58021F89
P 9825 5925
F 0 "#PWR03" H 9825 5675 50  0001 C CNN
F 1 "Earth" H 9825 5775 50  0001 C CNN
F 2 "" H 9825 5925 50  0000 C CNN
F 3 "" H 9825 5925 50  0000 C CNN
	1    9825 5925
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR04
U 1 1 58022154
P 8175 5950
F 0 "#PWR04" H 8175 5700 50  0001 C CNN
F 1 "GNDA" H 8175 5800 50  0000 C CNN
F 2 "" H 8175 5950 50  0000 C CNN
F 3 "" H 8175 5950 50  0000 C CNN
	1    8175 5950
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR05
U 1 1 580221FA
P 8675 5950
F 0 "#PWR05" H 8675 5700 50  0001 C CNN
F 1 "Earth" H 8675 5800 50  0001 C CNN
F 2 "" H 8675 5950 50  0000 C CNN
F 3 "" H 8675 5950 50  0000 C CNN
	1    8675 5950
	1    0    0    -1  
$EndComp
Text GLabel 7625 3850 0    60   Input ~ 0
SWCLK
Text GLabel 7625 3750 0    60   Input ~ 0
SWDIO
Text GLabel 7625 4450 0    60   Input ~ 0
NRST
$Comp
L CONN_01X06 P1
U 1 1 58036840
P 1725 3125
F 0 "P1" H 1725 3475 50  0000 C CNN
F 1 "SWD" V 1825 3125 50  0000 C CNN
F 2 "theapi:Pin_Header_Straight_1x06_Pitch2.54mm" H 1725 3125 50  0001 C CNN
F 3 "" H 1725 3125 50  0000 C CNN
	1    1725 3125
	-1   0    0    -1  
$EndComp
$Comp
L Earth #PWR06
U 1 1 580369A9
P 2425 3075
F 0 "#PWR06" H 2425 2825 50  0001 C CNN
F 1 "Earth" H 2425 2925 50  0001 C CNN
F 2 "" H 2425 3075 50  0000 C CNN
F 3 "" H 2425 3075 50  0000 C CNN
	1    2425 3075
	1    0    0    -1  
$EndComp
Text GLabel 1925 2975 2    60   Input ~ 0
SWCLK
Text GLabel 1925 3175 2    60   Input ~ 0
SWDIO
Text GLabel 1925 3275 2    60   Input ~ 0
NRST
$Comp
L Earth #PWR07
U 1 1 58039244
P 6675 1900
F 0 "#PWR07" H 6675 1650 50  0001 C CNN
F 1 "Earth" H 6675 1750 50  0001 C CNN
F 2 "" H 6675 1900 50  0000 C CNN
F 3 "" H 6675 1900 50  0000 C CNN
	1    6675 1900
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 58039250
P 6675 1750
F 0 "C4" H 6700 1850 50  0000 L CNN
F 1 "10uF" H 6700 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6713 1600 50  0001 C CNN
F 3 "" H 6675 1750 50  0000 C CNN
	1    6675 1750
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5803B990
P 7700 1750
F 0 "C2" H 7725 1850 50  0000 L CNN
F 1 "100nF" H 7725 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7738 1600 50  0001 C CNN
F 3 "" H 7700 1750 50  0000 C CNN
	1    7700 1750
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5803BA4A
P 7050 1750
F 0 "C3" H 7075 1850 50  0000 L CNN
F 1 "100nF" H 7075 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7088 1600 50  0001 C CNN
F 3 "" H 7050 1750 50  0000 C CNN
	1    7050 1750
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR08
U 1 1 5803BDC8
P 7050 1900
F 0 "#PWR08" H 7050 1650 50  0001 C CNN
F 1 "Earth" H 7050 1750 50  0001 C CNN
F 2 "" H 7050 1900 50  0000 C CNN
F 3 "" H 7050 1900 50  0000 C CNN
	1    7050 1900
	1    0    0    -1  
$EndComp
Text Label 7050 1600 1    60   ~ 0
VDDA
Text Label 8325 2150 1    60   ~ 0
VDDA
Text Label 8225 2150 1    60   ~ 0
VLCD
Text Label 8475 2150 1    60   ~ 0
24
Text Label 8925 2150 1    60   ~ 0
VDD_USB
Text Label 8575 2150 1    60   ~ 0
48
$Comp
L STM32L053C8_LQFP48 U2
U 1 1 580631B3
P 8475 4250
F 0 "U2" H 9125 6250 60  0000 C CNN
F 1 "STM32L053C8_LQFP48" H 8575 4250 60  0000 C CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 8275 2350 60  0001 C CNN
F 3 "" H 8275 2350 60  0000 C CNN
	1    8475 4250
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5806575A
P 7275 4800
F 0 "R4" V 7355 4800 50  0000 C CNN
F 1 "10K" V 7275 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7205 4800 50  0001 C CNN
F 3 "" H 7275 4800 50  0000 C CNN
	1    7275 4800
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR09
U 1 1 58065816
P 7275 4950
F 0 "#PWR09" H 7275 4700 50  0001 C CNN
F 1 "Earth" H 7275 4800 50  0001 C CNN
F 2 "" H 7275 4950 50  0000 C CNN
F 3 "" H 7275 4950 50  0000 C CNN
	1    7275 4950
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 580770D7
P 4200 2775
F 0 "R1" V 4280 2775 50  0000 C CNN
F 1 "820" V 4200 2775 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4130 2775 50  0001 C CNN
F 3 "" H 4200 2775 50  0000 C CNN
	1    4200 2775
	0    1    1    0   
$EndComp
$Comp
L Earth #PWR010
U 1 1 58077163
P 3550 2875
F 0 "#PWR010" H 3550 2625 50  0001 C CNN
F 1 "Earth" H 3550 2725 50  0001 C CNN
F 2 "" H 3550 2875 50  0000 C CNN
F 3 "" H 3550 2875 50  0000 C CNN
	1    3550 2875
	-1   0    0    -1  
$EndComp
NoConn ~ 7625 4950
NoConn ~ 7625 5050
$Comp
L LED D1
U 1 1 592DE199
P 3900 2775
F 0 "D1" H 3900 2875 50  0000 C CNN
F 1 "LED" H 3900 2675 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3900 2775 50  0001 C CNN
F 3 "" H 3900 2775 50  0001 C CNN
	1    3900 2775
	1    0    0    -1  
$EndComp
$Comp
L ADS1015 U3
U 1 1 592DD751
P 2900 4950
F 0 "U3" H 2300 5400 50  0000 L CNN
F 1 "ADS1015" H 3150 5400 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-10_3x3mm_Pitch0.5mm" H 2850 4900 50  0001 C CNN
F 3 "" H 2000 5350 50  0001 C CNN
	1    2900 4950
	1    0    0    -1  
$EndComp
Text GLabel 1450 6975 0    60   Input ~ 0
ANT
Text Notes 1075 6025 0    98   ~ 20
Antenna\n
Text Notes 2775 6025 0    98   ~ 20
RFM 95/96/92 Module
Text Notes 1275 7375 0    59   ~ 0
Place on Top layer
$Comp
L Earth #PWR011
U 1 1 592DF55A
P 2900 5450
F 0 "#PWR011" H 2900 5200 50  0001 C CNN
F 1 "Earth" H 2900 5300 50  0001 C CNN
F 2 "" H 2900 5450 50  0000 C CNN
F 3 "" H 2900 5450 50  0000 C CNN
	1    2900 5450
	-1   0    0    -1  
$EndComp
Text Notes 1125 4000 0    98   ~ 20
ADC
Text Notes 1125 2525 0    98   ~ 20
Programming Header
$Comp
L AP2112 U1
U 1 1 592F19AA
P 3750 1475
F 0 "U1" H 3500 1725 50  0000 L CNN
F 1 "AP2112" H 3800 1725 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5_HandSoldering" H 3700 1425 50  0001 C CNN
F 3 "" H 2850 1875 50  0001 C CNN
	1    3750 1475
	1    0    0    -1  
$EndComp
Text Notes 1175 925  0    98   ~ 20
Power
NoConn ~ 4150 1625
$Comp
L Earth #PWR012
U 1 1 592F1C89
P 3750 1925
F 0 "#PWR012" H 3750 1675 50  0001 C CNN
F 1 "Earth" H 3750 1775 50  0001 C CNN
F 2 "" H 3750 1925 50  0000 C CNN
F 3 "" H 3750 1925 50  0000 C CNN
	1    3750 1925
	1    0    0    -1  
$EndComp
Text GLabel 9575 3150 2    60   Input ~ 0
I2C1_SDA
Text GLabel 9575 3050 2    60   Input ~ 0
I2C1_SCL
Text GLabel 4350 4750 2    60   Input ~ 0
I2C1_SCL
Text GLabel 4350 4850 2    60   Input ~ 0
I2C1_SDA
Text GLabel 9575 3950 2    60   Input ~ 0
SPI2_MOSI
Text GLabel 9575 3850 2    60   Input ~ 0
SPI2_MISO
Text GLabel 9575 3750 2    60   Input ~ 0
SPI2_SCK
Text GLabel 3400 6725 0    60   Input ~ 0
SPI2_SCK
Text GLabel 3400 6525 0    60   Input ~ 0
SPI2_MISO
$Comp
L RFM92/95/96/97/98 U5
U 1 1 592DE9B5
P 3900 6825
F 0 "U5" H 3950 6425 60  0000 C CNN
F 1 "RFM92/95" H 3900 7375 60  0000 C CNN
F 2 "kicad:RFM92_95_96_98" H 4100 7025 60  0001 C CNN
F 3 "" H 4100 7025 60  0001 C CNN
F 4 "Low Power Long Range Transceiver Module" H 3900 6825 60  0001 C CNN "DESC"
F 5 "....." H 3900 6825 60  0001 C CNN "MFG_NAME"
F 6 "..." H 3900 6825 60  0001 C CNN "MPN"
	1    3900 6825
	1    0    0    -1  
$EndComp
Text GLabel 3400 6625 0    60   Input ~ 0
SPI2_MOSI
Text GLabel 9575 3650 2    60   Input ~ 0
SPI2_NSS
Text GLabel 3400 6925 0    60   Input ~ 0
RFM_RST
$Comp
L +3.3V #PWR013
U 1 1 592F43A5
P 4450 1175
F 0 "#PWR013" H 4450 1025 50  0001 C CNN
F 1 "+3.3V" H 4450 1315 50  0000 C CNN
F 2 "" H 4450 1175 50  0001 C CNN
F 3 "" H 4450 1175 50  0001 C CNN
	1    4450 1175
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 592F4BC3
P 1925 2875
F 0 "#PWR014" H 1925 2725 50  0001 C CNN
F 1 "+3.3V" H 1925 3015 50  0000 C CNN
F 2 "" H 1925 2875 50  0001 C CNN
F 3 "" H 1925 2875 50  0001 C CNN
	1    1925 2875
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR015
U 1 1 592F4E66
P 8475 1425
F 0 "#PWR015" H 8475 1275 50  0001 C CNN
F 1 "+3.3V" H 8475 1565 50  0000 C CNN
F 2 "" H 8475 1425 50  0001 C CNN
F 3 "" H 8475 1425 50  0001 C CNN
	1    8475 1425
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR016
U 1 1 592F53D7
P 4775 6725
F 0 "#PWR016" H 4775 6575 50  0001 C CNN
F 1 "+3.3V" H 4775 6865 50  0000 C CNN
F 2 "" H 4775 6725 50  0001 C CNN
F 3 "" H 4775 6725 50  0001 C CNN
	1    4775 6725
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 592F54CD
P 6675 1600
F 0 "#PWR017" H 6675 1450 50  0001 C CNN
F 1 "+3.3V" H 6675 1740 50  0000 C CNN
F 2 "" H 6675 1600 50  0001 C CNN
F 3 "" H 6675 1600 50  0001 C CNN
	1    6675 1600
	1    0    0    -1  
$EndComp
Text GLabel 4350 7125 2    60   Input ~ 0
ANT
NoConn ~ 7625 5150
NoConn ~ 7625 5250
NoConn ~ 7625 2750
NoConn ~ 9575 2450
NoConn ~ 7625 2850
NoConn ~ 7625 3550
NoConn ~ 7625 3950
NoConn ~ 9575 2550
NoConn ~ 9575 2850
NoConn ~ 9575 3250
NoConn ~ 9575 3350
NoConn ~ 9575 4550
NoConn ~ 9575 2650
NoConn ~ 4350 6425
NoConn ~ 4350 6525
NoConn ~ 4350 6825
NoConn ~ 4350 6925
NoConn ~ 3400 7025
$Comp
L Earth #PWR018
U 1 1 592F7C8A
P 4775 7025
F 0 "#PWR018" H 4775 6775 50  0001 C CNN
F 1 "Earth" H 4775 6875 50  0001 C CNN
F 2 "" H 4775 7025 50  0000 C CNN
F 3 "" H 4775 7025 50  0000 C CNN
	1    4775 7025
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR019
U 1 1 592F7CF0
P 2750 6425
F 0 "#PWR019" H 2750 6175 50  0001 C CNN
F 1 "Earth" H 2750 6275 50  0001 C CNN
F 2 "" H 2750 6425 50  0000 C CNN
F 3 "" H 2750 6425 50  0000 C CNN
	1    2750 6425
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR020
U 1 1 592F7EE4
P 3225 7125
F 0 "#PWR020" H 3225 6875 50  0001 C CNN
F 1 "Earth" H 3225 6975 50  0001 C CNN
F 2 "" H 3225 7125 50  0000 C CNN
F 3 "" H 3225 7125 50  0000 C CNN
	1    3225 7125
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR021
U 1 1 592F80DC
P 1300 6575
F 0 "#PWR021" H 1300 6325 50  0001 C CNN
F 1 "Earth" H 1300 6425 50  0001 C CNN
F 2 "" H 1300 6575 50  0000 C CNN
F 3 "" H 1300 6575 50  0000 C CNN
	1    1300 6575
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR022
U 1 1 592F814F
P 2050 6600
F 0 "#PWR022" H 2050 6350 50  0001 C CNN
F 1 "Earth" H 2050 6450 50  0001 C CNN
F 2 "" H 2050 6600 50  0000 C CNN
F 3 "" H 2050 6600 50  0000 C CNN
	1    2050 6600
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 59300E59
P 4200 4400
F 0 "R3" V 4280 4400 50  0000 C CNN
F 1 "10K" V 4200 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4130 4400 50  0001 C CNN
F 3 "" H 4200 4400 50  0001 C CNN
	1    4200 4400
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 59300F14
P 4000 4400
F 0 "R2" V 4080 4400 50  0000 C CNN
F 1 "10K" V 4000 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3930 4400 50  0001 C CNN
F 3 "" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR023
U 1 1 59301904
P 3800 5150
F 0 "#PWR023" H 3800 4900 50  0001 C CNN
F 1 "Earth" H 3800 5000 50  0001 C CNN
F 2 "" H 3800 5150 50  0000 C CNN
F 3 "" H 3800 5150 50  0000 C CNN
	1    3800 5150
	-1   0    0    -1  
$EndComp
Text GLabel 4350 4950 2    60   Input ~ 0
ADC_RDY
Text GLabel 9575 2950 2    60   Input ~ 0
ADC_RDY
Text Notes 3150 1125 0    59   ~ 0
Max 6v
$Comp
L C C5
U 1 1 59302470
P 3050 1625
F 0 "C5" H 3075 1725 50  0000 L CNN
F 1 "1uf" H 3075 1525 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3088 1475 50  0001 C CNN
F 3 "" H 3050 1625 50  0001 C CNN
	1    3050 1625
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5930294B
P 4450 1625
F 0 "C6" H 4475 1725 50  0000 L CNN
F 1 "1uf" H 4475 1525 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4488 1475 50  0001 C CNN
F 3 "" H 4450 1625 50  0001 C CNN
	1    4450 1625
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5930245B
P 3700 4300
F 0 "C7" H 3725 4400 50  0000 L CNN
F 1 "C" H 3725 4200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3738 4150 50  0001 C CNN
F 3 "" H 3700 4300 50  0001 C CNN
	1    3700 4300
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR024
U 1 1 593024E3
P 3700 4450
F 0 "#PWR024" H 3700 4200 50  0001 C CNN
F 1 "Earth" H 3700 4300 50  0001 C CNN
F 2 "" H 3700 4450 50  0000 C CNN
F 3 "" H 3700 4450 50  0000 C CNN
	1    3700 4450
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR025
U 1 1 59303669
P 2900 4150
F 0 "#PWR025" H 2900 4000 50  0001 C CNN
F 1 "+3.3V" H 2900 4290 50  0000 C CNN
F 2 "" H 2900 4150 50  0001 C CNN
F 3 "" H 2900 4150 50  0001 C CNN
	1    2900 4150
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR026
U 1 1 59303771
P 4100 4150
F 0 "#PWR026" H 4100 4000 50  0001 C CNN
F 1 "+3.3V" H 4100 4290 50  0000 C CNN
F 2 "" H 4100 4150 50  0001 C CNN
F 3 "" H 4100 4150 50  0001 C CNN
	1    4100 4150
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR027
U 1 1 59316863
P 2600 1675
F 0 "#PWR027" H 2600 1425 50  0001 C CNN
F 1 "Earth" H 2600 1525 50  0001 C CNN
F 2 "" H 2600 1675 50  0000 C CNN
F 3 "" H 2600 1675 50  0000 C CNN
	1    2600 1675
	1    0    0    -1  
$EndComp
Text Notes 3175 2525 0    98   ~ 20
Debug
$Comp
L Battery BT1
U 1 1 59319095
P 2600 1475
F 0 "BT1" H 2700 1575 50  0000 L CNN
F 1 "Battery" H 2700 1475 50  0000 L CNN
F 2 "theapi:BatteryBox" V 2600 1535 50  0001 C CNN
F 3 "" V 2600 1535 50  0001 C CNN
	1    2600 1475
	1    0    0    -1  
$EndComp
$Comp
L Solar_Cells SC1
U 1 1 59328B1A
P 1800 1275
F 0 "SC1" V 1925 1200 50  0000 L CNN
F 1 "Solar_Cells" V 1650 850 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" V 1800 1335 50  0001 C CNN
F 3 "" V 1800 1335 50  0001 C CNN
	1    1800 1275
	0    1    1    0   
$EndComp
$Comp
L D_Schottky D2
U 1 1 59328B99
P 2300 1275
F 0 "D2" H 2300 1375 50  0000 C CNN
F 1 "D_Schottky" H 2300 1175 50  0000 C CNN
F 2 "Diodes_SMD:D_1206" H 2300 1275 50  0001 C CNN
F 3 "" H 2300 1275 50  0001 C CNN
	1    2300 1275
	-1   0    0    1   
$EndComp
$Comp
L Earth #PWR028
U 1 1 593290F6
P 1275 4775
F 0 "#PWR028" H 1275 4525 50  0001 C CNN
F 1 "Earth" H 1275 4625 50  0001 C CNN
F 2 "" H 1275 4775 50  0000 C CNN
F 3 "" H 1275 4775 50  0000 C CNN
	1    1275 4775
	1    0    0    -1  
$EndComp
Text GLabel 9575 3550 2    60   Input ~ 0
RFM_RST
Text GLabel 4350 6625 2    60   Input ~ 0
DIO0
Text GLabel 7625 3650 0    60   Input ~ 0
DIO0
$Comp
L C C8
U 1 1 5933CA6B
P 4775 6875
F 0 "C8" H 4800 6975 50  0000 L CNN
F 1 "10uF" H 4800 6775 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4813 6725 50  0001 C CNN
F 3 "" H 4775 6875 50  0000 C CNN
	1    4775 6875
	1    0    0    -1  
$EndComp
$Comp
L UFL_SMA U4
U 1 1 5934043A
P 1725 6425
F 0 "U4" H 1775 6275 60  0000 C CNN
F 1 "UFL_SMA" H 1725 6625 60  0000 C CNN
F 2 "theapi:coaxial_u.fl-r-smt-1" H 1725 6425 60  0001 C CNN
F 3 "" H 1725 6425 60  0001 C CNN
	1    1725 6425
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 59354FC1
P 1850 1550
F 0 "R6" V 1930 1550 50  0000 C CNN
F 1 "300K" V 1850 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1780 1550 50  0001 C CNN
F 3 "" H 1850 1550 50  0001 C CNN
	1    1850 1550
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 593551BF
P 1650 1750
F 0 "R7" V 1730 1750 50  0000 C CNN
F 1 "100K" V 1650 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1580 1750 50  0001 C CNN
F 3 "" H 1650 1750 50  0001 C CNN
	1    1650 1750
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR029
U 1 1 59355822
P 1650 1900
F 0 "#PWR029" H 1650 1650 50  0001 C CNN
F 1 "Earth" H 1650 1750 50  0001 C CNN
F 2 "" H 1650 1900 50  0000 C CNN
F 3 "" H 1650 1900 50  0000 C CNN
	1    1650 1900
	1    0    0    -1  
$EndComp
Text Label 1400 1550 0    60   ~ 0
AIN2
Text Label 1950 5050 0    60   ~ 0
AIN2
$Comp
L R R9
U 1 1 59356DA1
P 1900 5350
F 0 "R9" V 1980 5350 50  0000 C CNN
F 1 "100K" V 1900 5350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1830 5350 50  0001 C CNN
F 3 "" H 1900 5350 50  0001 C CNN
	1    1900 5350
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 59356E2E
P 1575 5150
F 0 "R8" V 1655 5150 50  0000 C CNN
F 1 "100K" V 1575 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1505 5150 50  0001 C CNN
F 3 "" H 1575 5150 50  0001 C CNN
	1    1575 5150
	0    1    1    0   
$EndComp
$Comp
L Earth #PWR030
U 1 1 59357323
P 1900 5500
F 0 "#PWR030" H 1900 5250 50  0001 C CNN
F 1 "Earth" H 1900 5350 50  0001 C CNN
F 2 "" H 1900 5500 50  0000 C CNN
F 3 "" H 1900 5500 50  0000 C CNN
	1    1900 5500
	-1   0    0    -1  
$EndComp
Text GLabel 7625 2550 0    60   Input ~ 0
LGT_GND
NoConn ~ 1925 3375
NoConn ~ 9575 2750
Text Label 7400 1600 1    60   ~ 0
24
Text Label 7700 1600 1    60   ~ 0
48
Text GLabel 1500 5500 0    60   Input ~ 0
LGT_GND
NoConn ~ 7625 2650
Text GLabel 7625 3350 0    60   Input ~ 0
USART1_TX
Text GLabel 7625 2450 0    60   Input ~ 0
LED
Text GLabel 4350 2775 2    60   Input ~ 0
LED
NoConn ~ 7625 2950
NoConn ~ 7625 3050
NoConn ~ 7625 3150
NoConn ~ 7625 3250
Text GLabel 3400 6825 0    60   Input ~ 0
SPI2_NSS
NoConn ~ 7625 3450
Text GLabel 4075 3300 2    60   Input ~ 0
USART1_TX
$Comp
L CONN_01X01 J3
U 1 1 593748B5
P 3875 3300
F 0 "J3" H 3875 3400 50  0000 C CNN
F 1 "TX" V 3975 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 3875 3300 50  0001 C CNN
F 3 "" H 3875 3300 50  0001 C CNN
	1    3875 3300
	-1   0    0    1   
$EndComp
$Comp
L Earth #PWR031
U 1 1 59375C9C
P 7700 1900
F 0 "#PWR031" H 7700 1650 50  0001 C CNN
F 1 "Earth" H 7700 1750 50  0001 C CNN
F 2 "" H 7700 1900 50  0000 C CNN
F 3 "" H 7700 1900 50  0000 C CNN
	1    7700 1900
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 J1
U 1 1 59376748
P 1225 5200
F 0 "J1" H 1225 5350 50  0000 C CNN
F 1 "LGT" V 1325 5200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1225 5200 50  0001 C CNN
F 3 "" H 1225 5200 50  0001 C CNN
	1    1225 5200
	-1   0    0    -1  
$EndComp
Text Label 1925 4850 0    60   ~ 0
AIN1
Text Label 1925 4750 0    60   ~ 0
AIN0
Text Label 1950 5150 0    60   ~ 0
AIN3
Text GLabel 1600 1275 0    60   Input ~ 0
SOL_GND
Text GLabel 1275 4200 2    60   Input ~ 0
SOL_GND
$Comp
L R_Shunt R5
U 1 1 593784A2
P 1275 4575
F 0 "R5" V 1100 4575 50  0000 C CNN
F 1 "R_Shunt" V 1175 4575 50  0000 C CNN
F 2 "theapi:SHUNT_1210_HandSoldering" V 1205 4575 50  0001 C CNN
F 3 "" H 1275 4575 50  0001 C CNN
	1    1275 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	8175 5800 8175 5950
Wire Wire Line
	8675 5800 8675 5950
Connection ~ 8575 5800
Wire Wire Line
	2425 3075 1925 3075
Wire Wire Line
	9825 5925 9325 5925
Wire Wire Line
	3550 2875 3550 2775
Wire Wire Line
	3550 2775 3750 2775
Wire Wire Line
	8475 5800 8675 5800
Wire Wire Line
	3225 7125 3400 7125
Wire Wire Line
	4350 7025 4775 7025
Wire Wire Line
	1675 6675 1675 6975
Wire Wire Line
	1675 6975 1450 6975
Wire Notes Line
	975  2125 5075 2125
Wire Notes Line
	975  725  5075 725 
Wire Notes Line
	975  2275 975  3675
Wire Notes Line
	975  3675 2925 3675
Wire Notes Line
	975  2275 2925 2275
Wire Wire Line
	3400 6425 2750 6425
Wire Wire Line
	3600 4750 4350 4750
Wire Wire Line
	3600 4850 4350 4850
Wire Wire Line
	4000 4550 4000 4850
Connection ~ 4000 4850
Wire Wire Line
	4200 4550 4200 4750
Connection ~ 4200 4750
Wire Wire Line
	4000 4250 4200 4250
Connection ~ 4100 4250
Wire Wire Line
	3800 5150 3800 4650
Wire Wire Line
	3800 4650 3600 4650
Wire Wire Line
	4350 4950 3600 4950
Wire Wire Line
	3750 1875 3750 1925
Wire Wire Line
	3050 1925 4450 1925
Wire Wire Line
	3050 1925 3050 1775
Wire Wire Line
	4150 1475 4450 1475
Wire Wire Line
	4450 1475 4450 1175
Wire Wire Line
	4450 1925 4450 1775
Connection ~ 3750 1925
Wire Wire Line
	3350 1475 3050 1475
Wire Wire Line
	3350 1625 3350 1475
Wire Wire Line
	2900 4150 3700 4150
Wire Wire Line
	2900 4150 2900 4450
Wire Wire Line
	4100 4250 4100 4150
Wire Wire Line
	2450 1275 3050 1275
Wire Wire Line
	3050 1275 3050 1475
Wire Notes Line
	5075 725  5075 2125
Wire Notes Line
	3075 2275 3075 3675
Wire Notes Line
	3075 3675 5075 3675
Wire Notes Line
	5075 3675 5075 2275
Wire Notes Line
	5075 2275 3075 2275
Wire Notes Line
	975  725  975  2125
Wire Notes Line
	2925 2275 2925 3675
Wire Notes Line
	2525 5825 975  5825
Wire Notes Line
	975  5825 975  7525
Wire Notes Line
	975  7525 2525 7525
Wire Notes Line
	2525 7525 2525 5825
Wire Notes Line
	5075 5825 2675 5825
Wire Notes Line
	2675 5825 2675 7525
Wire Notes Line
	2675 7525 5075 7525
Wire Notes Line
	5075 7525 5075 5825
Wire Wire Line
	4775 6725 4350 6725
Wire Notes Line
	975  3825 975  5675
Wire Notes Line
	975  5675 5075 5675
Wire Notes Line
	5075 5675 5075 3825
Wire Notes Line
	5075 3825 975  3825
Wire Wire Line
	7275 4650 7625 4650
Wire Wire Line
	1725 4750 2200 4750
Wire Wire Line
	1625 4850 2200 4850
Wire Wire Line
	2000 1275 2150 1275
Connection ~ 2600 1275
Wire Wire Line
	2050 6600 2050 6425
Wire Wire Line
	2050 6425 1975 6425
Wire Wire Line
	1300 6575 1300 6425
Wire Wire Line
	1300 6425 1375 6425
Wire Wire Line
	1650 1600 1650 1550
Wire Wire Line
	1400 1550 1700 1550
Wire Wire Line
	2000 1550 2000 1275
Connection ~ 1650 1550
Wire Wire Line
	1950 5050 2200 5050
Wire Wire Line
	1725 5150 2200 5150
Wire Wire Line
	8475 1425 8475 2150
Wire Wire Line
	8575 1550 8575 2150
Wire Wire Line
	8225 1550 8925 1550
Connection ~ 8475 1550
Wire Wire Line
	8925 1550 8925 2150
Connection ~ 8575 1550
Wire Wire Line
	8325 1550 8325 2150
Wire Wire Line
	8225 2150 8225 1550
Connection ~ 8325 1550
Wire Wire Line
	1900 5200 1900 5150
Connection ~ 1900 5150
Wire Wire Line
	1425 5250 1425 5350
Wire Wire Line
	1425 5350 1500 5350
Wire Wire Line
	1500 5350 1500 5500
Wire Wire Line
	1425 4675 1625 4675
Wire Wire Line
	1625 4675 1625 4850
Wire Wire Line
	1725 4750 1725 4475
Wire Wire Line
	1725 4475 1425 4475
Wire Wire Line
	1275 4200 1275 4375
$Comp
L CONN_01X01 J2
U 1 1 593799A7
P 3875 3475
F 0 "J2" H 3875 3575 50  0000 C CNN
F 1 "GND" V 3975 3475 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 3875 3475 50  0001 C CNN
F 3 "" H 3875 3475 50  0001 C CNN
	1    3875 3475
	-1   0    0    1   
$EndComp
$Comp
L Earth #PWR032
U 1 1 59379ABF
P 4075 3475
F 0 "#PWR032" H 4075 3225 50  0001 C CNN
F 1 "Earth" H 4075 3325 50  0001 C CNN
F 2 "" H 4075 3475 50  0000 C CNN
F 3 "" H 4075 3475 50  0000 C CNN
	1    4075 3475
	-1   0    0    -1  
$EndComp
NoConn ~ 9575 3450
$EndSCHEMATC
