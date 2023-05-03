.segment "CODE"

test_setup:                         ; do nothing
    rts

test_this_will_pass:
    lda #0
    rts

test_this_will_fail:
    lda #1
    rts
