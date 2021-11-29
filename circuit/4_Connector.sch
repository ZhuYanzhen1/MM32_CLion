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
Text HLabel 1750 2650 0    50   BiDi ~ 0
SWDIO
Text HLabel 1750 2750 0    50   Output ~ 0
SWDCLK
Text HLabel 3550 2750 2    50   Output ~ 0
DEBUG_TX
Text HLabel 3550 2650 2    50   Input ~ 0
DEBUG_RX
$Comp
L MM32_Periph:Debugger M?
U 1 1 619F71DE
P 2300 2950
F 0 "M?" H 2350 3500 50  0000 C CNN
F 1 "Debugger" H 2500 2900 50  0000 C CNN
F 2 "" H 2350 3500 50  0001 C CNN
F 3 "" H 2350 3500 50  0001 C CNN
	1    2300 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2650 3550 2650
Wire Wire Line
	3550 2750 3150 2750
Wire Wire Line
	1750 2650 2200 2650
Wire Wire Line
	2200 2750 1750 2750
$Comp
L power:+3.3V #PWR0240
U 1 1 619F94A5
P 3350 1900
F 0 "#PWR0240" H 3350 1750 50  0001 C CNN
F 1 "+3.3V" H 3365 2073 50  0000 C CNN
F 2 "" H 3350 1900 50  0001 C CNN
F 3 "" H 3350 1900 50  0001 C CNN
	1    3350 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0241
U 1 1 619F9BA6
P 3350 3600
F 0 "#PWR0241" H 3350 3350 50  0001 C CNN
F 1 "GND" H 3355 3427 50  0000 C CNN
F 2 "" H 3350 3600 50  0001 C CNN
F 3 "" H 3350 3600 50  0001 C CNN
	1    3350 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0242
U 1 1 619F9F09
P 2000 3600
F 0 "#PWR0242" H 2000 3350 50  0001 C CNN
F 1 "GND" H 2005 3427 50  0000 C CNN
F 2 "" H 2000 3600 50  0001 C CNN
F 3 "" H 2000 3600 50  0001 C CNN
	1    2000 3600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0243
U 1 1 619FA449
P 2000 1900
F 0 "#PWR0243" H 2000 1750 50  0001 C CNN
F 1 "+3.3V" H 2015 2073 50  0000 C CNN
F 2 "" H 2000 1900 50  0001 C CNN
F 3 "" H 2000 1900 50  0001 C CNN
	1    2000 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1900 2000 2550
Wire Wire Line
	2000 2550 2200 2550
Wire Wire Line
	3350 1900 3350 2550
Wire Wire Line
	3350 2550 3150 2550
Wire Wire Line
	3150 2850 3350 2850
Wire Wire Line
	3350 2850 3350 3600
Wire Wire Line
	2200 2850 2000 2850
Wire Wire Line
	2000 2850 2000 3600
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 61B1D9F4
P 4650 2750
F 0 "J?" H 4600 2550 50  0000 L CNN
F 1 "VOUT" H 4600 2850 50  0000 L CNN
F 2 "" H 4650 2750 50  0001 C CNN
F 3 "~" H 4650 2750 50  0001 C CNN
	1    4650 2750
	1    0    0    1   
$EndComp
$Comp
L power:+15V #PWR?
U 1 1 61B26E64
P 4450 1900
F 0 "#PWR?" H 4450 1750 50  0001 C CNN
F 1 "+15V" H 4465 2073 50  0000 C CNN
F 2 "" H 4450 1900 50  0001 C CNN
F 3 "" H 4450 1900 50  0001 C CNN
	1    4450 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1900 4450 2650
$Comp
L power:GND #PWR?
U 1 1 61B26FE3
P 4450 3600
F 0 "#PWR?" H 4450 3350 50  0001 C CNN
F 1 "GND" H 4455 3427 50  0000 C CNN
F 2 "" H 4450 3600 50  0001 C CNN
F 3 "" H 4450 3600 50  0001 C CNN
	1    4450 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2750 4450 3600
