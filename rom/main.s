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
    ldx #long_rom_message          ; really show a startup message
    jsr uart_printz
    ; Current test program
    .a8                            ; use 8-bit accumulator and index registers
    .i8
    sep #%00110000
    jsr sd_test_main
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    ; Halt
    ldx #halt_message
    jsr uart_printz
    stp

rom_message: .asciiz "ROM revision 4.\r\n"
long_rom_message: .asciiz "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna\r\naliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\r\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint\r\noccaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\r\n"
halt_message: .asciiz "Halted\r\n"
