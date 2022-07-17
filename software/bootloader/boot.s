; boot.s: 512-byte bootloader. This utilizes services defined in ROM.

ROM_PRINT_STRING := $02
ROM_READ_DISK := $03

.segment "CODE"
    .a16
    .i16
    jmp code_start
kernel_start_block:
.word $01
ramdisk_start_block:
.word $09
code_start:
    ldx #loading_kernel
    cop ROM_PRINT_STRING
;    phb                             ; save data bank register for later
;    ; set data bank to $01
;    .a8                             ; Use 8-bit accumulator
;    sep #%00100000
;    lda #1
;    pha
;    .a16                            ; Revert to 16-bit accumulator
;    rep #%00100000
;    plb

    ldx #$4000                      ; destination address
    lda #1                          ; block number
    ldy #16                         ; number of blocks to read
    cop ROM_READ_DISK               ; read boot sector to RAM

;    plb                             ; restore data bank register ($00)
    ldx #loading_ramdisk
    cop ROM_PRINT_STRING

; TODO set to bank $08 and load ramdisk from ramdisk_start
;    ldx #loading_ramdisk
;    cop ROM_PRINT_STRING
;    ldx #0                          ; destination address
;    lda ramdisk_start               ; block number
;    ldy #1024                       ; number of blocks to read
;    cop ROM_READ_DISK               ; read boot sector to RAM

    ldx #boot
    cop ROM_PRINT_STRING
    jmp $4000
;    jml $01e000

loading_kernel: .asciiz "Loading kernel\r\n"
loading_ramdisk: .asciiz "Skipping ramdisk\r\n"
boot: .asciiz "Booting\r\n"

.segment "SIGNATURE"
    wdm $42                         ; Ensure x86 systems don't recognise this as bootable.
