.import uart_init, uart_print_char, uart_recv_char, uart_printz

.segment "CODE"
reset:
  clc                             ; switch to native mode
  xce
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%00110000
                                  ; TODO set stack

  jsr uart_init                   ; hardware init
  ldx #test_string                ; test output
  jsr uart_printz

  jsr uart_recv_char              ; test input
  jsr uart_print_char
  jsr uart_recv_char
  jsr uart_print_char
  jsr uart_recv_char
  jsr uart_print_char

  lda #$0a                        ; print a newline
  jsr uart_print_char

  lda #0                          ; Exit and return 0 in emulator.
	stp

test_string: .asciiz "Test test"

nmi_interrupt:
	rti

irq_interrrupt:
  rti

cop_interrupt:
  rti

unused_interrupt:                 ; Probably make this into a crash.
  rti

.segment "VECTORS"
; native mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word cop_interrupt               ; COP
.word unused_interrupt            ; BRK
.word unused_interrupt            ; Abort
.word nmi_interrupt               ; NMI
.word unused_interrupt            ; Reserved
.word irq_interrrupt              ; IRQ

; emulation mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; COP
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Abort
.word unused_interrupt            ; NMI
.word reset                       ; Reset
.word unused_interrupt            ; IRQ/BRK
