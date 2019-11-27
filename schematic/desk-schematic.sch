EESchema Schematic File Version 4
EELAYER 30 0
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
L MCU_Module:WeMos_D1_mini U1
U 1 1 5DDED4CC
P 1900 2950
F 0 "U1" H 1250 2900 50  0000 C CNN
F 1 "WeMos_D1_mini" H 1250 2800 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 1900 1800 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 50  1800 50  0001 C CNN
	1    1900 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:LED StatusLED1
U 1 1 5DDEEA55
P 6950 2700
F 0 "StatusLED1" V 6989 2582 50  0000 R CNN
F 1 "LED" V 6898 2582 50  0000 R CNN
F 2 "" H 6950 2700 50  0001 C CNN
F 3 "~" H 6950 2700 50  0001 C CNN
	1    6950 2700
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED PowerLED1
U 1 1 5DDEED72
P 6200 2650
F 0 "PowerLED1" V 6239 2532 50  0000 R CNN
F 1 "LED" V 6148 2532 50  0000 R CNN
F 2 "" H 6200 2650 50  0001 C CNN
F 3 "~" H 6200 2650 50  0001 C CNN
	1    6200 2650
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 5DDEF22E
P 1800 1850
F 0 "SW1" V 1754 1980 50  0000 L CNN
F 1 "SW_DIP_x01" V 1845 1980 50  0000 L CNN
F 2 "" H 1800 1850 50  0001 C CNN
F 3 "~" H 1800 1850 50  0001 C CNN
	1    1800 1850
	0    1    1    0   
$EndComp
$Comp
L Connector:DIN-7 DeskConnector1
U 1 1 5DDF3164
P 3500 3400
F 0 "DeskConnector1" H 3500 3125 50  0000 C CNN
F 1 "DIN-7" H 3500 3034 50  0000 C CNN
F 2 "" H 3500 3400 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 3500 3400 50  0001 C CNN
	1    3500 3400
	1    0    0    -1  
$EndComp
Text Label 3200 3400 2    50   ~ 0
RxD
Text Label 3500 3100 0    50   ~ 0
HS3
Text Label 3800 3400 0    50   ~ 0
HS1
Text Label 3200 3300 2    50   ~ 0
HS4
Text Label 3800 3300 0    50   ~ 0
HS2
Text Label 3200 3500 2    50   ~ 0
TxD
$Comp
L power:+5V #PWR0101
U 1 1 5DDF3170
P 4000 3500
F 0 "#PWR0101" H 4000 3350 50  0001 C CNN
F 1 "+5V" H 4015 3673 50  0000 C CNN
F 2 "" H 4000 3500 50  0001 C CNN
F 3 "" H 4000 3500 50  0001 C CNN
	1    4000 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3500 3800 3500
$Comp
L Connector:DIN-7 SwitchConnector1
U 1 1 5DDF3691
P 4650 3400
F 0 "SwitchConnector1" H 4650 3125 50  0000 C CNN
F 1 "DIN-7" H 4650 3034 50  0000 C CNN
F 2 "" H 4650 3400 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 4650 3400 50  0001 C CNN
	1    4650 3400
	1    0    0    -1  
$EndComp
Text Label 4350 3400 2    50   ~ 0
RxD
Text Label 4650 3100 0    50   ~ 0
HS3
Text Label 4950 3400 0    50   ~ 0
HS1
Text Label 4350 3300 2    50   ~ 0
HS4
Text Label 4950 3300 0    50   ~ 0
HS2
Text Label 4350 3500 2    50   ~ 0
TxD
$Comp
L power:+5V #PWR0102
U 1 1 5DDF369D
P 5150 3500
F 0 "#PWR0102" H 5150 3350 50  0001 C CNN
F 1 "+5V" H 5165 3673 50  0000 C CNN
F 2 "" H 5150 3500 50  0001 C CNN
F 3 "" H 5150 3500 50  0001 C CNN
	1    5150 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3500 4950 3500
Text Notes 3750 3000 0    50   ~ 0
DIN shell is ground
$Comp
L power:+5V #PWR0103
U 1 1 5DDF69B7
P 1800 1550
F 0 "#PWR0103" H 1800 1400 50  0001 C CNN
F 1 "+5V" H 1815 1723 50  0000 C CNN
F 2 "" H 1800 1550 50  0001 C CNN
F 3 "" H 1800 1550 50  0001 C CNN
	1    1800 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5DDF704B
P 1900 3750
F 0 "#PWR0104" H 1900 3500 50  0001 C CNN
F 1 "GND" H 1905 3577 50  0000 C CNN
F 2 "" H 1900 3750 50  0001 C CNN
F 3 "" H 1900 3750 50  0001 C CNN
	1    1900 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5DDF75FD
P 6200 2800
F 0 "#PWR0105" H 6200 2550 50  0001 C CNN
F 1 "GND" H 6205 2627 50  0000 C CNN
F 2 "" H 6200 2800 50  0001 C CNN
F 3 "" H 6200 2800 50  0001 C CNN
	1    6200 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5DDF96AB
P 6950 2850
F 0 "#PWR0106" H 6950 2600 50  0001 C CNN
F 1 "GND" H 6955 2677 50  0000 C CNN
F 2 "" H 6950 2850 50  0001 C CNN
F 3 "" H 6950 2850 50  0001 C CNN
	1    6950 2850
	1    0    0    -1  
$EndComp
Text Label 6200 2500 0    50   ~ 0
POWER_LED
Text Label 2300 2950 0    50   ~ 0
POWER_LED
Text Label 6950 2550 0    50   ~ 0
STATUS_LED
Text Label 2300 2750 0    50   ~ 0
STATUS_LED
Text Label 2300 3250 0    50   ~ 0
TxD
Text Label 2300 3150 0    50   ~ 0
HS1
Text Label 2300 3050 0    50   ~ 0
HS2
$EndSCHEMATC
