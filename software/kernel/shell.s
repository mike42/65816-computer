.import uart_printz, uart_print_char, uart_recv_char

.export shell_main

; simple shell. this is part of the kernel for now

.segment "CODE"
shell_main:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000

    ldx #shell_prompt              ; prompt for next command
    jsr uart_printz

    jsr uart_recv_char             ; wait for a character

    ; do nothing                   ; take that to mean exit!
    lda #$0a                        ; print a newline
    jsr uart_print_char
    lda #$00
    rts


shell_prompt: .asciiz "# "