$Comp
L MM32_Periph:1.8inch-TFT-LCD LCD?
U 1 1 61A05F33
P 6000 3250
F 0 "LCD?" H 6100 4300 50  0000 C CNN
F 1 "1.8inch-TFT-LCD" H 6350 3200 50  0000 C CNN
F 2 "" H 6100 4300 50  0001 C CNN
F 3 "" H 6100 4300 50  0001 C CNN
	1    6000 3250
	1    0    0    -1  
$EndComp
$Comp
L MM32_Periph:Camera J?
U 1 1 61A06CE0
P 8250 3250
F 0 "J?" H 8300 4300 50  0000 C CNN
F 1 "Camera" H 8400 3200 50  0000 C CNN
F 2 "" H 8300 4300 50  0001 C CNN
F 3 "" H 8300 4300 50  0001 C CNN
	1    8250 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61A0AA20
P 5800 1900
F 0 "#PWR?" H 5800 1750 50  0001 C CNN
F 1 "+3.3V" H 5815 2073 50  0000 C CNN
F 2 "" H 5800 1900 50  0001 C CNN
F 3 "" H 5800 1900 50  0001 C CNN
	1    5800 1900
	1    0    0    -1  
$EndComp
$Comp
L Driver_Display:XPT2046TS U?
U 1 1 61A0B2BA
P 2750 5750
F 0 "U?" H 2400 6300 50  0000 C CNN
F 1 "XPT2046TS" H 2550 5300 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 2750 5150 50  0001 C CIN
F 3 "http://www.xptek.cn/uploadfile/download/201707171401161883.pdf" H 2950 5200 50  0001 C CNN
	1    2750 5750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A23F37
P 5700 3600
F 0 "#PWR?" H 5700 3350 50  0001 C CNN
F 1 "GND" H 5705 3427 50  0000 C CNN
F 2 "" H 5700 3600 50  0001 C CNN
F 3 "" H 5700 3600 50  0001 C CNN
	1    5700 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3600 5700 2750
Wire Wire Line
	5700 2450 5900 2450
Wire Wire Line
	5900 2550 5700 2550
Connection ~ 5700 2550
Wire Wire Line
	5700 2550 5700 2450
Wire Wire Line
	5900 2650 5800 2650
Wire Wire Line
	5800 2650 5800 1900
Wire Wire Line
	5900 2750 5700 2750
Connection ~ 5700 2750
Wire Wire Line
	5700 2750 5700 2550
$Comp
L power:GND #PWR?
U 1 1 61A2920F
P 6850 3600
F 0 "#PWR?" H 6850 3350 50  0001 C CNN
F 1 "GND" H 6855 3427 50  0000 C CNN
F 2 "" H 6850 3600 50  0001 C CNN
F 3 "" H 6850 3600 50  0001 C CNN
	1    6850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 2850 6850 2850
Wire Wire Line
	6850 2850 6850 3600
$Comp
L power:+3.3V #PWR?
U 1 1 61A2A54F
P 6950 1900
F 0 "#PWR?" H 6950 1750 50  0001 C CNN
F 1 "+3.3V" H 6965 2073 50  0000 C CNN
F 2 "" H 6950 1900 50  0001 C CNN
F 3 "" H 6950 1900 50  0001 C CNN
	1    6950 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3050 6950 3050
Wire Wire Line
	6950 3050 6950 1900
Wire Wire Line
	6750 3150 6950 3150
Wire Wire Line
	6950 3150 6950 3050
Connection ~ 6950 3050
Text Label 7250 2350 2    50   ~ 0
T_XP
Text Label 7250 2450 2    50   ~ 0
T_YP
Text Label 7250 2550 2    50   ~ 0
T_XM
Text Label 7250 2650 2    50   ~ 0
T_YM
Wire Wire Line
	6750 2350 7250 2350
Wire Wire Line
	7250 2450 6750 2450
Wire Wire Line
	6750 2550 7250 2550
Wire Wire Line
	7250 2650 6750 2650
