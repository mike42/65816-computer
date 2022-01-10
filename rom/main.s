PORTB = $c000
PORTA = $c001
DDRB = $c002
DDRA = $c003

SOME_ADDRESS_IN_RAM = $beef

E = %10000000
RW = %01000000
RS = %00100000

.segment "CODE"

; Write 42 to VIA port B
reset:
  lda #%11111111 ; Set all pins to output
  sta DDRB

  lda #42
  sta PORTB

  lda #00
  sta SOME_ADDRESS_IN_RAM

loop:
  inc SOME_ADDRESS_IN_RAM
  lda SOME_ADDRESS_IN_RAM
  sta PORTB
  jmp loop

nmi:
  rti

irq:
  rti

.segment "VECTORS"
.word nmi
.word reset
.word irq

