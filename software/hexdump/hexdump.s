UART_THR = $c200
UART_LSR = $c205

.segment "CODE"
__main:
    ; set data bank register to equal program bank register (for accessing constants etc).
    phk
    plb
    .a8                             ; Switch to 16-bit index, 8-bit accumulator for per-byte work
    .i16
    sep #%00100000
    rep #%00010000

    ldx #$0000
@again:
    jsr hexdump_line
    cpx #$2000                      ; Size of loaded image
    bcc @again
    stp

; Given address in X, prints a line. Eg.
; 000c00  67 67 67 67 67 67 67 67  67 67 67 67 67 67 67 67  |gggggggggggggggg|
hexdump_line:
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    ; Print memory address first
    phx                             ; Push the address we are working on
    phb
    pla                             ; Bank byte
    jsr hex_print_byte
    lda $02, S                      ; High byte
    jsr hex_print_byte
    lda $01, S                      ; Low byte
    jsr hex_print_byte
    lda #' '                        ; Two spaces
    jsr uart_print_char
    jsr uart_print_char
    plx                             ; Restore X but keep it on stack
    phx
    jsr hexdump_line_hex            ; Print 16 bytes hex
    plx                             ; Restore X but keep it on stack
    phx
    lda #' '
    jsr uart_print_char             ; Extra space
    lda #'|'
    jsr uart_print_char             ; Print 16 bytes ascii
    plx                             ; Restore X again
    jsr hexdump_line_ascii
    phx                             ; Push incremented (+16 bytes) value this time
    lda #'|'
    jsr uart_print_char
    ; Newline at the end
    jsr shell_newline
    plx                             ; Restore X one last time
    rts

hexdump_line_hex:
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    ; Print line in hex
    ldy #0
@hexdump_line_next_byte:
    lda a:0000, X
    phx
    jsr hex_print_byte              ; Hex byte
    lda #' '                        ; Spaces between bytes
    jsr uart_print_char
    plx
    inx
    iny
    cpy #8
    beq @hexdump_line_half_way
    cpy #16
    beq @hexdump_line_done
    jmp @hexdump_line_next_byte
@hexdump_line_half_way:
    lda #' '                        ; Extra space after 8 bytes
    jsr uart_print_char
    jmp @hexdump_line_next_byte
@hexdump_line_done:
    rts

hexdump_line_ascii:
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    ldy #0
@hexdump_line_ascii_next_byte:
    lda a:0000, X
    phx
    cmp #128                        ; Substitute with '.' if >= 128
    bcs @char_subst
    cmp #32                         ; Print if >= 32
    bcs @char_print
@char_subst:                        ; Fallthrough to substitute
    lda #'.'
@char_print:
    jsr uart_print_char
    plx
    inx
    iny
    cpy #16
    beq @hexdump_line_ascii_done
    jmp @hexdump_line_ascii_next_byte
@hexdump_line_ascii_done:
    rts

shell_newline:
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    lda #$0d
    jsr uart_print_char
    lda #$0a
    jsr uart_print_char
    rts

hex_print_byte:                     ; print accumulator as two ascii digits (hex)
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    pha                             ; store byte for later
    lsr                             ; shift out lower nibble
    lsr
    lsr
    lsr
    jsr hex_print_nibble
    pla                             ; retrieve byte again
    jsr hex_print_nibble
    rts

hex_print_nibble:                   ; print one hex digit using least significant 4 bits in the A register
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    and #$0f
    tax                             ; NB: this assumes B accumulator is 0
    lda f:hex_chars, X              ; convert nibble in X to ASCII value. relies on code being in first bank
    jsr uart_print_char
    rts

uart_print_char:
    .a8                             ; assume 8-bit accumulator and 16-bit index registers
    .i16
    pha
@uart_print_wait_for_ready:         ; wait until transmit register is empty
    lda f:UART_LSR
    and #%00100000
    cmp #%00100000
    bne @uart_print_wait_for_ready
    pla
    sta f:UART_THR
    rts

hex_chars: .byte '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'

.segment "FILL"
.res 256, 'a'
.res 256, 'b'
.res 256, 'c'
.res 256, 'd'
.res 256, 'e'
.res 256, 'f'
.res 256, 'g'
.res 256, 'h'
.res 256, 'i'
.res 256, 'j'
.res 256, 'k'
.res 256, 'l'
.res 256, 'm'
.res 256, 'n'
.res 256, 'o'
.res 256, 'p'
.res 256, 'q'
.res 256, 'r'
.res 256, 's'
.res 256, 't'
.res 256, 'u'
.res 256, 'v'
.res 256, 'w'
.res 256, 'x'
.res 256, 'y'
.res 256, 'z'
.res 256, '0'
.res 256, '1'
.res 256, '2'
.res 256, '3'
.res 256, '4'
