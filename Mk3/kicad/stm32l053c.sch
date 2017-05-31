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
Title "thingBot-LoRa"
Date "2016-11-24"
Rev "v1.1"
Comp "thingTronics Innovations Pvt. Ltd."
Comment1 "Approved By: Lovelesh Patel"
Comment2 "Designed By: Manohar Bompella"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Earth #PWR01
U 1 1 58021BE1
P 9600 1250
F 0 "#PWR01" H 9600 1000 50  0001 C CNN
F 1 "Earth" H 9600 1100 50  0001 C CNN
F 2 "" H 9600 1250 50  0000 C CNN
F 3 "" H 9600 1250 50  0000 C CNN
	1    9600 1250
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 58021E64
P 9600 1100
F 0 "C5" H 9625 1200 50  0000 L CNN
F 1 "100nF" H 9625 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9638 950 50  0001 C CNN
F 3 "" H 9600 1100 50  0000 C CNN
	1    9600 1100
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR02
U 1 1 58021F75
P 7000 6100
F 0 "#PWR02" H 7000 5850 50  0001 C CNN
F 1 "GNDA" H 7000 5950 50  0000 C CNN
F 2 "" H 7000 6100 50  0000 C CNN
F 3 "" H 7000 6100 50  0000 C CNN
	1    7000 6100
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR03
U 1 1 58021F89
P 7500 6100
F 0 "#PWR03" H 7500 5850 50  0001 C CNN
F 1 "Earth" H 7500 5950 50  0001 C CNN
F 2 "" H 7500 6100 50  0000 C CNN
F 3 "" H 7500 6100 50  0000 C CNN
	1    7500 6100
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR04
U 1 1 58022154
P 7000 5600
F 0 "#PWR04" H 7000 5350 50  0001 C CNN
F 1 "GNDA" H 7000 5450 50  0000 C CNN
F 2 "" H 7000 5600 50  0000 C CNN
F 3 "" H 7000 5600 50  0000 C CNN
	1    7000 5600
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR05
U 1 1 580221FA
P 7500 5600
F 0 "#PWR05" H 7500 5350 50  0001 C CNN
F 1 "Earth" H 7500 5450 50  0001 C CNN
F 2 "" H 7500 5600 50  0000 C CNN
F 3 "" H 7500 5600 50  0000 C CNN
	1    7500 5600
	1    0    0    -1  
$EndComp
Text GLabel 8900 2400 2    60   Input ~ 0
SWO
Text GLabel 6100 3500 0    60   Input ~ 0
SWCLK
Text GLabel 6100 3400 0    60   Input ~ 0
SWDIO
Text GLabel 6450 4100 0    60   Input ~ 0
NRST
$Comp
L CONN_01X06 P1
U 1 1 58036840
P 3300 1300
F 0 "P1" H 3300 1650 50  0000 C CNN
F 1 "SWD" V 3400 1300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06" H 3300 1300 50  0001 C CNN
F 3 "" H 3300 1300 50  0000 C CNN
	1    3300 1300
	-1   0    0    -1  
$EndComp
$Comp
L Earth #PWR06
U 1 1 580369A9
P 4000 1250
F 0 "#PWR06" H 4000 1000 50  0001 C CNN
F 1 "Earth" H 4000 1100 50  0001 C CNN
F 2 "" H 4000 1250 50  0000 C CNN
F 3 "" H 4000 1250 50  0000 C CNN
	1    4000 1250
	1    0    0    -1  
$EndComp
Text GLabel 3500 1150 2    60   Input ~ 0
SWCLK
Text GLabel 3500 1350 2    60   Input ~ 0
SWDIO
Text GLabel 3500 1450 2    60   Input ~ 0
NRST
Text GLabel 3500 1550 2    60   Input ~ 0
SWO
Text Label 6150 2100 0    60   ~ 0
PA0
$Comp
L Earth #PWR07
U 1 1 58039244
P 9600 2000
F 0 "#PWR07" H 9600 1750 50  0001 C CNN
F 1 "Earth" H 9600 1850 50  0001 C CNN
F 2 "" H 9600 2000 50  0000 C CNN
F 3 "" H 9600 2000 50  0000 C CNN
	1    9600 2000
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 58039250
P 9600 1850
F 0 "C6" H 9625 1950 50  0000 L CNN
F 1 "10uF" H 9625 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9638 1700 50  0001 C CNN
F 3 "" H 9600 1850 50  0000 C CNN
	1    9600 1850
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5803B990
P 9900 1100
F 0 "C7" H 9925 1200 50  0000 L CNN
F 1 "100nF" H 9925 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9938 950 50  0001 C CNN
F 3 "" H 9900 1100 50  0000 C CNN
	1    9900 1100
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5803BA4A
P 10650 1100
F 0 "C8" H 10675 1200 50  0000 L CNN
F 1 "100nF" H 10675 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 10688 950 50  0001 C CNN
F 3 "" H 10650 1100 50  0000 C CNN
	1    10650 1100
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR08
U 1 1 5803BDC8
P 10650 1250
F 0 "#PWR08" H 10650 1000 50  0001 C CNN
F 1 "Earth" H 10650 1100 50  0001 C CNN
F 2 "" H 10650 1250 50  0000 C CNN
F 3 "" H 10650 1250 50  0000 C CNN
	1    10650 1250
	1    0    0    -1  
