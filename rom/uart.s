; uart.s: driver for NXP SC16C752 UART
.import UART_BASE
.export uart_init, uart_print_char, uart_recv_char, uart_printz, uart_recv_char_with_timeout

; UART registers
; Not included: FIFO ready register, Xon1 Xon2 words.
UART_RHR = UART_BASE                ; Receiver Holding Register (RHR) - R
UART_THR = UART_BASE                ; Transmit Holding Register (THR) - W
UART_IER = UART_BASE + 1            ; Interrupt Enable Register (IER) - R/W
UART_IIR = UART_BASE + 2            ; Interrupt Identification Register (IIR) - R
UART_FCR = UART_BASE + 2            ; FIFO Control Register (FCR) - W
UART_LCR = UART_BASE + 3            ; Line Control Register (LCR) - R/W
UART_MCR = UART_BASE + 4            ; Modem Control Register (MCR) - R/W
UART_LSR = UART_BASE + 5            ; Line Status Register (LSR) - R
UART_MSR = UART_BASE + 6            ; Modem Status Register (MSR) - R
UART_SPR = UART_BASE + 7            ; Scratchpad Register (SPR) - R/W
; Different meaning when LCR is logic 1
UART_DLL = UART_BASE                ; Divisor latch LSB - R/W
UART_DLM = UART_BASE + 1            ; Divisor latch MSB - R/W
; Different meaning when LCR is %1011 1111 ($bh).
UART_EFR = UART_BASE + 2            ; Enhanced Feature Register (EFR) - R/W
UART_TCR = UART_BASE + 6            ; Transmission Control Register (TCR) - R/W
UART_TLR = UART_BASE + 7            ; Trigger Level Register (TLR) - R/W

.segment "CODE"
; uart_init: Initialize the UART
uart_init:
    .a8                             ; Use 8-bit accumulator
    sep #%00100000

    lda #$80                        ; Enable divisor latches
    sta f:UART_LCR
    lda #1                          ; Set divisior to 1 - on a 1.8432 MHZ XTAL1, this gets 115200bps.
    sta f:UART_DLL
    lda #0
    sta f:UART_DLM
    lda #%00010111                  ; Sets up 8-n-1
    sta f:UART_LCR
    lda #%00001111                  ; Enable FIFO, set DMA mode 1
    sta f:UART_FCR
    .a16                            ; Revert to 16-bit accumulator
    rep #%00100000
    rts

; uart_print_char: Print a single character to the UART
; A - ASCII character to print. Only the least significant 8 bits are used
uart_print_char:
    php
    .a8                             ; Use 8-bit accumulator
    sep #%00100000
    pha
@uart_print_wait_for_ready:         ; wait until transmit register is empty
    lda f:UART_LSR
    and #%00100000
    cmp #%00100000
    bne @uart_print_wait_for_ready
    pla
    sta f:UART_THR
    plp                             ; Revert to previous setting
    rts

; uart_printz: Print characters to UART until a null is reached
; X - pointer to string to print
uart_printz:
    php
    .a8                             ; Use 8-bit accumulator
    sep #%00100000
@uart_printz_next:
    lda a:$0000, X                  ; a: to avoid this being interpreted as direct page
    beq @uart_printz_done
    jsr uart_print_char
;   sta UART_THR ; much faster but drops chars..
    inx
    jmp @uart_printz_next
@uart_printz_done:
    plp                             ; Revert to previous setting
    rts

; uart_recv_char: Block until a character is received, then load result to A register.
uart_recv_char:
    php
    .a8                             ; Use 8-bit accumulator
    sep #%00100000
@uart_recv_wait_for_ready:
    lda f:UART_LSR
    and #%00000001
    cmp #%00000001
    bne @uart_recv_wait_for_ready
    lda f:UART_RHR
    plp                             ; Revert to previous setting
    rts

; uart_recv_char_with_timeout: Load any received character to A register and set carry bit. If no character is received
; after a short/arbitrary delay, then this will return with A=0 and a clear carry bit.
uart_recv_char_with_timeout:
    php
    .a8                             ; Switch to 16-bit index, 8-bit accumulator for per-byte work
    .i16
    sep #%00100000
    rep #%00010000
    ldx #$0004                      ; Tune outer loop $0000-$ffff to adjust timeout
:   ldy #$ffff                      ; Inner loop
:   lda f:UART_LSR                  ; Check for character
    and #%00000001
    cmp #%00000001
    beq @uart_recv_got_char
    dey                             ; Inner and outer loop
    cpy #0
    bne :-
    dex
    cpx #0
    bne :--
    lda #0                          ; Timeout here
    plp                             ; Revert to previous setting
    clc                             ; ... but clear carry bit
    rts
@uart_recv_got_char:                ; Character recieved here
    lda f:UART_RHR
    plp                             ; Revert to previous setting
    sec                             ; ... but set carry bit
    rts
