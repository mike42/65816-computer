; main.s: entry point for kernel
.import uart_init, uart_print_char, uart_recv_char, uart_printz, panic, __forty_two, shell_main
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

  ; kick off the shell
  jsr shell_main
  jsr panic

test_string: .asciiz "Hello, world!"
