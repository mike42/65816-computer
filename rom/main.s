; main.s: ROM startup code
.import post_start, uart_init, uart_printz

.export reset, post_done

.segment "CODE"
reset:
    .a8
    .i8
    clc                            ; switch to native mode
    xce
    jmp post_start

post_done:                         ; run some code
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    jsr uart_init
    ldx #rom_message
    jsr uart_printz
    ldx #halt_message
    jsr uart_printz
    stp

rom_message: .asciiz "ROM revision 1.\r\n"
halt_message: .asciiz "Halted\r\n"
