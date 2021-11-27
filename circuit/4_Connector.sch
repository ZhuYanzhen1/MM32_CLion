EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 1850 2450 0    50   BiDi ~ 0
SWDIO
Text HLabel 1850 2550 0    50   Output ~ 0
SWDCLK
Text HLabel 3650 2550 2    50   Output ~ 0
DEBUG_TX
Text HLabel 3650 2450 2    50   Input ~ 0
DEBUG_RX
$Comp
L MM32_Periph:Debugger M?
U 1 1 619F71DE
P 2400 2750
F 0 "M?" H 2450 3300 50  0000 C CNN
F 1 "Debugger" H 2600 2700 50  0000 C CNN
F 2 "" H 2450 3300 50  0001 C CNN
F 3 "" H 2450 3300 50  0001 C CNN
	1    2400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2450 3650 2450
Wire Wire Line
	3650 2550 3250 2550
Wire Wire Line
	1850 2450 2300 2450
Wire Wire Line
	2300 2550 1850 2550
$Comp
L power:+3.3V #PWR0240
U 1 1 619F94A5
P 3450 1700
F 0 "#PWR0240" H 3450 1550 50  0001 C CNN
F 1 "+3.3V" H 3465 1873 50  0000 C CNN
F 2 "" H 3450 1700 50  0001 C CNN
F 3 "" H 3450 1700 50  0001 C CNN
	1    3450 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0241
U 1 1 619F9BA6
P 3450 3400
F 0 "#PWR0241" H 3450 3150 50  0001 C CNN
F 1 "GND" H 3455 3227 50  0000 C CNN
F 2 "" H 3450 3400 50  0001 C CNN
F 3 "" H 3450 3400 50  0001 C CNN
	1    3450 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0242
U 1 1 619F9F09
P 2100 3400
F 0 "#PWR0242" H 2100 3150 50  0001 C CNN
F 1 "GND" H 2105 3227 50  0000 C CNN
F 2 "" H 2100 3400 50  0001 C CNN
F 3 "" H 2100 3400 50  0001 C CNN
	1    2100 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0243
U 1 1 619FA449
P 2100 1700
F 0 "#PWR0243" H 2100 1550 50  0001 C CNN
F 1 "+3.3V" H 2115 1873 50  0000 C CNN
F 2 "" H 2100 1700 50  0001 C CNN
F 3 "" H 2100 1700 50  0001 C CNN
	1    2100 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1700 2100 2350
Wire Wire Line
	2100 2350 2300 2350
Wire Wire Line
	3450 1700 3450 2350
Wire Wire Line
	3450 2350 3250 2350
Wire Wire Line
	3250 2650 3450 2650
Wire Wire Line
	3450 2650 3450 3400
Wire Wire Line
	2300 2650 2100 2650
Wire Wire Line
	2100 2650 2100 3400
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 61B1D9F4
P 4750 2550
F 0 "J?" H 4700 2350 50  0000 L CNN
F 1 "VOUT" H 4700 2650 50  0000 L CNN
F 2 "" H 4750 2550 50  0001 C CNN
F 3 "~" H 4750 2550 50  0001 C CNN
	1    4750 2550
	1    0    0    1   
$EndComp
$Comp
L power:+15V #PWR?
U 1 1 61B26E64
P 4550 1700
F 0 "#PWR?" H 4550 1550 50  0001 C CNN
F 1 "+15V" H 4565 1873 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1700 4550 2450
$Comp
L power:GND #PWR?
U 1 1 61B26FE3
P 4550 3400
F 0 "#PWR?" H 4550 3150 50  0001 C CNN
F 1 "GND" H 4555 3227 50  0000 C CNN
F 2 "" H 4550 3400 50  0001 C CNN
F 3 "" H 4550 3400 50  0001 C CNN
	1    4550 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 2550 4550 3400
$Comp
L MM32_Periph:1.8inch-TFT-LCD LCD?
U 1 1 61A05F33
P 6100 3050
F 0 "LCD?" H 6200 4100 50  0000 C CNN
F 1 "1.8inch-TFT-LCD" H 6450 3000 50  0000 C CNN
F 2 "" H 6200 4100 50  0001 C CNN
F 3 "" H 6200 4100 50  0001 C CNN
	1    6100 3050
	1    0    0    -1  
$EndComp
$Comp
L MM32_Periph:Camera J?
U 1 1 61A06CE0
P 8350 3050
F 0 "J?" H 8400 4100 50  0000 C CNN
F 1 "Camera" H 8500 3000 50  0000 C CNN
F 2 "" H 8400 4100 50  0001 C CNN
F 3 "" H 8400 4100 50  0001 C CNN
	1    8350 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61A0AA20
P 5900 1700
F 0 "#PWR?" H 5900 1550 50  0001 C CNN
F 1 "+3.3V" H 5915 1873 50  0000 C CNN
F 2 "" H 5900 1700 50  0001 C CNN
F 3 "" H 5900 1700 50  0001 C CNN
	1    5900 1700
	1    0    0    -1  
$EndComp
$Comp
L Driver_Display:XPT2046TS U?
U 1 1 61A0B2BA
P 2750 5500
F 0 "U?" H 2400 6050 50  0000 C CNN
F 1 "XPT2046TS" H 2550 5050 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 2750 4900 50  0001 C CIN
F 3 "http://www.xptek.cn/uploadfile/download/201707171401161883.pdf" H 2950 4950 50  0001 C CNN
	1    2750 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A23F37
P 5800 3400
F 0 "#PWR?" H 5800 3150 50  0001 C CNN
F 1 "GND" H 5805 3227 50  0000 C CNN
F 2 "" H 5800 3400 50  0001 C CNN
F 3 "" H 5800 3400 50  0001 C CNN
	1    5800 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3400 5800 2550
