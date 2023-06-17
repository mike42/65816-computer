; random.s: pseudo-random numbers for the 65C816
.export RAND_MAX, __rand, __srand, random_state

RAND_MAX := 65535

.segment "BSS"
random_state: .res 2

.segment "CODE"
;void srand(uint16_t value)
__srand:
    .a16                            ; assume 16-bit accumulator and index register
    .i16
    tsc                             ; Prologue
    sec
    sbc #0
    tcs
    phd
    tcd
srand_value    :=    3
    lda <srand_value                ; random_state = value + 1;
    inc
    sta random_state
    lda random_state                ; if(random_state == 0) {
    bne random_state_nonzero        ;   // Random state must never be 0!
    inc random_state                ;   random_state++;
random_state_nonzero:               ; }
    lda <1                          ; Epilogue
    sta <3
    pld
    tsc
    clc
    adc #2
    tcs
    rts

;uint16_t rand()
; This is a 16-bit Xorshift https://en.wikipedia.org/wiki/Xorshift
; 7,9,13 triplet is from http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
__rand:
    .a16
    .i16
    lda random_state                ; random_state ^= random_state >> 7;
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    eor random_state
    sta random_state                ; random_state ^= random_state << 9;
    asl
    asl
    asl
    asl
    asl
    asl
    asl
    asl
    asl
    eor random_state
    sta random_state                ; random_state ^= random_state >> 13;
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    lsr
    eor random_state
    sta random_state
    dec                             ; return random_state - 1; // to match built-in rand.
    rts
