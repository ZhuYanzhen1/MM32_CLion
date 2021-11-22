EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "MM32_Board"
Date "2021-11-14"
Rev "1.0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 900  700  1300 1000
U 6191448D
F0 "Power" 50
F1 "1_Power.sch" 50
F2 "PWR_EN" I R 2200 1000 50 
$EndSheet
$Sheet
S 2550 700  1850 1600
U 6194FB75
F0 "MCU" 50
F1 "2_MCU.sch" 50
F2 "PWR_EN" I L 2550 1000 50 
F3 "SPI2_CS" O R 4400 1450 50 
F4 "SPI2_MISO" I R 4400 1350 50 
F5 "SPI2_MOSI" O R 4400 1250 50 
F6 "SPI2_CLK" O R 4400 1150 50 
$EndSheet
$Sheet
S 5150 700  1400 1600
U 61A493C9
F0 "IMU" 50
F1 "3_IMU.sch" 50
F2 "IMU_CS" I L 5150 1450 50 
F3 "IMU_RST" I L 5150 1600 50 
F4 "IMU_DR" O L 5150 1700 50 
F5 "IMU_CLK" I L 5150 1150 50 
F6 "IMU_MOSI" I L 5150 1250 50 
F7 "IMU_MISO" O L 5150 1350 50 
$EndSheet
Wire Wire Line
	4400 1150 5150 1150
Wire Wire Line
	4400 1250 5150 1250
Wire Wire Line
	5150 1350 4400 1350
Wire Wire Line
	4400 1450 5150 1450
$EndSCHEMATC
