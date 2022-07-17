; boot.s: 512-byte bootloader. This utilizes services defined in ROM.

ROM_PRINT_STRING := $02
ROM_READ_DISK := $03

.segment "CODE"
    .a16
    .i16
    jmp code_start

code_start:
    ldx #loading_kernel
    cop ROM_PRINT_STRING

    ldx #$a000                      ; destination address
    lda #1                          ; block number
    ldy #16                         ; number of blocks to read
    cop ROM_READ_DISK               ; read boot sector to RAM

    ldx #boot
    cop ROM_PRINT_STRING
    jmp $a000

loading_kernel: .asciiz "Loading kernel\r\n"
boot: .asciiz "Booting\r\n"

.segment "SIGNATURE"
    wdm $42                         ; Ensure x86 systems don't recognise this as bootable.
