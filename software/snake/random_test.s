.segment "CODE"
.import __srand, random_state, RAND_MAX, __rand

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

test_rand:
    ; generate some numbers to check the algorithm is correct: expected values found by running C implementation first.
    .a16
    .i16
    lda #123                        ; seed number generator with 123
    pha
    jsr __srand
    jsr __rand                      ; generate 3 random numbers
    cmp #63610                      ; first expected is 63610
    beq :+
    lda #1                          ; assertion failed
    rts
:   jsr __rand
    cmp #61323                      ; second expected is 61323
    beq :+
    lda #1                          ; assertion failed
    rts
:   jsr __rand
    cmp #18512                      ; third expected is 18512
    beq :+
    lda #1                          ; assertion failed
    rts
:   lda #0                          ; test passes
    rts
