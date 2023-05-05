; kernel_tmp.s: A temporary placeholder kernel to test boot process.

ROM_PRINT_CHAR   := $00

.segment "CODE"
    .a16
    .i16
demo_code:
    lda #'z'
    cop ROM_PRINT_CHAR
    stp
