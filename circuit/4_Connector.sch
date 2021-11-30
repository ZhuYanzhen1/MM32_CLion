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
P 10050 2750
F 0 "J?" H 10000 2550 50  0000 L CNN
F 1 "VOUT" H 10000 2850 50  0000 L CNN
F 2 "" H 10050 2750 50  0001 C CNN
F 3 "~" H 10050 2750 50  0001 C CNN
	1    10050 2750
	1    0    0    1   
$EndComp
$Comp
L power:+15V #PWR?
U 1 1 61B26E64
P 9850 1900
F 0 "#PWR?" H 9850 1750 50  0001 C CNN
F 1 "+15V" H 9865 2073 50  0000 C CNN
F 2 "" H 9850 1900 50  0001 C CNN
F 3 "" H 9850 1900 50  0001 C CNN
	1    9850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1900 9850 2650
$Comp
L power:GND #PWR?
U 1 1 61B26FE3
P 9850 3600
F 0 "#PWR?" H 9850 3350 50  0001 C CNN
F 1 "GND" H 9855 3427 50  0000 C CNN
F 2 "" H 9850 3600 50  0001 C CNN
F 3 "" H 9850 3600 50  0001 C CNN
	1    9850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2750 9850 3600
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
P 6300 5650
F 0 "U?" H 5950 6200 50  0000 C CNN
F 1 "XPT2046TS" H 6100 5200 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 6300 5050 50  0001 C CIN
F 3 "http://www.xptek.cn/uploadfile/download/201707171401161883.pdf" H 6500 5100 50  0001 C CNN
	1    6300 5650
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
Text HLabel 7150 5650 2    50   Input ~ 0
LCD_CLK
Text HLabel 7150 5450 2    50   Input ~ 0
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
P 5450 6450
F 0 "#PWR?" H 5450 6200 50  0001 C CNN
F 1 "GND" H 5455 6277 50  0000 C CNN
F 2 "" H 5450 6450 50  0001 C CNN
F 3 "" H 5450 6450 50  0001 C CNN
	1    5450 6450
	1    0    0    -1  
$EndComp
Text HLabel 7150 5550 2    50   Output ~ 0
LCD_MISO
Text HLabel 7150 5850 2    50   Input ~ 0
TOUCH_CS
Wire Wire Line
	6800 5850 7150 5850
Wire Wire Line
	7150 5650 6800 5650
Wire Wire Line
	6800 5550 7150 5550
Wire Wire Line
	7150 5450 6800 5450
Text Label 6950 5650 0    50   ~ 0
CLK
Text Label 6950 5450 0    50   ~ 0
DIN
Text Label 5250 3050 0    50   ~ 0
DIN
Text Label 5250 3150 0    50   ~ 0
CLK
Text Label 5300 5250 0    50   ~ 0
T_XP
Text Label 5300 5350 0    50   ~ 0
T_YP
Text Label 5300 5450 0    50   ~ 0
T_XM
Text Label 5300 5550 0    50   ~ 0
T_YM
Wire Wire Line
	5300 5250 5800 5250
Wire Wire Line
	5800 5350 5300 5350
Wire Wire Line
	5300 5450 5800 5450
Wire Wire Line
	5800 5550 5300 5550
Wire Wire Line
	5450 6450 5450 5950
Wire Wire Line
	5450 5950 5800 5950
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 61A4F77C
P 9200 2700
F 0 "J?" H 9150 2900 50  0000 L CNN
F 1 "Conn_01x03" H 9050 2500 50  0000 L CNN
F 2 "" H 9200 2700 50  0001 C CNN
F 3 "~" H 9200 2700 50  0001 C CNN
	1    9200 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A525A9
P 8850 3600
F 0 "#PWR?" H 8850 3350 50  0001 C CNN
F 1 "GND" H 8855 3427 50  0000 C CNN
F 2 "" H 8850 3600 50  0001 C CNN
F 3 "" H 8850 3600 50  0001 C CNN
	1    8850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 3600 8850 2800
Wire Wire Line
	8850 2800 9000 2800
Text HLabel 8750 2600 0    50   Input ~ 0
MOTOR_TX
Text HLabel 8750 2700 0    50   Output ~ 0
MOTOR_RX
Wire Wire Line
	8750 2600 9000 2600
Wire Wire Line
	9000 2700 8750 2700
NoConn ~ 6800 5750
$Comp
L power:+3.3V #PWR?
U 1 1 61A5A1C1
P 6900 4700
F 0 "#PWR?" H 6900 4550 50  0001 C CNN
F 1 "+3.3V" H 6915 4873 50  0000 C CNN
F 2 "" H 6900 4700 50  0001 C CNN
F 3 "" H 6900 4700 50  0001 C CNN
	1    6900 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4700 6900 5250
Wire Wire Line
	6900 5350 6800 5350
Wire Wire Line
	6800 5250 6900 5250
Connection ~ 6900 5250
Wire Wire Line
	6900 5250 6900 5350
Text HLabel 7150 5950 2    50   Output ~ 0
TOUCH_IT
Wire Wire Line
	6800 5950 7150 5950
