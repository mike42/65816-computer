.segment "CODE"
.import __srand, random_state, RAND_MAX

test_setup:
    clc                             ; switch to native mode
    xce
    .a16                            ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    rts

test_srand_saves_seed:
    .a16
    .i16
    lda #41                         ; seed random number generator with 41
    pha
    jsr __srand
    lda random_state
    cmp #42                         ; assert random state is now 42 (expected value)
    beq :+
    lda #1                          ; assertion failed
    rts
:   lda #0                          ; test passed
    rts

test_srand_does_not_allow_zero:
    .a16
    .i16
    lda #RAND_MAX                   ; edge case: trigger wrap-around
    pha
    jsr __srand
    lda random_state
    cmp #1                          ; assert random state is 1 (not zero)
    beq :+
    lda #1                          ; assertion failed
    rts
:   lda #0                          ; test passed
    rts
