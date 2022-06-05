.import __print
.export __test

R0 := 1
R1 := 5
R2 := 9
R3 := 13
.segment "CODE"
__test:
	.a16
	.i16
	tsc
	sec
	sbc	#L2
	tcs
	phd
	tcd
	pea	L1
	jsr	__print
L4:
	pld
	tsc
	clc
	adc	#L2
	tcs
	rts
L2 := 0
L3 := 1
L1:
	.byte $48,$65,$6C,$6C,$6F,$20,$77,$6F,$72,$6C,$64,$0A,$00