$EndComp
Text Label 10650 850  0    60   ~ 0
VDDA
Text Label 7150 1800 1    60   ~ 0
VDDA
Text Label 7050 1800 1    60   ~ 0
VLCD
Text Label 7300 1800 1    60   ~ 0
VDD
Text Label 7750 1800 1    60   ~ 0
VDD_USB
Text Label 7400 1800 1    60   ~ 0
VDD
$Comp
L STM32L053C8_LQFP48 U1
U 1 1 580631B3
P 7300 3900
F 0 "U1" H 7950 5900 60  0000 C CNN
F 1 "STM32L053C8_LQFP48" H 7400 3900 60  0000 C CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 7100 2000 60  0001 C CNN
F 3 "" H 7100 2000 60  0000 C CNN
	1    7300 3900
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5806575A
P 5550 4450
F 0 "R1" V 5630 4450 50  0000 C CNN
F 1 "10K" V 5550 4450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5480 4450 50  0001 C CNN
F 3 "" H 5550 4450 50  0000 C CNN
	1    5550 4450
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR09
U 1 1 58065816
P 5550 4600
F 0 "#PWR09" H 5550 4350 50  0001 C CNN
F 1 "Earth" H 5550 4450 50  0001 C CNN
F 2 "" H 5550 4600 50  0000 C CNN
F 3 "" H 5550 4600 50  0000 C CNN
	1    5550 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5450 7000 5600
Wire Wire Line
	7500 5450 7500 5600
Connection ~ 7400 5450
Wire Wire Line
	7300 850  7300 1800
Wire Wire Line
	7400 1050 7400 1800
Wire Wire Line
	7150 1050 7150 1800
Wire Wire Line
	7050 1050 7750 1050
Connection ~ 7300 1050
Wire Wire Line
	7050 1050 7050 1800
Wire Wire Line
	7750 1050 7750 1800
Wire Wire Line
	6450 3400 6100 3400
Wire Wire Line
	6100 3500 6450 3500
Wire Wire Line
	4000 1250 3500 1250
Wire Wire Line
	7500 6100 7000 6100
Wire Wire Line
	6150 2800 6450 2800
Wire Wire Line
	6150 2700 6450 2700
Wire Wire Line
	6450 2600 6150 2600
Wire Wire Line
	6150 2500 6450 2500
Wire Wire Line
	6150 2100 6450 2100
Wire Wire Line
	8400 2400 8900 2400
Wire Wire Line
	8400 2700 8750 2700
Wire Wire Line
	8400 2800 8750 2800
Wire Wire Line
	10650 850  10650 950 
Connection ~ 7400 1050
Wire Wire Line
	9900 950  9600 950 
Wire Wire Line
	9900 1250 9600 1250
$Comp
L R R4
U 1 1 580770D7
P 10350 3150
F 0 "R4" V 10430 3150 50  0000 C CNN
F 1 "820" V 10350 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 10280 3150 50  0001 C CNN
F 3 "" H 10350 3150 50  0000 C CNN
	1    10350 3150
	0    1    1    0   
$EndComp
$Comp
L Earth #PWR010
U 1 1 58077163
P 9700 3250
F 0 "#PWR010" H 9700 3000 50  0001 C CNN
F 1 "Earth" H 9700 3100 50  0001 C CNN
F 2 "" H 9700 3250 50  0000 C CNN
F 3 "" H 9700 3250 50  0000 C CNN
	1    9700 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9700 3250 9700 3150
Wire Wire Line
	9700 3150 9900 3150
Text Label 10700 3150 2    60   ~ 0
PA0
Wire Wire Line
	10700 3150 10500 3150
