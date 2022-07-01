; main.s: ROM startup code
.import BOOTLOADER_BASE, post_start, uart_init, uart_printz, uart_recv_char, spi_sd_init, spi_sd_block_read, hexdump_memory_block

.importzp ROM_PRINT_STRING

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
    jsr uart_printz
    ; Current boot order
    jsr boot_from_sd
    jsr boot_fail

boot_from_sd:
    jsr spi_sd_init                 ; initialise the SD card
    ldx #BOOTLOADER_BASE            ; destination address
    lda #$0000                      ; block number high
    ldy #$0000                      ; block number low
    jsr spi_sd_block_read           ; read boot sector to RAM
    ; Hexdump the boot sector
    ldx #BOOTLOADER_BASE            ; source address
    jsr hexdump_memory_block
    ; Prompt for whether to run the bootloader.
    ldx #boot_prompt
    jsr uart_printz
    jsr uart_recv_char
    cmp #'y'
    beq @boot_from_sd_ok
    cmp #'Y'
    beq @boot_from_sd_ok
    ldx #newline
    jsr uart_printz
    rts
@boot_from_sd_ok:
    ldx #newline
    jsr uart_printz
    jmp BOOTLOADER_BASE

boot_fail:
    ldx #halt_message
    cop ROM_PRINT_STRING
    ldx #halt_message_2
    cop ROM_PRINT_STRING
    stp

rom_message:                        ; ASCII art startup message with ROM revision.
.byte $1b
.asciiz "[2J+---------------------------------+\r\n|   __  ____   ____ ___  _  __    |\r\n|  / /_| ___| / ___( _ )/ |/ /_   |\r\n| | '_ \\___ \\| |   / _ \\| | '_ \\  |\r\n| | (_) |__) | |__| (_) | | (_) | |\r\n|  \\___/____/ \\____\\___/|_|\\___/  |\r\n|                                 |\r\n|         ROM revision 12          |\r\n+---------------------------------+\r\n"
halt_message: .asciiz "No boot options remaining.\r\n"
halt_message_2: .asciiz "Halted\r\n"
boot_prompt: .asciiz "Boot from SD card? (y/N) "
newline: .asciiz "\r\n"
