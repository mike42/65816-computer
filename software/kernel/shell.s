.import uart_printz, uart_print_char, uart_recv_char

.export shell_main

; simple shell. this is part of the kernel for now

; Constants for ASCII control characters
ASCII_NUL := $00
ASCII_SOH := $01
ASCII_STX := $02
ASCII_ETX := $03
ASCII_EOT := $04
ASCII_ENQ := $05
ASCII_ACK := $06
ASCII_BEL := $07
ASCII_BS := $08
ASCII_HT := $09
ASCII_LF := $0a
ASCII_VT := $0b
ASCII_FF := $0c
ASCII_CR := $0d
ASCII_SO := $0e
ASCII_SI := $0f
ASCII_DLE := $10
ASCII_DC1 := $11
ASCII_DC2 := $12
ASCII_DC3 := $13
ASCII_DC4 := $14
ASCII_NAK := $15
ASCII_SYN := $16
ASCII_ETB := $17
ASCII_CAN := $18
ASCII_EM := $19
ASCII_SUB := $1a
ASCII_ESC := $1b
ASCII_FS := $1c
ASCII_GS := $1d
ASCII_RS := $1e
ASCII_US := $1f
ASCII_DEL := $7f

SHELL_BUF_MAX := 64

; Incoming characters are parsed via a state machine
SHELL_STATE_DEFAULT := 0    ; expect ascii or escape code
SHELL_STATE_ESC := 2        ; ESC has been pressed. [ now has special meaning
SHELL_STATE_CODE := 4       ; ESC [ has been pressed. Number, semicolon, or one-letter code expected.

.segment "BSS"
shell_buffer: .res SHELL_BUF_MAX
shell_buffer_used: .res 2
shell_state: .res 2

.segment "CODE"
shell_main:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    stz shell_state
    ldx #shell_welcome             ; print welcome message
    jsr uart_printz
    jsr shell_newline

shell_next_command:
    ldx #shell_prompt              ; prompt for next command
    jsr uart_printz
    stz shell_buffer_used

shell_next_char:
    jsr uart_recv_char             ; wait for a character
    ldx shell_state
    jmp (shell_parse_table, X)

shell_parse_default:
    cmp #ASCII_LF                  ; return key pressed? - might need to be ASCII_CR
    beq @run_command               ; run the command
    cmp #32                        ; Check for control characters 0-31, which are ^@, then ^A through ^Z, then ^[, ^\, ^], ^^, ^_
    bcc @ascii_control_code
    cmp #ASCII_DEL
    beq @ascii_del_pressed
    ldx shell_buffer_used          ; append any other key to buffer
    cpx #SHELL_BUF_MAX             ; .. unless buffer full
    bcs shell_next_char
    sta shell_buffer, X            ; ascii printable is assumed, save to buffer
    inx
    inx
    stx shell_buffer_used
    jsr uart_print_char
    jmp shell_next_char
@run_command:
    jsr shell_newline
    jmp shell_run_command
@ascii_del_pressed:
    lda #32
@ascii_control_code:               ; run a method from control code table
    asl
    tax
    jsr (ascii_control_code_table, X)
    jmp shell_next_char

shell_parse_esc:
    cmp #'['
    bne @not_left_bracket
    lda #SHELL_STATE_CODE
    sta shell_state
    jmp shell_next_char
@not_left_bracket:
    lda #SHELL_STATE_DEFAULT
    sta shell_state
    jmp shell_next_char

shell_parse_code:
    ; reset to normal state
    lda #SHELL_STATE_DEFAULT
    sta shell_state
    ; do nothing
    jmp shell_next_char

shell_parse_table:
.word shell_parse_default
.word shell_parse_esc
.word shell_parse_code

ascii_control_code_table:
.word shell_shortcut_nop           ; ^@
.word shell_shortcut_nop           ; ^A home
.word shell_shortcut_nop           ; ^B end
.word shell_shortcut_nop           ; ^C cancel command
.word shell_shortcut_exit          ; ^D
.word shell_shortcut_nop           ; ^E
.word shell_shortcut_nop           ; ^F one character forward
.word shell_shortcut_nop           ; ^G one character backwards
.word shell_shortcut_nop           ; ^H
.word shell_shortcut_nop           ; ^I
.word shell_shortcut_nop           ; ^J
.word shell_shortcut_nop           ; ^K
.word shell_shortcut_clear_screen  ; ^L
.word shell_shortcut_nop           ; ^M
.word shell_shortcut_nop           ; ^N
.word shell_shortcut_nop           ; ^O
.word shell_shortcut_nop           ; ^P
.word shell_shortcut_nop           ; ^Q
.word shell_shortcut_nop           ; ^R
.word shell_shortcut_nop           ; ^S
.word shell_shortcut_nop           ; ^T
.word shell_shortcut_delete_to_line_start ; ^U
.word shell_shortcut_nop           ; ^V
.word shell_shortcut_nop           ; ^W
.word shell_shortcut_nop           ; ^X
.word shell_shortcut_nop           ; ^Y
.word shell_shortcut_nop           ; ^Z
.word shell_shortcut_esc           ; ^[
.word shell_shortcut_nop           ; ^\
.word shell_shortcut_nop           ; ^]
.word shell_shortcut_nop           ; ^^
.word shell_shortcut_nop           ; ^_
.word shell_shortcut_backspace     ; ^?

shell_shortcut_nop:
    rts

shell_shortcut_exit:
    lda shell_buffer_used          ; check if line is empty
    cmp #0
    bne @shell_shortcut_exit_skip
    ldx #shell_exit_bin            ; print the name of the exit command
    jsr uart_printz
    jsr shell_newline              ; run the exit command
    jsr shell_exit_main
@shell_shortcut_exit_skip:
    rts

shell_shortcut_clear_screen:
    jsr shell_clear_main           ; clear screen
    jsr shell_reprint_line
    rts

shell_shortcut_delete_to_line_start:
    lda #$08
    ldx shell_buffer_used          ; skip if line empty (shell_buffer_used = 0)
@backspace_next_char:
    beq @backspace_done
    dex
    dex
    jsr uart_print_char
    cpx #0
    jmp @backspace_next_char
@backspace_done:
    stz shell_buffer_used          ; all chars in buffer have now been backspaced
    ldx #shell_clear_end_of_line   ; drop chars off end of line
    jsr uart_printz
    rts

shell_shortcut_esc:
    lda #SHELL_STATE_ESC
    sta shell_state
    rts

shell_shortcut_backspace:
    ldx shell_buffer_used               ; skip if line empty (shell_buffer_used = 0)
    beq @shell_shortcut_backspace_skip
    dex
    dex
    stx shell_buffer_used
    lda #$08                            ; backspace
    jsr uart_print_char
    ldx #shell_clear_end_of_line        ; drop chars off end of line
    jsr uart_printz
@shell_shortcut_backspace_skip:
    rts
shell_clear_end_of_line: .asciiz "\x1b[K"

shell_newline:
    lda #$0a                       ; print a newline
    jsr uart_print_char
    rts

shell_reprint_line:
    ldx #shell_prompt              ; print out the prompt
    jsr uart_printz
    ldx #0
@shell_reprint_line_next:
    cpx shell_buffer_used
    bcs @shell_reprint_line_done
    lda shell_buffer, X
    jsr uart_print_char
    inx
    inx
    jmp @shell_reprint_line_next
@shell_reprint_line_done:
    rts

shell_run_command:
    ; jsr shell_hello_main         ; TODO look up command in shell_builtin...
    ldx #shell_not_found           ; pretend command is not found
    jsr uart_printz
    jsr shell_newline

    jmp shell_next_command

; prompts and messages
shell_not_found: .asciiz "Command not found"
shell_welcome: .asciiz "65816 Computer Ready"
shell_prompt: .asciiz "# "

;
; Built-in command table
;
shell_builtin:
    .word shell_hello_bin, shell_hello_main
    .word shell_exit_bin, shell_exit_main
    .word shell_clear_bin, shell_clear_main

;
; Built-in command: hello
;
shell_hello_bin: .asciiz "hello"
shell_hello_main:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    ldx #hello_world
    jsr uart_printz
    jsr shell_newline
    rts
hello_world: .asciiz "Hello, world"

;
; Built-in command: clear
;
shell_clear_bin: .asciiz "clear"
shell_clear_main:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    ldx #shell_clear_str
    jsr uart_printz
    rts
shell_clear_str: .asciiz "\x1b[H\x1b[2J\x1b[3J"

;
; Built-in command: exit
;
shell_exit_bin: .asciiz "exit"
shell_exit_main:
    lda #0
    stp
