; interrupt.s: Handling of software interrupts, the interface into the ROM for
; software (eg. bootloaders)
;
; Usage: Set registers and use 'cop' to trigger software interrupt.
; Eg:
;   ldx #'a'
;   cop ROM_PRINT_CHAR
; CPU should be in native mode with all registers 16-bit.

.import uart_printz, uart_recv_char, uart_print_char, spi_sd_block_read
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
ROM_PRINT_STRING := $02

; Read data from disk to RAM in 512 byte blocks.
;   X is address to write to, use data bank register for addresses outside bank 0.
;   A is low 2 bytes of block number
;   Y is number of blocks to read
ROM_READ_DISK    := $03

.segment "CODE"
; table of routines
cop_routines:
.word rom_print_char_handler
.word rom_read_char_hanlder
.word rom_print_string_handler
.word rom_read_disk_handler

cop_handler:
    .a16                            ; use 16-bit accumulator and index registers
    .i16
    rep #%00110000
    ; Save caller context to stack
    pha                             ; Push A, X, Y
    phx
    phy
    phb                             ; Push data bank, direct register
    phd
    ; Set up stack frame for COP handler
    tsc
    sec
    sbc #cop_handler_local_vars_size
    tcs
    phd
    tcd
caller_k := 15
caller_ret := 13
caller_p := 12
caller_a := 10
caller_x := 8
caller_y := 6
caller_b := 5
caller_d := 3
cop_call_addr := 0
    ; set up 24 bit pointer to COP instruction
    ldx <frame_base+caller_ret
    dex
    dex
    stx <frame_base+cop_call_addr
    .a8                             ; Use 8-bit accumulator
    sep #%00100000
    lda <frame_base+caller_k
    sta <frame_base+cop_call_addr+2
    .a16                            ; Revert to 16-bit accumulator
    rep #%00100000

    ; load COP instruction which triggered this interrupt to figure out routine to run
    lda [<frame_base+cop_call_addr]
    xba                             ; interested only in second byte
    and #%00000011                  ; mask down to final two bits (there are only 4 valid functions at the moment)
    asl                             ; multiply by 2 to index into table of routines
    tax
    jsr (cop_routines, X)

    ; Remove stack frame for COP handler
    pld
    tsc
    clc
    adc #cop_handler_local_vars_size
    tcs

    ; Restore caller context from stack, reverse order
    pld                             ; Pull direct register, data bank
    plb
    ply                             ; Pull Y, X, A
    plx
    pla
    rti

cop_handler_local_vars_size := 3
frame_base := 1

rom_print_char_handler:
    ; Print char from A register
    lda <frame_base+caller_a
    jsr uart_print_char
    rts

rom_read_char_hanlder:
    ; Receive character, caller gets result in A register
    jsr uart_recv_char
    sta <frame_base+caller_a
    rts

rom_print_string_handler:
    ; Print string from X register
    ldx <frame_base+caller_x
    jsr uart_printz
    rts

rom_read_disk_handler:
    ; push arguments, right to left
    lda <frame_base+caller_y        ;   Y is number of blocks to read
    pha
    lda <frame_base+caller_a        ;   A is low 2 bytes of block number
    pha
    lda <frame_base+caller_x        ;   X is address to write to, use data bank register for addresses outside bank 0.
    pha
    jsr spi_sd_multiblock_read
    rts

spi_sd_multiblock_read:
    .a16
    .i16
    tsc
    sec
    sbc #L2
    tcs
    phd
    tcd
dest_address_0 := 3
block_number_low_1 := 5
block_count_2 := 7
@next_block:
    ldx <L2+block_count_2
    cpx #0
    beq L4
    ; Read one block of data
    ldx <L2+dest_address_0          ; destination address
    lda #0                          ; block number high
    ldy <L2+block_number_low_1      ; block number low
    jsr spi_sd_block_read           ; Read block to RAM
    dec <L2+block_count_2           ; Decrement number of blocks remaining
    lda <L2+dest_address_0          ; Wind dest address forward 512 bytes
    clc
    adc #512
    sta <L2+dest_address_0
    inc <L2+block_number_low_1      ; Wind block number forward by 1
    jmp @next_block
L4:
    lda <L2+1
    sta <L2+1+6
    pld
    tsc
    clc
    adc #L2+6
    tcs
    rts
L2	:=	10
L3	:=	1
