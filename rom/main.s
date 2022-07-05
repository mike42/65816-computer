; main.s: ROM startup code
.import BOOTLOADER_BASE, post_start, uart_init, spi_sd_init, hexdump_memory_block

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
    ; Current boot order
    jsr boot_from_sd
    jsr boot_fail

boot_from_sd:
    jsr spi_sd_init                 ; initialise the SD card
    ldx #BOOTLOADER_BASE            ; destination address
    lda #0                          ; block number
    ldy #1                          ; number of blocks to read
    cop ROM_READ_DISK               ; read boot sector to RAM
    ; Hexdump the boot sector
    ldx #BOOTLOADER_BASE            ; source address
    jsr hexdump_memory_block
    ; Prompt for whether to run the bootloader.
    ldx #boot_prompt
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

boot_fail:
    ldx #halt_message
    cop ROM_PRINT_STRING
    stp

rom_message:                        ; ASCII art startup message with ROM revision.
.byte $1b
.asciiz "[2J+---------------------------------+\r\n|   __  ____   ____ ___  _  __    |\r\n|  / /_| ___| / ___( _ )/ |/ /_   |\r\n| | '_ \\___ \\| |   / _ \\| | '_ \\  |\r\n| | (_) |__) | |__| (_) | | (_) | |\r\n|  \\___/____/ \\____\\___/|_|\\___/  |\r\n|                                 |\r\n|         ROM revision 13         |\r\n+---------------------------------+\r\n"
halt_message: .asciiz "No boot options remaining. Halted\r\n"
boot_prompt: .asciiz "Boot from SD card? (y/N) "
newline: .asciiz "\r\n"
