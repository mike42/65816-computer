; main.s: entry point for kernel
.import uart_init, uart_print_char, uart_recv_char, uart_printz
.export main

.segment "CODE"
main:
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
