EESchema Schematic File Version 4
LIBS:miniflex-cache
EELAYER 26 0
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
L Ultrasonic:HC-SR04 U2
U 1 1 5BFF0DFD
P 6200 2850
F 0 "U2" H 6630 2846 50  0000 L CNN
F 1 "HC-SR04" H 6630 2755 50  0000 L CNN
F 2 "Ultrasonic:XCVR_HC-SR04" H 6200 2850 50  0001 L BNN
F 3 "OSEPP" H 6200 2850 50  0001 L BNN
F 4 "None" H 6200 2850 50  0001 L BNN "Field4"
F 5 "Ultrasonic Sensor Module" H 6200 2850 50  0001 L BNN "Field5"
F 6 "Unavailable" H 6200 2850 50  0001 L BNN "Field6"
F 7 "None" H 6200 2850 50  0001 L BNN "Field7"
F 8 "HC-SR04" H 6200 2850 50  0001 L BNN "Field8"
	1    6200 2850
	1    0    0    -1  
$EndComp
$Comp
L Analog_DAC:MCP4901 U1
U 1 1 5BFF14A5
P 4700 3950
F 0 "U1" H 5441 3996 50  0000 L CNN
F 1 "MCP4901" H 5441 3905 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 5700 3850 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/22248a.pdf" H 5700 3850 50  0001 C CNN
	1    4700 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2750 4700 2750
Wire Wire Line
	4700 2750 4700 3250
Connection ~ 4700 2750
Wire Wire Line
	4700 2750 5700 2750
Wire Wire Line
	4900 3550 4900 3250
Wire Wire Line
	4900 3250 4700 3250
Connection ~ 4700 3250
Wire Wire Line
	4700 3250 4700 3550
Wire Wire Line
	3550 2850 5600 2850
Wire Wire Line
	5600 2850 5600 3050
Wire Wire Line
	5600 3050 5950 3050
Wire Wire Line
	5750 2950 5750 2850
Wire Wire Line
	5750 2850 6000 2850
Wire Wire Line
	5850 3150 5850 2950
Wire Wire Line
	5850 2950 6000 2950
Connection ~ 5950 3050
Wire Wire Line
	5950 3050 6000 3050
Wire Wire Line
	4100 4350 4100 3850
Wire Wire Line
	4100 3850 4300 3850
Wire Wire Line
	4100 4350 4700 4350
Connection ~ 4700 4350
Wire Wire Line
	4700 4350 5950 4350
Wire Wire Line
	3550 3150 3950 3150
Wire Wire Line
	3950 3150 3950 3950
Wire Wire Line
	3950 3950 4300 3950
Wire Wire Line
	3550 3250 3900 3250
Wire Wire Line
	3900 3250 3900 4050
Wire Wire Line
	3900 4050 4300 4050
Wire Wire Line
	3550 3350 3850 3350
Wire Wire Line
	3850 3350 3850 4150
Wire Wire Line
	3850 4150 4300 4150
$Comp
L Connector:Conn_01x07_Female J1
U 1 1 5BFF413F
P 3350 3050
F 0 "J1" H 3244 2525 50  0000 C CNN
F 1 "Conn_01x07_Female" H 3244 2616 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 3350 3050 50  0001 C CNN
F 3 "~" H 3350 3050 50  0001 C CNN
	1    3350 3050
	-1   0    0    1   
$EndComp
Wire Wire Line
	3550 3050 5450 3050
Wire Wire Line
	5450 3050 5450 2950
Wire Wire Line
	5450 2950 5750 2950
Wire Wire Line
	3550 2950 5350 2950
Wire Wire Line
	5350 2950 5350 3150
Wire Wire Line
	5350 3150 5850 3150
$Comp
L Amplifier_Operational:TS912 U3
U 1 1 5C120150
P 7350 4050
F 0 "U3" H 7350 4417 50  0000 C CNN
F 1 "TS912" H 7350 4326 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 7350 4050 50  0001 C CNN
F 3 "www.st.com/resource/en/datasheet/ts912.pdf" H 7350 4050 50  0001 C CNN
	1    7350 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4150 7050 4400
Wire Wire Line
	7050 4400 7650 4400
Wire Wire Line
	7650 4400 7650 4050
Wire Wire Line
	5950 3050 5950 3300
$Comp
L Amplifier_Operational:TS912 U3
U 2 1 5C121955
P 7350 4750
F 0 "U3" H 7350 5117 50  0000 C CNN
F 1 "TS912" H 7350 5026 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 7350 4750 50  0001 C CNN
F 3 "www.st.com/resource/en/datasheet/ts912.pdf" H 7350 4750 50  0001 C CNN
	2    7350 4750
	1    0    0    -1  
$EndComp
NoConn ~ 7050 4650
NoConn ~ 7050 4850
NoConn ~ 7650 4750
$Comp
L Amplifier_Operational:TS912 U3
U 3 1 5C122CAD
P 7800 2650
F 0 "U3" H 7758 2696 50  0000 L CNN
F 1 "TS912" H 7758 2605 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 7800 2650 50  0001 C CNN
F 3 "www.st.com/resource/en/datasheet/ts912.pdf" H 7800 2650 50  0001 C CNN
	3    7800 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2750 5700 2350
Wire Wire Line
	5700 2350 7700 2350
Connection ~ 5700 2750
Wire Wire Line
	5700 2750 6000 2750
Wire Wire Line
	5950 3300 7700 3300
Wire Wire Line
	7700 3300 7700 2950
Connection ~ 5950 3300
Wire Wire Line
	5950 3300 5950 4350
Wire Wire Line
	5400 3950 7050 3950
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5C12464D
P 8250 3950
F 0 "J2" H 8277 3926 50  0000 L CNN
F 1 "Conn_01x02_Female" H 8277 3835 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 8250 3950 50  0001 C CNN
F 3 "~" H 8250 3950 50  0001 C CNN
	1    8250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3300 8050 3300
Wire Wire Line
	8050 3300 8050 3950
Connection ~ 7700 3300
Wire Wire Line
	8050 4050 7650 4050
Connection ~ 7650 4050
$EndSCHEMATC
