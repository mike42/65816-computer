.import nmi, multitasking_test, uart_test

.segment "CODE"
reset:
  clc                             ; switch to native mode
  xce
  ; Set up stack and run UART test first
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%00110000
  lda #$0200                      ; set up stack
  tcs
  jsr uart_test
  ; Run multi-tasking test next.
  jmp multitasking_test           ; Test multitasking afterward

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
