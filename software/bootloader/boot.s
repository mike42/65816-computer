; boot.s: 512-byte bootloader. This utilizes services defined in ROM.
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
    lda #$01
    pha
    plb
    plp
    .a16
    ldx #$0000                      ; lower 2 bytes of destination address
    lda #$01                        ; block number
    ldy #$10                        ; number of blocks to read - 8 KiB
    cop ROM_READ_DISK               ; read kernel to RAM
    plb                             ; Restore previous data bank register

    ldx #boot                       ; Print boot message
    cop ROM_PRINT_STRING
    jml $010000

loading_kernel: .asciiz "Loading kernel\r\n"
boot: .asciiz "Booting\r\n"

.segment "SIGNATURE"
    wdm $42                         ; Ensure x86 systems don't recognise this as bootable.