Text HLabel 7050 2950 2    50   Input ~ 0
LCD_CS
Text HLabel 3600 5750 2    50   Input ~ 0
LCD_CLK
Text HLabel 3600 5550 2    50   Input ~ 0
LCD_MOSI
Text HLabel 5600 2950 0    50   Input ~ 0
LCD_A0
Text HLabel 5600 2850 0    50   Input ~ 0
LCD_RST
Wire Wire Line
	5600 2850 5900 2850
Wire Wire Line
	5900 2950 5600 2950
Wire Wire Line
	5250 3050 5900 3050
Wire Wire Line
	5900 3150 5250 3150
Wire Wire Line
	6750 2950 7050 2950
$Comp
L power:GND #PWR?
U 1 1 61A377C1
P 1900 6550
F 0 "#PWR?" H 1900 6300 50  0001 C CNN
F 1 "GND" H 1905 6377 50  0000 C CNN
F 2 "" H 1900 6550 50  0001 C CNN
F 3 "" H 1900 6550 50  0001 C CNN
	1    1900 6550
	1    0    0    -1  
$EndComp
Text HLabel 3600 5650 2    50   Output ~ 0
LCD_MISO
Text HLabel 3600 5950 2    50   Input ~ 0
TOUCH_CS
Wire Wire Line
	3250 5950 3600 5950
Wire Wire Line
	3600 5750 3250 5750
Wire Wire Line
	3250 5650 3600 5650
Wire Wire Line
	3600 5550 3250 5550
Text Label 3400 5750 0    50   ~ 0
CLK
Text Label 3400 5550 0    50   ~ 0
DIN
Text Label 5250 3050 0    50   ~ 0
DIN
Text Label 5250 3150 0    50   ~ 0
CLK
Text Label 1750 5350 0    50   ~ 0
T_XP
Text Label 1750 5450 0    50   ~ 0
T_YP
Text Label 1750 5550 0    50   ~ 0
T_XM
Text Label 1750 5650 0    50   ~ 0
T_YM
Wire Wire Line
	1750 5350 2250 5350
Wire Wire Line
	2250 5450 1750 5450
Wire Wire Line
	1750 5550 2250 5550
Wire Wire Line
	2250 5650 1750 5650
Wire Wire Line
	1900 6550 1900 6050
Wire Wire Line
	1900 6050 2250 6050
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 61A4F77C
P 10250 2700
F 0 "J?" H 10330 2742 50  0000 L CNN
F 1 "Conn_01x03" H 10330 2651 50  0000 L CNN
F 2 "" H 10250 2700 50  0001 C CNN
F 3 "~" H 10250 2700 50  0001 C CNN
	1    10250 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A525A9
P 9900 3600
F 0 "#PWR?" H 9900 3350 50  0001 C CNN
F 1 "GND" H 9905 3427 50  0000 C CNN
F 2 "" H 9900 3600 50  0001 C CNN
F 3 "" H 9900 3600 50  0001 C CNN
	1    9900 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3600 9900 2800
Wire Wire Line
	9900 2800 10050 2800
Text HLabel 9800 2600 0    50   Input ~ 0
MOTOR_TX
Text HLabel 9800 2700 0    50   Output ~ 0
MOTOR_RX
Wire Wire Line
	9800 2600 10050 2600
Wire Wire Line
	10050 2700 9800 2700
NoConn ~ 3250 5850
$Comp
L power:+3.3V #PWR?
U 1 1 61A5A1C1
P 3350 4800
F 0 "#PWR?" H 3350 4650 50  0001 C CNN
F 1 "+3.3V" H 3365 4973 50  0000 C CNN
F 2 "" H 3350 4800 50  0001 C CNN
F 3 "" H 3350 4800 50  0001 C CNN
	1    3350 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4800 3350 5350
Wire Wire Line
	3350 5450 3250 5450
Wire Wire Line
	3250 5350 3350 5350
Connection ~ 3350 5350
Wire Wire Line
	3350 5350 3350 5450
Text HLabel 3600 6050 2    50   Output ~ 0
TOUCH_IT
Wire Wire Line
	3250 6050 3600 6050
NoConn ~ 2250 5950
NoConn ~ 2250 5850
NoConn ~ 2250 5750
$EndSCHEMATC
