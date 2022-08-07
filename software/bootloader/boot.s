; boot.s: 512-byte bootloader. This utilizes services defined in ROM.
ROM_PRINT_CHAR   := $00
ROM_PRINT_STRING := $02
ROM_READ_DISK := $03

.segment "CODE"
    .a16
    .i16
    jmp code_start

code_start:
    ; load kernel
    ldx #loading_kernel             ; Print loading message (kernel)
    cop ROM_PRINT_STRING
    phb                             ; Save current data bank register
    .a8                             ; Set data bank register to 1
    php
    sep #%00100000
    lda #1
    pha
    plb
    plp
    .a16
    ldx #0                          ; destination address
    lda #1                          ; block number
    ldy #16                         ; number of blocks to read
    cop ROM_READ_DISK               ; read kernel to RAM
    plb                             ; Restore previous data bank register

    ; load ramdisk, or 128k of it anyway (takes a while and it is mostly blank)
    ldx #loading_ramdisk            ; Print loading message (ramdisk)
    cop ROM_PRINT_STRING
    phb                             ; Save current data bank register
    .a8                             ; Set data bank register to 8
    php
    sep #%00100000
    lda #$08
    pha
    plb
    plp
    .a16

    ldx #0                          ; destination address
    lda #17                         ; block number
    ldy #128                        ; number of blocks to read
    cop ROM_READ_DISK               ; read ramdisk to RAM (1 of 8)
    lda #'.'                        ; Progress..
    cop ROM_PRINT_CHAR

    .a8                             ; Set data bank register to 9
    php
    sep #%00100000
    lda #$09
    pha
    plb
    plp
    .a16

    ldx #0                          ; destination address
    lda #17                         ; block number
    ldy #145                        ; number of blocks to read
    cop ROM_READ_DISK               ; read ramdisk to RAM (2 of 8)
    lda #'.'                        ; Progress..
    cop ROM_PRINT_CHAR

    plb                             ; Restore previous data bank register
    ldx #newline                    ; Newline
    cop ROM_PRINT_STRING

    ldx #boot                       ; Print boot message
    cop ROM_PRINT_STRING
    jml $010000

loading_kernel: .asciiz "Loading kernel\r\n"
loading_ramdisk: .asciiz "Loading ramdisk"
newline: .asciiz "\r\n"
boot: .asciiz "Booting\r\n"

.segment "SIGNATURE"
    wdm $42                         ; Ensure x86 systems don't recognise this as bootable.
