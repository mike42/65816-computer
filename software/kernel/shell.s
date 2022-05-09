.import uart_printz, uart_print_char, uart_recv_char

.export shell_main

; simple shell. this is part of the kernel for now

.segment "BSS"

shell_buffer: .res 64
shell_buffer_used: .res 2
shell_tmp_1: .res 2

.segment "CODE"
shell_main:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000

    ldx #shell_welcome             ; print welcome message
    jsr uart_printz
    jsr shell_newline

shell_next_command:
    ldx #shell_prompt              ; prompt for next command
    jsr uart_printz
    stz shell_buffer_used

shell_next_char:
    jsr uart_recv_char             ; wait for a character
    cmp #$0a                       ; return key pressed? - might need to be $0d
    beq @run_command               ; run the command
    ldx shell_buffer_used          ; append any other key to buffer
    cpx #64                        ; ignore char if buffer full
    bcs shell_next_char
    cmp #27                        ; Check for control characters 0-26, which are ^@, then ^A through ^Z
    bcc @ascii_control_code

    sta shell_buffer, X            ; ascii printable is assumed, save to buffer
    inx
    inx
    stx shell_buffer_used
    jsr uart_print_char

    jmp shell_next_char
@run_command:
    jsr shell_newline
    jsr shell_hello_main
    jmp shell_next_command
@ascii_control_code:                ; run a method from control code table
    asl
    tax
    jsr (ascii_control_code_table, X)
    jmp shell_next_char

ascii_control_code_table:
ctrl_at: .word shell_shortcut_nop
ctrl_a: .word shell_shortcut_nop    ; home
ctrl_b: .word shell_shortcut_nop    ; end
ctrl_c: .word shell_shortcut_nop    ; cancel command
ctrl_d: .word shell_shortcut_exit
ctrl_e: .word shell_shortcut_nop
ctrl_f: .word shell_shortcut_nop    ; one character forward
ctrl_g: .word shell_shortcut_nop    ; one character backwards
ctrl_h: .word shell_shortcut_nop
ctrl_i: .word shell_shortcut_nop
ctrl_j: .word shell_shortcut_nop
ctrl_k: .word shell_shortcut_nop
ctrl_l: .word shell_shortcut_clear_screen
ctrl_m: .word shell_shortcut_nop
ctrl_n: .word shell_shortcut_nop
ctrl_o: .word shell_shortcut_nop
ctrl_p: .word shell_shortcut_nop
ctrl_q: .word shell_shortcut_nop
ctrl_r: .word shell_shortcut_nop
ctrl_s: .word shell_shortcut_nop
ctrl_t: .word shell_shortcut_nop
ctrl_u: .word shell_shortcut_delete_to_line_start
ctrl_v: .word shell_shortcut_nop
ctrl_w: .word shell_shortcut_nop
ctrl_x: .word shell_shortcut_nop
ctrl_y: .word shell_shortcut_nop
ctrl_z: .word shell_shortcut_nop

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
    stz shell_buffer_used
    jsr shell_reprint_line
    rts

shell_newline:
    lda #$0a                       ; print a newline
    jsr uart_print_char
    rts

shell_reprint_line:
    ldx #shell_prompt              ; print out the prompt
    jsr uart_printz
    ldx #0
@shell_reprint_line_next:

    inx
    inx
    cpx shell_buffer_used


;
;
;    ldx #shell_buffer_used         ; throw a null at the end of the buffer
;    stz shell_buffer, X
;    ldx #shell_buffer

    rts

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
