; xmodem.s: Routines to receive a program to boot from serial
.import uart_recv_char_with_timeout, uart_print_char, uart_recv_char, VIA_BASE

.export xmodem_recv

; Destination address for programs loaded over serial
SERIAL_PROGRAM_BANK = $01           ; Destination bank
SERIAL_PROGRAM_BASE = $0000         ; Base

; Size of XMODEM block
XMODEM_BLOCK_SIZE := 128

; ASCII control characters used by XMODEM
ASCII_SOH := $01
ASCII_EOT := $04
ASCII_ACK := $06
ASCII_NAK := $15

.segment "ZEROPAGE"
dest_block_ptr: .res 2

.segment "CODE"
xmodem_recv:
    php
    .a8                             ; Switch to 16-bit index, 8-bit accumulator for per-byte work
    .i16
    sep #%00100000
    rep #%00010000
    phb                             ; Save data bank for later
    lda #SERIAL_PROGRAM_BANK        ; Set data bank register to where the program will be saved
    pha
    plb
    ldx #SERIAL_PROGRAM_BASE        ; Set up pointer to next block to work with
    stx dest_block_ptr
@shell_block_nak:                   ; NAK, ACK once
    lda #ASCII_NAK                  ; NAK gets started
    jsr uart_print_char
    jsr click                       ; Click each time we send a NAK or ACK
    jsr uart_recv_char_with_timeout ; Check in loop w/ timeout
    bcc @shell_block_nak            ; Not received yet
    cmp #ASCII_SOH                  ; If we do have char, should be SOH
    bne @shell_rx_fail              ; Terminate transfer if we don't get SOH
@shell_rx_block:
    ; Receive one block
    jsr uart_recv_char              ; Block number
    jsr uart_recv_char              ; Inverse block number
    ldy #0                          ; Start at char 0
@shell_rx_char:
    jsr uart_recv_char
    sta (dest_block_ptr), Y
    iny
    cpy #XMODEM_BLOCK_SIZE
    bne @shell_rx_char
    jsr uart_recv_char              ; Checksum - TODO verify this and jump to shell_block_nak to repeat if not matching
    lda #ASCII_ACK                  ; ACK the packet
    jsr uart_print_char
    jsr click                       ; Click each time we send a NAK or ACK
    jsr uart_recv_char
    cmp #ASCII_EOT                  ; EOT char, no more blocks
    beq @shell_rx_done
    cmp #ASCII_SOH                  ; SOH char, next block on the way
    bne @shell_block_nak            ; Anything else fail transfer for the block
    ; Move write pointer along for next block
    php
    .a16
    rep #%00100000                  ; Some quick 16-bit arithetic
    lda #XMODEM_BLOCK_SIZE
    adc dest_block_ptr
    sta dest_block_ptr
    plp
    .a8
    jmp @shell_rx_block
@shell_rx_done:
    lda #ASCII_ACK                  ; ACK the EOT as well.
    jsr uart_print_char
    jsr click                       ; Click each time we send a NAK or ACK


    jsr uart_recv_char_with_timeout ; induce delay, printing anything to the terminal immediately will not work.

    ; jsr shell_rx_print_user_program TODO
    ; jsr shell_newline TODO would also be useful
    lda #'!'
    jsr uart_print_char

    lda #0
    jmp @shell_rx_cleanup
@shell_rx_fail:
    jsr uart_recv_char_with_timeout ; induce delay, printing anything to the terminal immediately will not work.
    lda #1
@shell_rx_cleanup:
    plb                             ; Restore previous data bank register
    plp                             ; Restore previous processor status
    rts

VIA_PORTB = VIA_BASE + $00
click:                              ; Make speaker click
    .a8                             ; Assume 8-bit accumulator
    phb                             ; Swap to data bank 0
    lda #0
    pha
    plb
    lda #%10000000                  ; PB7
    tsb a:VIA_PORTB                 ; Set to 1
    nop
    nop
    nop
    trb a:VIA_PORTB                 ; Set to 0
    plb                             ; Restore data bank
    rts
