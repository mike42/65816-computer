PORTB = $c000
PORTA = $c001
DDRB = $c002
DDRA = $c003
T1C_L = $c004
T1C_H = $c005
ACR = $c00b
IFR = $c00b
IER = $c00e

.segment "CODE"

reset:
  ; switch to native mode
  clc
  xce

  ; use 16-bit accumulator and index registers
  .a16
  .i16
  rep #%00110000                 ; set up stack
  lda #$2000
  tcs

  ; use 8-bit accumulator and index registers
  .a8
  .i8
  sep #%00110000

  ; set all VIA pins to output
  lda #%11111111
  sta DDRA
  sta DDRB

  ; set up timer 1
  lda #%01000000        ; set ACR. first two bits = 01 is continuous interrupts for T1
  sta ACR
  lda #%11000000        ; enable VIA interrupt for T1
  sta IER

  ; set up a timer at ~65535 clock pulses.
  lda #$ff              ; set T1 low-order counter
  sta T1C_L
  lda #$ff              ; set T1 high-order counter
  sta T1C_H

;  jmp task_1_main
   jmp task_2_main

task_1_main:
@repeat_1:
  lda #%00000000
  sta PORTA
  lda #%00000011
  sta PORTA
  jmp @repeat_1

task_2_main:
  lda #%01010101
@repeat_2:
  ror
  sta PORTB
  jmp @repeat_2

nmi:
  ; TODO is figuring out how to save the state completely here
  .a8
  .i8
  sep #%00110000  ; save X only (assumes it is 8 bits..)
  phx
  ldx T1C_L       ; Clear the interrupt, side-effect of reading
  plx
  rti

irq:
  rti

unused_interrupt:  ; Probably make this into a crash.
  rti

.segment "VECTORS"
; native mode interrupt vectors
.word unused_interrupt ; Reserved
.word unused_interrupt ; Reserved
.word unused_interrupt ; COP
.word unused_interrupt ; BRK
.word unused_interrupt ; Abort
.word nmi              ; NMI
.word unused_interrupt ; Reserved
.word irq              ; IRQ

; emulation mode interrupt vectors
.word unused_interrupt ; Reserved
.word unused_interrupt ; Reserved
.word unused_interrupt ; COP
.word unused_interrupt ; Reserved
.word unused_interrupt ; Abort
.word unused_interrupt ; NMI
.word reset            ; Reset
.word unused_interrupt ; IRQ/BRK
