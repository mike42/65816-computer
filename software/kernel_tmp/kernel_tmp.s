; kernel_tmp.s: A temporary placeholder program to test the boot process.

ROM_PRINT_STRING := $02

.segment "CODE"
    .a16
    .i16
    jmp __main

__main:
    ; set data bank register to equal program bank register
    phk
    plb

    ; Print test string
    ldx #(test_string & $ffff)
    cop ROM_PRINT_STRING

    ; Halt
    stp

test_string: .asciiz "Hello, world!\r\n"
