PORTB = $c000
PORTA = $c001
DDRB = $c002
DDRA = $c003

.segment "CODE"
.a8
.i8

reset:
  ; switch to native mode
  clc
  xce
  ; use 8-bit accumulator and index registers
  sep #%0011000

  ; Set all VIA pins to output
  lda #%11111111
  sta DDRA
  sta DDRB

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
  rti

irq:
  rti

.segment "VECTORS"
.word nmi
.word reset
.word irq

