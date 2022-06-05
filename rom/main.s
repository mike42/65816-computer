.import nmi, multitasking_test, uart_init, __test, uart_printz

.export __print

.macro pause len                   ; time-wasting loop as macro
    lda #len
:
    ldx #64
:
    ldy #255
:
    dey
    cpy #0
    bne :-
    dex
    cpx #0
    bne :--
    dec
    cmp #0
    bne :---
.endmacro

.macro blink
    sec                            ; switch to emulation mode
    xce
    pause 1
    clc                            ; switch to native mode
    xce
    pause 2
    sec                            ; switch to emulation mode
.endmacro

.segment "CODE"
reset:
    .a8
    .i8
    clc                            ; switch to native mode
    xce
    jmp post_check_loram

post_check_loram:
    ldx #%01010101                 ; Power-on self test (POST) - do we have low RAM?
    ldy #%10101010
    stx a:$00                      ; store known values at two addresses
    sty a:$01
    ldx a:$00                      ; read back the values - unlikely to be correct if RAM not present
    ldy a:$01
    cpx #%01010101
    bne post_fail_loram
    cpy #%10101010
    bne post_fail_loram
    jmp post_check_hiram

post_fail_loram:                   ; blink emulation mode output with two pulses
    pause 8
    blink
    blink
    jmp post_fail_loram            ; repeat indefinitely

post_check_hiram:
    ldx #%10101010                 ; Power-on self test (POST) - do we have high RAM?
    ldy #%01010101
    lda #$08                       ; data bank to high ram
    pha
    plb
    stx a:$00                      ; store known values at two addresses
    sty a:$01
    ldx a:$00                      ; read back the values - unlikely to be correct if RAM not present
    ldy a:$01
    cpx #%10101010
    bne post_fail_hiram
    cpy #%01010101
    bne post_fail_hiram
    lda #$00                       ; reset data bank to boot-up value
    pha
    plb
    jmp post_check_via

post_fail_hiram:                   ; blink emulation mode output with three pulses. we could use RAM here?
    pause 8
    blink
    blink
    blink
    jmp post_fail_hiram            ; repeat indefinitely

VIA_IER = $c00e
post_check_via:                    ; Power-on self test (POST) - do we have a 65C22 Versatile Interface Adapter (VIA)?
    lda a:VIA_IER
    cmp #%10000000                 ; start-up state, interrupts enabled overall (IFR7) but all interrupt sources (IFR0-6) disabled.
    bne post_fail_via
    jmp post_ok

post_fail_via:
    pause 8
    blink
    blink
    blink
    blink
    jmp post_fail_via

VIA_DDRB = $c002
VIA_T1C_L = $c004
VIA_T1C_H = $c005
VIA_ACR = $c00b
BEEP_FREQ_DIVIDER = 461            ; 1KHz, formula is CPU clock / (desired frequency * 2), or 921600 / (1000 * 2) ~= 461
post_ok:                           ; all good, emit celebratory beep, approx 1KHz for 1/10th second
    ; Start beep
    lda #%10000000                 ; VIA PIN PB7 only
    sta VIA_DDRB
    lda #%11000000                 ; set ACR. first two bits = 11 is continuous square wave output on PB7
    sta VIA_ACR
    lda #<BEEP_FREQ_DIVIDER        ; set T1 low-order counter
    sta VIA_T1C_L
    lda #>BEEP_FREQ_DIVIDER        ; set T1 high-order counter
    sta VIA_T1C_H
    ; wait approx 0.1 seconds
    pause 1
    ; Stop beep
    lda #%11000000                 ; set ACR. returns to a one-shot mode
    sta VIA_ACR
    stz VIA_T1C_L                  ; zero the counters
    stz VIA_T1C_H
    ; POST is now done
    jmp post_done

post_done:                         ; run some code
    ;jsr uart_test
    ;jmp multitasking_test

    .a16                            ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    jsr uart_init

    jsr __test
    stp

; print(const char *arg1)
__print:
	.a16
	.i16
	tsc
	sec
	sbc	#L2
	tcs
	phd
	tcd
arg1_0 := 3
	ldx	<L2+arg1_0
	jsr uart_printz
L4:
	lda	<L2+1
	sta	<L2+1+2
	pld
	tsc
	clc
	adc	#L2+2
	tcs
	rts
L2 := 0
L3 := 1

irq:
    rti

unused_interrupt:                 ; Probably make this into a crash.
    rti

.segment "VECTORS"
; native mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; COP
.word unused_interrupt            ; BRK
.word unused_interrupt            ; Abort
.word nmi                         ; NMI
.word unused_interrupt            ; Reserved
.word irq                         ; IRQ

; emulation mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; COP
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Abort
.word unused_interrupt            ; NMI
.word reset                       ; Reset
.word unused_interrupt            ; IRQ/BRK
