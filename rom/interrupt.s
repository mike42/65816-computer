; interrupt.s: Handling of software interrupts, the interface into the ROM for
; software (eg. bootloaders)

.import uart_printz
.export cop_handler
.export ROM_PRINT_CHAR, ROM_READ_CHAR, ROM_PRINT_STRING, ROM_READ_DISK

; Routines available in ROM via software interrupts.
; Print one ASCII char.
;   A is char to print
ROM_PRINT_CHAR   := $00
; Read one ASCII char.
;   Returns typed character in A register
ROM_READ_CHAR    := $01
; Print a null-terminated ASCII string.
;   X is address of string, use data bank register for addresses outside bank 0.
ROM_PRINT_STRING := $fe
; Read data from disk to RAM in 512 byte blocks.
;   X is address to write to, use data bank register for addresses outside bank 0.
;   A is high 2 bytes of block number
;   Y is low 2 bytes of block number
;   Number of blocks to read is read from the stack (2 bytes)
ROM_READ_DISK    := $03

.segment "CODE"
cop_handler:
    .a16                           ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    ; Save task context to stack
    pha                            ; Push A, X, Y
    phx
    phy
    phb                            ; Push data bank, direct register
    phd
    tsc                            ; set direct register to equal stack pointer
    tcd

    ; TODO read byte before return address to find which function is being called
    ldx $06                        ; saved X register from before
    jsr uart_printz

    ; Restore process context from stack, reverse order
    pld                             ; Pull direct register, data bank
    plb
    ply                             ; Pull Y, X, A
    plx
    pla
    rti
