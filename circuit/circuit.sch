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
L Connector_Generic:Conn_02x40_Odd_Even Rockpro64
U 1 1 60AB8B17
P 7600 950
F 0 "Rockpro64" V 7604 -1137 50  0000 R CNN
F 1 "P12Bus" V 7695 -1137 50  0000 R CNN
F 2 "" H 7600 950 50  0001 C CNN
F 3 "~" H 7600 950 50  0001 C CNN
	1    7600 950 
	0    1    1    0   
$EndComp
Text Label 9200 1400 0    50   ~ 0
UART_Tx
$Comp
L Connector_Generic:Conn_02x10_Counter_Clockwise DL160
U 1 1 60ACAB61
P 5000 2150
F 0 "DL160" H 5050 2767 50  0000 C CNN
F 1 "Front Panel" H 5050 2676 50  0000 C CNN
F 2 "" H 5000 2150 50  0001 C CNN
F 3 "~" H 5000 2150 50  0001 C CNN
	1    5000 2150
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_Every Arduino
U 1 1 60AECA9E
P 8750 3300
F 0 "Arduino" H 8750 2211 50  0000 C CNN
F 1 "Nano" H 8750 2120 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 8750 3300 50  0001 C CIN
F 3 "https://content.arduino.cc/assets/NANOEveryV3.0_sch.pdf" H 8750 3300 50  0001 C CNN
	1    8750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2600 7650 2600
Wire Wire Line
	9200 1750 9200 1250
Wire Wire Line
	7650 2600 7650 1750
Wire Wire Line
	7650 1750 9200 1750
Text GLabel 7300 2800 0    50   Input ~ 0
ATX_PS_ON
Text GLabel 9150 2050 2    50   Input ~ 0
ATX_5VSB
Wire Wire Line
	7300 2800 8250 2800
Wire Wire Line
	8250 2900 6350 2900
Wire Wire Line
	6350 2900 6350 2050
Wire Wire Line
	6350 2050 5300 2050
Text Label 5400 2050 0    50   ~ 0
Panel_Pwr
Wire Wire Line
	8250 3000 6400 3000
Wire Wire Line
	6400 3000 6400 1750
Wire Wire Line
	6400 1750 5300 1750
Text Label 5400 1750 0    50   ~ 0
Sys_LED_+
Wire Wire Line
	8250 3100 4700 3100
Wire Wire Line
	4700 3100 4700 1950
Wire Wire Line
	4700 1950 4800 1950
Text Label 4350 1950 0    50   ~ 0
NIC_LED_+
Wire Wire Line
	8250 3200 4650 3200
Text Label 4250 2350 0    50   ~ 0
HDD_LED_+
Wire Wire Line
	8250 3300 7600 3300
Wire Wire Line
	7600 3300 7600 1700
Wire Wire Line
	7600 1700 9750 1700
Wire Wire Line
	9750 1700 9750 750 
Wire Wire Line
	9750 750  9500 750 
Text Label 9500 750  0    50   ~ 0
Board_Pwr
$Comp
L Device:Q_NMOS_DGS Q1
U 1 1 60B41EDD
P 6650 3900
F 0 "Q1" V 6992 3900 50  0000 C CNN
F 1 "2N7000" V 6901 3900 50  0000 C CNN
F 2 "" H 6850 4000 50  0001 C CNN
F 3 "~" H 6650 3900 50  0001 C CNN
	1    6650 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8250 3500 7350 3500
Wire Wire Line
	7350 3500 7350 3800
Wire Wire Line
	7350 3800 6850 3800
Text GLabel 6500 4400 0    50   Input ~ 0
ATX_3.3V
Wire Wire Line
	6650 4400 6650 4100
$Comp
L Device:R_US R1
U 1 1 60B5DF8E
P 5950 3800
F 0 "R1" V 5745 3800 50  0000 C CNN
F 1 "50K" V 5836 3800 50  0000 C CNN
F 2 "" V 5990 3790 50  0001 C CNN
F 3 "~" H 5950 3800 50  0001 C CNN
	1    5950 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 3800 6450 3800
