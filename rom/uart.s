; uart.s: driver for NXP SC16C752 UART
.import UART_BASE
.export uart_init, uart_print_char, uart_recv_char, uart_printz

; UART registers
; Not included: FIFO ready register, Xon1 Xon2 words.
UART_RHR = UART_BASE        ; Receiver Holding Register (RHR) - R
UART_THR = UART_BASE        ; Transmit Holding Register (THR) - W
UART_IER = UART_BASE + 1    ; Interrupt Enable Register (IER) - R/W
UART_IIR = UART_BASE + 2    ; Interrupt Identification Register (IIR) - R
UART_FCR = UART_BASE + 2    ; FIFO Control Register (FCR) - W
UART_LCR = UART_BASE + 3    ; Line Control Register (LCR) - R/W
UART_MCR = UART_BASE + 4    ; Modem Control Register (MCR) - R/W
UART_LSR = UART_BASE + 5    ; Line Status Register (LSR) - R
UART_MSR = UART_BASE + 6    ; Modem Status Register (MSR) - R
UART_SPR = UART_BASE + 7    ; Scratchpad Register (SPR) - R/W
; Different meaning when LCR is logic 1
UART_DLL = UART_BASE        ; Divisor latch LSB - R/W
UART_DLM = UART_BASE + 1    ; Divisor latch MSB - R/W
; Different meaning when LCR is %1011 1111 ($bh).
UART_EFR = UART_BASE + 2    ; Enhanced Feature Register (EFR) - R/W
UART_TCR = UART_BASE + 6    ; Transmission Control Register (TCR) - R/W
UART_TLR = UART_BASE + 7    ; Trigger Level Register (TLR) - R/W

.segment "CODE"
; uart_init: Initialize the UART
uart_init:
    .a8                       ; Use 8-bit accumulator
    sep #%00100000

    lda #$80                  ; Enable divisor latches
    sta UART_LCR
    lda #1                    ; Set divisior to 1 - on a 1.8432 MHZ XTAL1, this gets 115200bps.
    sta UART_DLL
    lda #0
    sta UART_DLM
    lda #%00010111            ; Sets up 8-n-1
    sta UART_LCR
    lda #%00001111            ; Enable FIFO, set DMA mode 1
    sta UART_FCR
    .a16                      ; Revert to 16-bit accumulator
    rep #%00100000
    rts

; uart_print_char: Print a single character to the UART
; A - ASCII character to print. Only the least significant 8 bits are used
uart_print_char:
    php
    .a8                       ; Use 8-bit accumulator
    sep #%00100000
    pha
@uart_wait_for_ready:         ; wait until transmit register is empty
    lda UART_LSR
    and #%00100000
    cmp #%00100000
    bne @uart_wait_for_ready
    pla
    sta UART_THR
    plp                       ; Revert to previous setting
    rts

; uart_printz: Print characters to UART until a null is reached
; X - pointer to string to print
uart_printz:
    .a8                       ; Use 8-bit accumulator
    sep #%00100000
@uart_printz_next:
    lda a:$0000, X            ; a: to avoid this being interpreted as direct page
    beq @uart_printz_done
    jsr uart_print_char
;   sta UART_THR ; much faster but drops chars..
    inx
    jmp @uart_printz_next
@uart_printz_done:
    .a16                      ; Revert to 16-bit accumulator
    rep #%00100000
    rts

; uart_recv_char: Block until a character is received, then load result to A register.
uart_recv_char:
    .a8                       ; Use 8-bit accumulator
    sep #%00100000
    lda UART_LSR
    and #%00000001
    cmp #%00000001
    bne uart_recv_char
    lda UART_RHR
    .a16                      ; Revert to 16-bit accumulator
    rep #%00100000
    rts
