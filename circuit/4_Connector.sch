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
UART_TX
Text HLabel 3650 2450 2    50   Input ~ 0
UART_RX
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
P 4750 2500
F 0 "J?" H 4700 2600 50  0000 L CNN
F 1 "VOUT" H 4700 2300 50  0000 L CNN
F 2 "" H 4750 2500 50  0001 C CNN
F 3 "~" H 4750 2500 50  0001 C CNN
	1    4750 2500
	1    0    0    -1  
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
	4550 1700 4550 2500
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
	4550 2600 4550 3400
$EndSCHEMATC
