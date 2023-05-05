; main.s: ROM startup code
.import BOOTLOADER_BASE, post_start, uart_init, spi_sd_init, hexdump_memory_block, xmodem_recv

.importzp ROM_PRINT_STRING, ROM_READ_CHAR, ROM_READ_DISK, ROM_PRINT_CHAR

.export reset, post_done

.segment "CODE"
reset:
    .a8
    .i8
    clc                             ; switch to native mode
    xce
    jmp post_start

post_done:                          ; POST passed, time to run some code
    .a16                            ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    jsr uart_init                   ; show a startup message
    ldx #rom_message
    cop ROM_PRINT_STRING
.if .defined(WARM_BOOT)
; stub out boot process if WARM_BOOT is defined at build time.
; this is a hack for the emulator: it will place the code in RAM for us, and
; these disk routines will not work.
    ldx #warm_boot_message
    cop ROM_PRINT_STRING
    jml $010000
.endif
    ; Current boot order
    jsr sd_init_multiple_attempts   ; attempt SD card init
    cmp #0
    bne @boot_no_sd
    jsr boot_from_sd
@boot_no_sd:                        ; no SD card, or boot from SD declined
    jsr boot_from_serial
    jsr boot_fail

; attempt to reset SD card up to 3 times - first attempt will often fail from a cold start
MAX_ATTEMPTS := 30
sd_init_multiple_attempts:
    ldx #0
@sd_init_again:
    phx
    jsr spi_sd_init                 ; attempt SD reset
    plx
    cmp #0
    beq @sd_init_ok
    inx
    cpx #MAX_ATTEMPTS               ; compare to max attempts
    bne @sd_init_again
    ldx #string_sd_reset_fail
    cop ROM_PRINT_STRING
    lda #1                          ; return nonzero
@sd_init_ok:
    rts

boot_from_sd:
    ldx #BOOTLOADER_BASE            ; destination address
    lda #0                          ; block number
    ldy #1                          ; number of blocks to read
    cop ROM_READ_DISK               ; read boot sector to RAM
    ; Hexdump the boot sector
    ldx #BOOTLOADER_BASE            ; source address
    jsr hexdump_memory_block
    ; Prompt for whether to run the bootloader.
    ldx #boot_prompt_sd
    cop ROM_PRINT_STRING
    cop ROM_READ_CHAR
    cmp #'y'
    beq @boot_from_sd_ok
    cmp #'Y'
    beq @boot_from_sd_ok
    ldx #newline
    cop ROM_PRINT_STRING
    rts
@boot_from_sd_ok:
    ldx #newline
    cop ROM_PRINT_STRING
    jmp BOOTLOADER_BASE

boot_from_serial:
    ldx #boot_prompt_serial         ; Ask if user wants to boot from serial
    cop ROM_PRINT_STRING
    cop ROM_READ_CHAR
    cmp #'y'
    beq @boot_from_serial_selected
    cmp #'Y'
    beq @boot_from_serial_selected
    ldx #newline                    ; Serial not selected
    cop ROM_PRINT_STRING
    rts
@boot_from_serial_selected:
    ldx #newline                    ; Serial is selected
    cop ROM_PRINT_STRING
    jsr xmodem_recv                 ; Receive program over serial
    cmp #0                          ; 0 is OK, nonzero means problem
    bne @boot_from_serial_fail
    ldx #start_of_line              ; move cursor back to start of line - some whitespace is printed by xmodem_recv
    cop ROM_PRINT_STRING
    jml $010000                     ; Run the uploaded program
@boot_from_serial_fail:
    ldx #start_of_line              ; move cursor back to start of line
    cop ROM_PRINT_STRING
    ldx #string_boot_serial_fail    ; Something went wrong with the transfer
    cop ROM_PRINT_STRING
    rts

boot_fail:
    ldx #halt_message
    cop ROM_PRINT_STRING
    stp

rom_message:                        ; ASCII art startup message with ROM revision.
.byte $1b
.asciiz "[2J+---------------------------------+\r\n|   __  ____   ____ ___  _  __    |\r\n|  / /_| ___| / ___( _ )/ |/ /_   |\r\n| | '_ \\___ \\| |   / _ \\| | '_ \\  |\r\n| | (_) |__) | |__| (_) | | (_) | |\r\n|  \\___/____/ \\____\\___/|_|\\___/  |\r\n|                                 |\r\n|         ROM revision 19         |\r\n+---------------------------------+\r\n"
halt_message: .asciiz "No boot options remaining. Halted\r\n"
boot_prompt_sd: .asciiz "Boot from SD card? (y/N) "
string_sd_reset_fail: .asciiz "SD card init failed\r\n"
boot_prompt_serial: .asciiz "Boot from serial (y/N) "
string_boot_serial_fail: .asciiz "Serial transfer failed\r\n"
.if .defined(WARM_BOOT)
warm_boot_message: .asciiz "Performing warm boot\r\n"
.endif
start_of_line:
.byte $1b
.asciiz "[1G"
newline: .asciiz "\r\n"