Wire Wire Line
	5800 2250 6000 2250
Wire Wire Line
	6000 2350 5800 2350
Connection ~ 5800 2350
Wire Wire Line
	5800 2350 5800 2250
Wire Wire Line
	6000 2450 5900 2450
Wire Wire Line
	5900 2450 5900 1700
Wire Wire Line
	6000 2550 5800 2550
Connection ~ 5800 2550
Wire Wire Line
	5800 2550 5800 2350
$Comp
L power:GND #PWR?
U 1 1 61A2920F
P 6950 3400
F 0 "#PWR?" H 6950 3150 50  0001 C CNN
F 1 "GND" H 6955 3227 50  0000 C CNN
F 2 "" H 6950 3400 50  0001 C CNN
F 3 "" H 6950 3400 50  0001 C CNN
	1    6950 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2650 6950 2650
Wire Wire Line
	6950 2650 6950 3400
$Comp
L power:+3.3V #PWR?
U 1 1 61A2A54F
P 7050 1700
F 0 "#PWR?" H 7050 1550 50  0001 C CNN
F 1 "+3.3V" H 7065 1873 50  0000 C CNN
F 2 "" H 7050 1700 50  0001 C CNN
F 3 "" H 7050 1700 50  0001 C CNN
	1    7050 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2850 7050 2850
Wire Wire Line
	7050 2850 7050 1700
Wire Wire Line
	6850 2950 7050 2950
Wire Wire Line
	7050 2950 7050 2850
Connection ~ 7050 2850
Text Label 7350 2150 2    50   ~ 0
T_XP
Text Label 7350 2250 2    50   ~ 0
T_YP
Text Label 7350 2350 2    50   ~ 0
T_XM
Text Label 7350 2450 2    50   ~ 0
T_YM
Wire Wire Line
	6850 2150 7350 2150
Wire Wire Line
	7350 2250 6850 2250
Wire Wire Line
	6850 2350 7350 2350
Wire Wire Line
	7350 2450 6850 2450
Text HLabel 7150 2750 2    50   Input ~ 0
LCD_CS
Text HLabel 3600 5500 2    50   Input ~ 0
LCD_CLK
Text HLabel 3600 5300 2    50   Input ~ 0
LCD_MOSI
Text HLabel 5700 2750 0    50   Input ~ 0
LCD_A0
Text HLabel 5700 2650 0    50   Input ~ 0
LCD_RST
Wire Wire Line
	5700 2650 6000 2650
Wire Wire Line
	6000 2750 5700 2750
Wire Wire Line
	5350 2850 6000 2850
Wire Wire Line
	6000 2950 5350 2950
Wire Wire Line
	6850 2750 7150 2750
$Comp
L power:GND #PWR?
U 1 1 61A377C1
P 1900 6300
F 0 "#PWR?" H 1900 6050 50  0001 C CNN
F 1 "GND" H 1905 6127 50  0000 C CNN
F 2 "" H 1900 6300 50  0001 C CNN
F 3 "" H 1900 6300 50  0001 C CNN
	1    1900 6300
	1    0    0    -1  
$EndComp
Text HLabel 3600 5400 2    50   Output ~ 0
LCD_MISO
Text HLabel 3600 5700 2    50   Input ~ 0
TOUCH_CS
Wire Wire Line
	3250 5700 3600 5700
Wire Wire Line
	3600 5500 3250 5500
Wire Wire Line
	3250 5400 3600 5400
Wire Wire Line
	3600 5300 3250 5300
Text Label 3400 5500 0    50   ~ 0
CLK
Text Label 3400 5300 0    50   ~ 0
DIN
Text Label 5350 2850 0    50   ~ 0
DIN
Text Label 5350 2950 0    50   ~ 0
CLK
$Comp
L power:+3.3V #PWR?
U 1 1 61A444F1
P 1950 4550
F 0 "#PWR?" H 1950 4400 50  0001 C CNN
F 1 "+3.3V" H 1965 4723 50  0000 C CNN
F 2 "" H 1950 4550 50  0001 C CNN
F 3 "" H 1950 4550 50  0001 C CNN
	1    1950 4550
	1    0    0    -1  
$EndComp
Text Label 1750 5100 0    50   ~ 0
T_XP
Text Label 1750 5200 0    50   ~ 0
T_YP
Text Label 1750 5300 0    50   ~ 0
T_XM
Text Label 1750 5400 0    50   ~ 0
T_YM
Wire Wire Line
	1750 5100 2250 5100
Wire Wire Line
	2250 5200 1750 5200
Wire Wire Line
	1750 5300 2250 5300
Wire Wire Line
	2250 5400 1750 5400
Wire Wire Line
	1900 6300 1900 5800
Wire Wire Line
	1900 5800 2250 5800
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 61A4F77C
P 10350 2500
F 0 "J?" H 10430 2542 50  0000 L CNN
F 1 "Conn_01x03" H 10430 2451 50  0000 L CNN
F 2 "" H 10350 2500 50  0001 C CNN
F 3 "~" H 10350 2500 50  0001 C CNN
	1    10350 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A525A9
P 10000 3400
F 0 "#PWR?" H 10000 3150 50  0001 C CNN
F 1 "GND" H 10005 3227 50  0000 C CNN
F 2 "" H 10000 3400 50  0001 C CNN
F 3 "" H 10000 3400 50  0001 C CNN
	1    10000 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 3400 10000 2600
Wire Wire Line
	10000 2600 10150 2600
Text HLabel 9900 2400 0    50   Input ~ 0
MOTOR_TX
Text HLabel 9900 2500 0    50   Output ~ 0
MOTOR_RX
Wire Wire Line
	9900 2400 10150 2400
Wire Wire Line
	10150 2500 9900 2500
$EndSCHEMATC
