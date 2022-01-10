EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3250 3400 1300 950 
U 61E490C8
F0 "CPU& Bank Address Latching" 50
F1 "cpu.sch" 50
F2 "CLK" O R 4550 3550 50 
F3 "~RES" O R 4550 3650 50 
F4 "A[0..19]" O R 4550 3950 50 
F5 "D[0..7]" T R 4550 4050 50 
F6 "~RW" O R 4550 3750 50 
$EndSheet
$Sheet
S 6850 3200 2200 1400
U 61EA29AB
F0 "Memory & I/O" 50
F1 "memory.sch" 50
F2 "CLK" I L 6850 3400 50 
F3 "~RW" I L 6850 3600 50 
F4 "A[0..19]" I L 6850 3800 50 
F5 "D[0..7]" B L 6850 3900 50 
F6 "~RES" I L 6850 3500 50 
$EndSheet
Wire Bus Line
	4550 4050 5750 4050
Wire Bus Line
	5750 4050 5750 3900
Wire Bus Line
	5750 3900 6850 3900
Wire Bus Line
	4550 3950 5650 3950
Wire Bus Line
	5650 3950 5650 3800
Wire Bus Line
	5650 3800 6850 3800
Wire Wire Line
	4550 3750 5550 3750
Wire Wire Line
	5550 3750 5550 3600
Wire Wire Line
	5550 3600 6850 3600
Wire Wire Line
	4550 3650 5450 3650
Wire Wire Line
	5450 3650 5450 3500
Wire Wire Line
	5450 3500 6850 3500
Wire Wire Line
	4550 3550 5350 3550
Wire Wire Line
	5350 3550 5350 3400
Wire Wire Line
	5350 3400 6850 3400
$EndSCHEMATC
