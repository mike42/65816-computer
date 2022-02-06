PORTB = $c000
PORTA = $c001
DDRB = $c002
DDRA = $c003
T1C_L = $c004
T1C_H = $c005
ACR = $c00b
IFR = $c00b
IER = $c00e

.segment "BSS"
next_task_sp: .res 2       ; Stack pointer of other task
temp: .res 2

.segment "CODE"
reset:
  clc                             ; switch to native mode
  xce

  ; Save context as if we are at task_2_main, so we can switch to it later.
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%0011000
  lda #$3000                      ; set up stack, direct page at $3000
  tcs
  tcd
  ; emulate what is pushed to the stack before NMI is called: progam bank, program counter, processor status register
  ; TODO suspect that there is a problem here..
  phk                             ; program bank register, same as current code, will be 0 here.
  .a8                             ; use 8-bit accumulator and index registers
  .i8
  sep #%0011000
  lda #>task_2_main
  pha
  lda #<task_2_main
  pha
  ;  pea task_2_main                 ; 16 bit program counter for the start of this task
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%0011000
  php                             ; processor status register
  ; match what we push to the stack in the nmi routine
  lda #0
  tax
  tay
  pha                             ; Push A, X, Y
  phx
  phy
  phb                             ; Push data bank, direct register
  phd
  tsc                             ; save stack pointer to next_task_sp
  sta next_task_sp

  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%00110000
  lda #$2000                      ; set up stack, direct page at $2000 for task_2_main
  tcs
  tcd

  ; Set up the interrupt timer
  .a8                             ; use 8-bit accumulator and index registers
  .i8
  sep #%00110000
  lda #%11111111                  ; set all VIA pins to output
  sta DDRA
  sta DDRB
  ; set up timer 1
  lda #%01000000                 ; set ACR. first two bits = 01 is continuous interrupts for T1
  sta ACR
  lda #%11000000                ; enable VIA interrupt for T1
  sta IER
  ; set up a timer at ~65535 clock pulses.
  lda #$ff                      ; set T1 low-order counter
  sta T1C_L
  lda #$ff                      ; set T1 high-order counter
  sta T1C_H

  ; start running task 1
  jmp task_1_main
; jmp task_2_main

; Task 1
task_1_main:
  .a8                             ; use 8-bit accumulator and index registers
  .i8
  sep #%00110000
@repeat_1:
  lda #%00000000                  ; alternate between two values
  sta PORTA
  lda #%00000011
  sta PORTA
  jmp @repeat_1                   ; repeat forever

; Task 2
task_2_main:
  .a8                             ; use 8-bit accumulator and index registers
  .i8
  sep #%00110000
  lda #%01010101                  ; grab a start value
@repeat_2:
  ror                             ; rotate right
  sta PORTB
  jmp @repeat_2                   ; repeat forever

nmi:
  .a16
  .i16
  rep #%00110000
  ; Save task context to stack
  pha       ; Push A, X, Y
  phx
  phy
  phb       ; Push data bank, direct register
  phd

  ; Return to same task
  tsc
  sta next_task_sp
  lda next_task_sp
  tcs

  ; swap stack pointer so that we will restore the other task
;  tsc                     ; transfer current stack pointer to memory
;  sta temp
;  lda next_task_sp        ; load next stack pointer from memory
;  tcs
;  lda temp                ; previous task is up next
;  sta next_task_sp

  ; Clear interrut
  .a8
  .i8
  sep #%00110000  ; save X only (assumes it is 8 bits..)
  ldx T1C_L       ; Clear the interrupt, side-effect of reading

  .a16
  .i16
  rep #%00110000
  ; Restore process context from stack, reverse order
  pld       ; Pull direct register, data bank
  plb
  ply       ; Pull Y, X, A
  plx
  pla
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
