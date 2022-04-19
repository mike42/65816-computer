; main.s: entry point for kernel
.import uart_init, uart_print_char, uart_recv_char, uart_printz, panic, __forty_two
.export main

.segment "CODE"
main:
  clc                             ; switch to native mode
  xce
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%00110000
  lda #$3000                      ; set up stack, direct page at $3000
  tcs
  tcd

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

  jsr  __forty_two                ; run some C code?

  jsr panic
  stp

test_string: .asciiz "Hello, world!"