NoConn ~ 6450 4600
NoConn ~ 6450 4700
Wire Wire Line
	7300 5450 7500 5450
$Comp
L LED D1
U 1 1 592DE199
P 10050 3150
F 0 "D1" H 10050 3250 50  0000 C CNN
F 1 "LED" H 10050 3050 50  0000 C CNN
F 2 "" H 10050 3150 50  0001 C CNN
F 3 "" H 10050 3150 50  0001 C CNN
	1    10050 3150
	1    0    0    -1  
$EndComp
Connection ~ 7150 1050
$Comp
L ADS1015 U2
U 1 1 592DD751
P 1600 3250
F 0 "U2" H 1000 3700 50  0000 L CNN
F 1 "ADS1015" H 1850 3700 50  0000 L CNN
F 2 "" H 1550 3200 50  0001 C CNN
F 3 "" H 700 3650 50  0001 C CNN
	1    1600 3250
	1    0    0    -1  
$EndComp
Text GLabel 1175 6850 0    60   Input ~ 0
ANT
Wire Wire Line
	1025 6250 1025 6450
Wire Wire Line
	1025 6350 1100 6350
Wire Wire Line
	1025 6250 1100 6250
Connection ~ 1025 6350
Wire Wire Line
	1700 6250 1775 6250
Wire Wire Line
	1775 6250 1775 6475
Wire Wire Line
	1700 6350 1775 6350
Connection ~ 1775 6350
Wire Wire Line
	3050 7000 3225 7000
Wire Wire Line
	4175 6600 4725 6600
Wire Wire Line
	4175 6900 4600 6900
Wire Notes Line
	700  5875 700  7325
Wire Notes Line
	700  7325 2250 7325
Text Notes 1150 5900 0    98   ~ 20
Antenna\n
Wire Notes Line
	700  5900 700  5725
Wire Notes Line
	700  5725 2250 5725
Wire Notes Line
	2250 5725 2250 7325
Text Notes 2950 5900 0    98   ~ 20
RFM 95/96/92 Module
Wire Notes Line
	2475 5700 2475 7375
Wire Notes Line
	2475 7375 4900 7375
Wire Notes Line
	4900 7375 4900 5700
Wire Notes Line
	4900 5700 2475 5700
Text Notes 1000 7250 0    59   ~ 0
Place on Top layer
$Comp
L EDGE_SMA U4
U 1 1 592DE9B4
P 1450 6300
F 0 "U4" H 1500 6150 60  0000 C CNN
F 1 "EDGE_SMA" H 1450 6500 60  0000 C CNN
F 2 "sma:EDGE_SMA_5" H 1450 6300 60  0001 C CNN
F 3 "" H 1450 6300 60  0001 C CNN
F 4 "SMA RF Connector" H 1450 6300 60  0001 C CNN "DESC"
F 5 "molex" H 1450 6300 60  0001 C CNN "MFG_NAME"
F 6 "0732511150" H 1450 6300 60  0001 C CNN "MPN"
	1    1450 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 6550 1400 6850
Wire Wire Line
	1400 6850 1175 6850
$Comp
L Earth #PWR011
U 1 1 592DF55A
P 1600 3750
F 0 "#PWR011" H 1600 3500 50  0001 C CNN
F 1 "Earth" H 1600 3600 50  0001 C CNN
F 2 "" H 1600 3750 50  0000 C CNN
F 3 "" H 1600 3750 50  0000 C CNN
	1    1600 3750
	-1   0    0    -1  
$EndComp
Text Notes 1450 2400 0    98   ~ 20
ADC
$Comp
L VDD #PWR012
U 1 1 592DF7A0
P 1600 2750
F 0 "#PWR012" H 1600 2600 50  0001 C CNN
F 1 "VDD" H 1600 2900 50  0000 C CNN
F 2 "" H 1600 2750 50  0000 C CNN
F 3 "" H 1600 2750 50  0000 C CNN
	1    1600 2750
	1    0    0    -1  
$EndComp
Wire Notes Line
	550  2200 2650 2200
Wire Notes Line
	2650 2200 2650 4000
Wire Notes Line
	2650 4000 550  4000
Wire Notes Line
	550  4000 550  2200
Text Notes 2750 800  0    98   ~ 20
Programming Header
$Comp
L AP2112 U3
U 1 1 592F19AA
P 1450 1400
F 0 "U3" H 1200 1650 50  0000 L CNN
F 1 "AP2112" H 1500 1650 50  0000 L CNN
F 2 "" H 1400 1350 50  0001 C CNN
F 3 "" H 550 1800 50  0001 C CNN
	1    1450 1400
	1    0    0    -1  