NoConn ~ 5800 5850
NoConn ~ 5800 5750
NoConn ~ 5800 5650
$Comp
L MM32_Periph:ATGM336H U?
U 1 1 61A635E4
P 2350 6100
F 0 "U?" H 2500 7150 50  0000 C CNN
F 1 "ATGM336H" H 2650 6050 50  0000 C CNN
F 2 "RF_GPS:ublox_MAX" H 2700 5950 50  0001 C CNN
F 3 "" H 2500 7150 50  0001 C CNN
	1    2350 6100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A6539B
P 2250 6450
F 0 "#PWR?" H 2250 6200 50  0001 C CNN
F 1 "GND" H 2255 6277 50  0000 C CNN
F 2 "" H 2250 6450 50  0001 C CNN
F 3 "" H 2250 6450 50  0001 C CNN
	1    2250 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 6450 2250 5200
Wire Wire Line
	2250 5200 2350 5200
$Comp
L power:GND #PWR?
U 1 1 61A66EEE
P 3300 6450
F 0 "#PWR?" H 3300 6200 50  0001 C CNN
F 1 "GND" H 3305 6277 50  0000 C CNN
F 2 "" H 3300 6450 50  0001 C CNN
F 3 "" H 3300 6450 50  0001 C CNN
	1    3300 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 6000 3300 6000
Wire Wire Line
	3300 6000 3300 6450
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 61A69CDB
P 4100 5900
F 0 "J?" H 4180 5892 50  0000 L CNN
F 1 "Conn_01x02" H 4180 5801 50  0000 L CNN
F 2 "" H 4100 5900 50  0001 C CNN
F 3 "~" H 4100 5900 50  0001 C CNN
	1    4100 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 5800 3300 5800
Wire Wire Line
	3300 5800 3300 6000
Connection ~ 3300 6000
Wire Wire Line
	3200 5900 3550 5900
$Comp
L Device:L L?
U 1 1 61A70071
P 3550 5750
F 0 "L?" H 3603 5796 50  0000 L CNN
F 1 "47nH" H 3603 5705 50  0000 L CNN
F 2 "" H 3550 5750 50  0001 C CNN
F 3 "~" H 3550 5750 50  0001 C CNN
	1    3550 5750
	1    0    0    -1  
$EndComp
Connection ~ 3550 5900
Wire Wire Line
	3550 5900 3900 5900
Wire Wire Line
	3200 5600 3550 5600
$Comp
L power:GND #PWR?
U 1 1 61A7288A
P 3900 6450
F 0 "#PWR?" H 3900 6200 50  0001 C CNN
F 1 "GND" H 3905 6277 50  0000 C CNN
F 2 "" H 3900 6450 50  0001 C CNN
F 3 "" H 3900 6450 50  0001 C CNN
	1    3900 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 6000 3900 6450
NoConn ~ 3200 5300
NoConn ~ 3200 5400
$Comp
L power:+3.3V #PWR?
U 1 1 61A77A72
P 2000 4700
F 0 "#PWR?" H 2000 4550 50  0001 C CNN
F 1 "+3.3V" H 2015 4873 50  0000 C CNN
F 2 "" H 2000 4700 50  0001 C CNN
F 3 "" H 2000 4700 50  0001 C CNN
	1    2000 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 5900 2000 5900
$Comp
L Device:C C?
U 1 1 61A7CD85
P 2000 6200
F 0 "C?" H 2115 6246 50  0000 L CNN
F 1 "C" H 2115 6155 50  0000 L CNN
F 2 "" H 2038 6050 50  0001 C CNN
F 3 "~" H 2000 6200 50  0001 C CNN
	1    2000 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A7DB72
P 2000 6450
F 0 "#PWR?" H 2000 6200 50  0001 C CNN
F 1 "GND" H 2005 6277 50  0000 C CNN
F 2 "" H 2000 6450 50  0001 C CNN
F 3 "" H 2000 6450 50  0001 C CNN
	1    2000 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6050 2000 5900
Connection ~ 2000 5900
Wire Wire Line
	2000 6350 2000 6450
$Comp
L Device:Battery_Cell BT?
U 1 1 61A84728
P 1600 6150
F 0 "BT?" H 1718 6200 50  0000 L CNN
F 1 "Battery_Cell" H 1718 6155 50  0001 L CNN
F 2 "Battery:BatteryHolder_Keystone_1060_1x2032" V 1600 6210 50  0001 C CNN
F 3 "~" V 1600 6210 50  0001 C CNN
	1    1600 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4700 2000 5900
Wire Wire Line
	1600 5950 1600 5700
Wire Wire Line
	1600 5700 2350 5700
$Comp
L power:GND #PWR?
U 1 1 61A8DC9F
P 1600 6450
F 0 "#PWR?" H 1600 6200 50  0001 C CNN
F 1 "GND" H 1605 6277 50  0000 C CNN
F 2 "" H 1600 6450 50  0001 C CNN
F 3 "" H 1600 6450 50  0001 C CNN
	1    1600 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6450 1600 6250
NoConn ~ 2350 6000
Text HLabel 1950 5400 0    50   Input ~ 0
GPS_RXD
Text HLabel 1950 5300 0    50   Output ~ 0
GPS_TXD
Wire Wire Line
	1950 5300 2350 5300
Wire Wire Line
	2350 5400 1950 5400
$EndSCHEMATC
