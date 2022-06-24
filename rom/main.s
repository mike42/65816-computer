; main.s: ROM startup code
.import post_start, uart_init, uart_printz, sd_test_main

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
    jsr uart_init                  ; show a startup message
    ldx #rom_message
    jsr uart_printz
    ; Current test program
    jsr sd_test_main
    ; Halt
    ldx #halt_message
    jsr uart_printz
    stp

rom_message:                       ; ASCII art startup message with ROM revision.
.byte $1b
.asciiz "[2J+---------------------------------+\r\n|   __  ____   ____ ___  _  __    |\r\n|  / /_| ___| / ___( _ )/ |/ /_   |\r\n| | '_ \\___ \\| |   / _ \\| | '_ \\  |\r\n| | (_) |__) | |__| (_) | | (_) | |\r\n|  \\___/____/ \\____\\___/|_|\\___/  |\r\n|                                 |\r\n|         ROM revision 9          |\r\n+---------------------------------+\r\n"
halt_message: .asciiz "Halted\r\n"
