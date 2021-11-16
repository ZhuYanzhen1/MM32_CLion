EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "MM32_Board"
Date "2021-11-14"
Rev "1.0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MM32_Periph:MM32F3277G9P U?
U 1 1 6192F179
P 2350 6600
F 0 "U?" H 2350 10600 50  0000 L CNN
F 1 "MM32F3277G9P" H 6250 6550 50  0000 L CNN
F 2 "Package_QFP:LQFP-144_20x20mm_P0.5mm" H 7150 6400 50  0001 C CNN
F 3 "" H 8650 7850 50  0001 C CNN
	1    2350 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_GND24 Y?
U 1 1 619018BC
P 8300 2000
F 0 "Y?" H 8494 2046 50  0000 L CNN
F 1 "8MHz" H 8494 1955 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 8300 2000 50  0001 C CNN
F 3 "~" H 8300 2000 50  0001 C CNN
	1    8300 2000
	1    0    0    -1  
$EndComp
Text Label 1700 5100 0    50   ~ 0
XTAL_IN
Text Label 1700 5200 0    50   ~ 0
XTAL_OUT
Wire Wire Line
	2250 5100 1700 5100
Wire Wire Line
	1700 5200 2250 5200
$Sheet
S 900  700  1300 1000
U 6191448D
F0 "Power" 50
F1 "1_Power.sch" 50
$EndSheet
$EndSCHEMATC
