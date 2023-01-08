; snake.s: a simple game to test the computer

ROM_PRINT_CHAR   := $00

.segment "CODE"
    .a16
    .i16
    lda #'s'
    cop ROM_PRINT_CHAR
    stp

