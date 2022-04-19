.export panic

.segment "CODE"
; panic: Dump registers to console and halt CPU.
panic:
  php                             ; Save processor status
  .a16
  .i16
  rep #%00110000
  ; Preserve registers to stack
  pha                             ; Push A, X, Y
  phx
  phy
  phb                             ; Push data bank, program bank
  phk
  phd                             ; push direct page register
  tsc
  pha                             ; push stack pointer as well - does not match exact value from when panic was called
  tsc                             ; set direct register to stack pointer
  tcd
  ; Set up 16 bit values for printing
  lda $0d
  and #$ff
  pha       ; P
  lda $03
  pha       ; D
  lda $01
  pha       ; S
  lda $05
  and #$ff
  pha       ; PBR
  lda $06
  and #$ff
  pha       ; DBR

  lda $07   ; Y
  pha
  lda $09   ; X
  pha
  lda $0b   ; A
  pha
  lda $0e   ; PC
  pha
  ; Print the panic message
  ldx #panic_message
  jsr kprintf
  lda #$ff                        ; this will be the exit code of the emulator process
  stp

panic_message: .asciiz "\n** kernel panic at PC = $%u **\nA = $%u, X = $%u, Y = $%u\nDBR = $%u, PBR = $%u\nS = $%u, D = $%u, P = $%u\n"

; kprintf: print a formatted string, in a style similar to printf. Contains dodgy stack usage, intentionally not exported.
; X - pointer to format string
; Stack - a 16 bit value will be read from the stack and formatted each time %c %s %x or %u is encountered
kprintf:
  ldy #0                    ; Y is used by some code paths, so always clobber it
  .a8                       ; Use 8-bit accumulator
  sep #%00100000
@kprintf_next:
  lda a:$0000, X            ; a: to avoid this being interpreted as direct page
  beq @kprintf_done         ; stop on null
  inx
  cmp #'%'                  ; break on special % char
  beq @kprintf_check_specifier
  jsr _kprintchar
  jmp @kprintf_next
@kprintf_check_specifier:   ; handle % char by reading format specifier
  lda a:$0000, X
  beq @kprintf_done         ; end of string..
  inx
  cmp #'%'                  ; %%
  bne @check_char
  lda #'%'
  jsr _kprintchar
  jmp @kprintf_next
@check_char:
  cmp #'c'                  ; %c
  bne @check_string
  jmp @kprintf_char
@check_string:
  cmp #'s'                  ; %s
  bne @check_hex
  jmp @kprintf_string
@check_hex:
  cmp #'x'                  ; %x
  bne @check_unsigned_dec
  jmp @kprintf_hex
@check_unsigned_dec:
  cmp #'u'                  ; %u
  bne @bad_format
  jmp @kprintf_unsigned_dec
@bad_format:                ; This is an error
  lda #'?'
@kprintf_done:
  .a16                      ; Revert to 16-bit accumulator
  rep #%00100000
  rts

@kprintf_char:              ; print one character %c
  .a16
  rep #%00100000
  ply                       ; return address
  pla                       ; A is char to print
  phy                       ; put return address back
  .a8
  sep #%00100000
  jsr _kprintchar
  jmp @kprintf_next

@kprintf_string:            ; print null-terminated string %s
  .a16
  rep #%00100000
  pla                       ; return address
  ply
  pha                       ; put return address back
  .a8
  sep #%00100000
@string_next_char:
  lda a:$0000, Y
  beq @string_done
  iny
  jsr _kprintchar
  jmp @string_next_char
@string_done:
  jmp @kprintf_next

@kprintf_hex:               ; print a hex value %x
  .a16
  rep #%00100000
  ply                       ; return address
  pla                       ; A is char to print
  phy                       ; put return address back
  phx
  pha                       ; save A for later
  xba
  and #$00ff
  cmp #0                    ; skip first byte if 0
  beq @kprintf_hex_second_byte
  jsr @kprintf_hex_byte     ; print most significant byte.
@kprintf_hex_second_byte:
  pla
  and #$00ff
  jsr @kprintf_hex_byte     ; print least significant byte
  .a8                      ; switch back to 8-bit accumulator
  sep #%00100000
  plx
  jmp @kprintf_next
@kprintf_hex_byte:
  .a16                     ; accumulator is 16-bit when called
  .a8                      ; use 8-bit accumulator
  sep #%00100000
  pha                      ; store byte for later
  lsr                      ; shift out lower nibble
  lsr
  lsr
  lsr
  tax
  lda hex_chars, X            ; convert 0-15 to ascii char for hex digit
  jsr _kprintchar
  pla
  and #$0f
  tax
  lda hex_chars, X
  jsr _kprintchar
  .a16                     ; Revert to 16-bit accumulator
  rep #%00100000
  rts

@kprintf_unsigned_dec:     ; TODO not supported
  jmp @kprintf_hex

_kprintchar:
  .a8
  sta $c200
  rts

hex_chars: .byte '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
