EESchema Schematic File Version 2
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
LIBS:2
LIBS:mcp2562
LIBS:stm32f042k6tx
LIBS:Propulsion_control
LIBS:carte_moteurs-cache
EELAYER 25 0
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
$Comp
L +5V #PWR065
U 1 1 596AAF98
P 4650 2050
F 0 "#PWR065" H 4650 1900 50  0001 C CNN
F 1 "+5V" H 4650 2190 50  0000 C CNN
F 2 "" H 4650 2050 50  0001 C CNN
F 3 "" H 4650 2050 50  0001 C CNN
	1    4650 2050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR066
U 1 1 596AAFB0
P 4750 3150
F 0 "#PWR066" H 4750 3000 50  0001 C CNN
F 1 "+5V" H 4750 3290 50  0000 C CNN
F 2 "" H 4750 3150 50  0001 C CNN
F 3 "" H 4750 3150 50  0001 C CNN
	1    4750 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR067
U 1 1 596AB08B
P 4750 2300
F 0 "#PWR067" H 4750 2050 50  0001 C CNN
F 1 "GND" H 4750 2150 50  0000 C CNN
F 2 "" H 4750 2300 50  0001 C CNN
F 3 "" H 4750 2300 50  0001 C CNN
	1    4750 2300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR068
U 1 1 596AB0FD
P 4600 3400
F 0 "#PWR068" H 4600 3150 50  0001 C CNN
F 1 "GND" H 4600 3250 50  0000 C CNN
F 2 "" H 4600 3400 50  0001 C CNN
F 3 "" H 4600 3400 50  0001 C CNN
	1    4600 3400
	0    1    1    0   
$EndComp
Text HLabel 4750 2200 0    60   Input ~ 0
Ch_A1
Text HLabel 4750 2400 0    60   Input ~ 0
Ch_B1
Text HLabel 4600 3300 0    60   Input ~ 0
Ch_A2
Text HLabel 4600 3500 0    60   Input ~ 0
Ch_B2
Wire Wire Line
	4650 2050 4650 2100
Wire Wire Line
	4650 2100 5000 2100
Wire Wire Line
	4750 2200 5000 2200
Wire Wire Line
	4750 2300 5000 2300
Wire Wire Line
	4750 2400 5000 2400
Wire Wire Line
	4750 3150 4750 3200
Wire Wire Line
	4750 3200 4850 3200
Wire Wire Line
	4600 3300 4850 3300
Wire Wire Line
	4600 3400 4850 3400
Wire Wire Line
	4600 3500 4850 3500
$Comp
L CONN_01X04 P5
U 1 1 59C30399
P 5200 2250
F 0 "P5" H 5200 2500 50  0000 C CNN
F 1 "encoder_1" V 5300 2250 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 5200 2250 50  0001 C CNN
F 3 "" H 5200 2250 50  0000 C CNN
	1    5200 2250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P4
U 1 1 59C303CC
P 5050 3350
F 0 "P4" H 5050 3600 50  0000 C CNN
F 1 "encoder_2" V 5150 3350 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 5050 3350 50  0001 C CNN
F 3 "" H 5050 3350 50  0000 C CNN
	1    5050 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