$EndComp
Text Notes 650  750  0    98   ~ 20
3v3 Voltage Regulator
$Comp
L VDD #PWR013
U 1 1 592F1B4B
P 750 1400
F 0 "#PWR013" H 750 1250 50  0001 C CNN
F 1 "VDD" H 750 1550 50  0000 C CNN
F 2 "" H 750 1400 50  0000 C CNN
F 3 "" H 750 1400 50  0000 C CNN
	1    750  1400
	1    0    0    -1  
$EndComp
NoConn ~ 1850 1550
$Comp
L Earth #PWR014
U 1 1 592F1C89
P 1450 1800
F 0 "#PWR014" H 1450 1550 50  0001 C CNN
F 1 "Earth" H 1450 1650 50  0001 C CNN
F 2 "" H 1450 1800 50  0000 C CNN
F 3 "" H 1450 1800 50  0000 C CNN
	1    1450 1800
	1    0    0    -1  
$EndComp
Wire Notes Line
	550  550  550  1950
Wire Notes Line
	550  1950 2400 1950
Wire Notes Line
	2400 1950 2400 550 
Wire Notes Line
	2400 550  550  550 
Wire Notes Line
	2600 550  2600 1950
Wire Notes Line
	2600 1950 4450 1950
Wire Notes Line
	4450 1950 4450 550 
Wire Notes Line
	4450 550  2600 550 
Text GLabel 8750 2800 2    60   Input ~ 0
I2C1_SDA
Text GLabel 8750 2700 2    60   Input ~ 0
I2C1_SCL
Text GLabel 2300 3050 2    60   Input ~ 0
I2C1_SCL
Text GLabel 2300 3150 2    60   Input ~ 0
I2C1_SDA
Text GLabel 6150 2800 0    60   Input ~ 0
SPI1_MOSI
Text GLabel 6150 2700 0    60   Input ~ 0
SPI1_MISO
Text GLabel 6150 2600 0    60   Input ~ 0
SPI1_SCK
Text GLabel 3225 6600 0    60   Input ~ 0
SPI1_SCK
Text GLabel 3225 6400 0    60   Input ~ 0
SPI1_MISO
$Comp
L RFM92/95/96/97/98 U5
U 1 1 592DE9B5
P 3725 6700
F 0 "U5" H 3775 6300 60  0000 C CNN
F 1 "RFM92/95" H 3725 7250 60  0000 C CNN
F 2 "Temp:RFM92_95_96_98" H 3925 6900 60  0001 C CNN
F 3 "" H 3925 6900 60  0001 C CNN
F 4 "Low Power Long Range Transceiver Module" H 3725 6700 60  0001 C CNN "DESC"
F 5 "....." H 3725 6700 60  0001 C CNN "MFG_NAME"
F 6 "..." H 3725 6700 60  0001 C CNN "MPN"
	1    3725 6700
	1    0    0    -1  
$EndComp
Text GLabel 3225 6500 0    60   Input ~ 0
SPI1_MOSI
Text GLabel 6150 2500 0    60   Input ~ 0
SPI1_CS
Text GLabel 3225 6700 0    60   Input ~ 0
SPI1_CS
Text GLabel 3225 6800 0    60   Input ~ 0
NRST
$Comp
L +3.3V #PWR015
U 1 1 592F43A5
P 2100 1400
F 0 "#PWR015" H 2100 1250 50  0001 C CNN
F 1 "+3.3V" H 2100 1540 50  0000 C CNN
F 2 "" H 2100 1400 50  0001 C CNN
F 3 "" H 2100 1400 50  0001 C CNN
	1    2100 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1400 2100 1400
Wire Wire Line
	750  1400 1050 1400
Wire Wire Line
	750  1400 750  1550
Wire Wire Line
	750  1550 1050 1550
$Comp
L +3.3V #PWR016
U 1 1 592F4BC3
P 3500 1050
F 0 "#PWR016" H 3500 900 50  0001 C CNN
F 1 "+3.3V" H 3500 1190 50  0000 C CNN
F 2 "" H 3500 1050 50  0001 C CNN
F 3 "" H 3500 1050 50  0001 C CNN
	1    3500 1050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 592F4E66
P 7300 850
F 0 "#PWR017" H 7300 700 50  0001 C CNN
F 1 "+3.3V" H 7300 990 50  0000 C CNN
F 2 "" H 7300 850 50  0001 C CNN
F 3 "" H 7300 850 50  0001 C CNN
	1    7300 850 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 592F5385