Text Label 7900 3500 0    50   ~ 0
PWM
$Comp
L Connector_Generic:Conn_01x04 PWM
U 1 1 60B6B8D9
P 5700 4700
F 0 "PWM" V 5572 4880 50  0000 L CNN
F 1 "FAN" V 5663 4880 50  0000 L CNN
F 2 "" H 5700 4700 50  0001 C CNN
F 3 "~" H 5700 4700 50  0001 C CNN
	1    5700 4700
	0    1    1    0   
$EndComp
$Comp
L power:Earth #PWR?
U 1 1 60B6D2BE
P 5100 5350
F 0 "#PWR?" H 5100 5100 50  0001 C CNN
F 1 "Earth" H 5100 5200 50  0001 C CNN
F 2 "" H 5100 5350 50  0001 C CNN
F 3 "~" H 5100 5350 50  0001 C CNN
	1    5100 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4500 5600 4300
Wire Wire Line
	5350 3800 5800 3800
Text GLabel 5350 3800 0    50   Input ~ 0
ATX_5V
Wire Wire Line
	5800 4500 5800 3800
Connection ~ 5800 3800
Text GLabel 5350 4000 0    50   Input ~ 0
ATX_12V
Wire Wire Line
	5350 4000 5700 4000
Wire Wire Line
	5700 4000 5700 4500
Wire Wire Line
	6500 4400 6650 4400
Wire Wire Line
	5100 4300 5600 4300
Wire Wire Line
	8750 5250 7700 5250
Connection ~ 5100 5250
Wire Wire Line
	5100 5250 5100 5350
Wire Wire Line
	8250 3900 7550 3900
Wire Wire Line
	7550 3900 7550 3300
Wire Wire Line
	7550 3300 6300 3300
Wire Wire Line
	6300 3300 6300 2650
Wire Wire Line
	6300 2650 5300 2650
Text Label 5400 2650 0    50   ~ 0
PWR_G+_R-
Wire Wire Line
	9150 2050 8650 2050
Wire Wire Line
	8650 2050 8650 2300
Wire Wire Line
	9250 4000 9250 4850
Wire Wire Line
	4600 2650 4800 2650
Wire Wire Line
	8750 4300 8750 5250
Wire Wire Line
	5100 4300 5100 5250
Wire Wire Line
	8250 4000 7500 4000
Wire Wire Line
	7500 4000 7500 3350
Wire Wire Line
	7500 3350 4600 3350
Wire Wire Line
	4600 3350 4600 2650
Wire Wire Line
	4500 4850 4500 1750
Wire Wire Line
	4500 1750 4800 1750
Wire Wire Line
	4500 4850 9250 4850
Text Label 4150 1750 0    50   ~ 0
Sys_LED_+
Wire Wire Line
	4800 2350 4650 2350
Wire Wire Line
	4650 2350 4650 3200
Text Label 4200 2650 0    50   ~ 0
PWR_G-_R+
Wire Wire Line
	4800 2450 4450 2450
Wire Wire Line
	4450 2450 4450 4900
Wire Wire Line
	4800 2550 4400 2550
Wire Wire Line
	4400 2550 4400 4950
Wire Wire Line
	4450 4900 9350 4900
Wire Wire Line
	9350 4900 9350 3900
Wire Wire Line
	9350 3900 9250 3900
Wire Wire Line
	4400 4950 9450 4950
Wire Wire Line
	9450 4950 9450 3800
Wire Wire Line
	9450 3800 9250 3800
Text Label 4200 2450 0    50   ~ 0
Sys_R-
Text Label 4150 2550 0    50   ~ 0
Sys_G-
Wire Wire Line
	5300 1950 7700 1950
Wire Wire Line
	7700 1950 7700 5250
Connection ~ 7700 5250
Wire Wire Line
	7700 5250 5100 5250
$EndSCHEMATC