P 9350 950
F 0 "#PWR018" H 9350 800 50  0001 C CNN
F 1 "+3.3V" H 9350 1090 50  0000 C CNN
F 2 "" H 9350 950 50  0001 C CNN
F 3 "" H 9350 950 50  0001 C CNN
	1    9350 950 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 592F53D7
P 4725 6600
F 0 "#PWR019" H 4725 6450 50  0001 C CNN
F 1 "+3.3V" H 4725 6740 50  0000 C CNN
F 2 "" H 4725 6600 50  0001 C CNN
F 3 "" H 4725 6600 50  0001 C CNN
	1    4725 6600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 592F540C
P 9600 950
F 0 "#PWR020" H 9600 800 50  0001 C CNN
F 1 "+3.3V" H 9600 1090 50  0000 C CNN
F 2 "" H 9600 950 50  0001 C CNN
F 3 "" H 9600 950 50  0001 C CNN
	1    9600 950 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 592F54CD
P 9600 1700
F 0 "#PWR021" H 9600 1550 50  0001 C CNN
F 1 "+3.3V" H 9600 1840 50  0000 C CNN
F 2 "" H 9600 1700 50  0001 C CNN
F 3 "" H 9600 1700 50  0001 C CNN
	1    9600 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3225 6300 2575 6300
Text GLabel 4175 7000 2    60   Input ~ 0
ANT
Wire Wire Line
	6450 4300 5550 4300
NoConn ~ 6450 4800
NoConn ~ 6450 4900
NoConn ~ 6450 2200
NoConn ~ 6450 2300
NoConn ~ 6450 2400
NoConn ~ 6450 2900
NoConn ~ 6450 3000
NoConn ~ 6450 3100
NoConn ~ 6450 3200
NoConn ~ 6450 3300
NoConn ~ 6450 3600
NoConn ~ 8400 2100
NoConn ~ 8400 2200
NoConn ~ 8400 2500
NoConn ~ 8400 2600
NoConn ~ 8400 2900
NoConn ~ 8400 3000
NoConn ~ 8400 3100
NoConn ~ 8400 3200
NoConn ~ 8400 3300
NoConn ~ 8400 3400
NoConn ~ 8400 3500
NoConn ~ 8400 3600
NoConn ~ 8400 4200
NoConn ~ 8400 2300
NoConn ~ 4175 6300
NoConn ~ 4175 6400
NoConn ~ 4175 6500
NoConn ~ 4175 6700
NoConn ~ 4175 6800
NoConn ~ 3225 6900
$Comp
L Earth #PWR022
U 1 1 592F7C8A
P 4600 6900
F 0 "#PWR022" H 4600 6650 50  0001 C CNN
F 1 "Earth" H 4600 6750 50  0001 C CNN
F 2 "" H 4600 6900 50  0000 C CNN
F 3 "" H 4600 6900 50  0000 C CNN
	1    4600 6900
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR023
U 1 1 592F7CF0
P 2575 6300
F 0 "#PWR023" H 2575 6050 50  0001 C CNN
F 1 "Earth" H 2575 6150 50  0001 C CNN
F 2 "" H 2575 6300 50  0000 C CNN
F 3 "" H 2575 6300 50  0000 C CNN
	1    2575 6300
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR024
U 1 1 592F7EE4
P 3050 7000
F 0 "#PWR024" H 3050 6750 50  0001 C CNN
F 1 "Earth" H 3050 6850 50  0001 C CNN
F 2 "" H 3050 7000 50  0000 C CNN
F 3 "" H 3050 7000 50  0000 C CNN
	1    3050 7000
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR025
U 1 1 592F80DC
P 1025 6450
F 0 "#PWR025" H 1025 6200 50  0001 C CNN
F 1 "Earth" H 1025 6300 50  0001 C CNN
F 2 "" H 1025 6450 50  0000 C CNN
F 3 "" H 1025 6450 50  0000 C CNN
	1    1025 6450
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR026
U 1 1 592F814F
P 1775 6475
F 0 "#PWR026" H 1775 6225 50  0001 C CNN
F 1 "Earth" H 1775 6325 50  0001 C CNN
F 2 "" H 1775 6475 50  0000 C CNN
F 3 "" H 1775 6475 50  0000 C CNN
	1    1775 6475
	1    0    0    -1  
$EndComp
$EndSCHEMATC
