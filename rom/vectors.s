; vectors.s: Interrupt vectors for ROM
.import reset, cop_handler

.segment "CODE"
unused_interrupt:                   ; Probably make this into a crash.
    rti

.segment "VECTORS"
; native mode interrupt vectors
.word unused_interrupt              ; Reserved
.word unused_interrupt              ; Reserved
.word cop_handler                   ; COP
.word unused_interrupt              ; BRK
.word unused_interrupt              ; Abort
.word unused_interrupt              ; NMI
.word unused_interrupt              ; Reserved
.word unused_interrupt              ; IRQ

; emulation mode interrupt vectors
.word unused_interrupt              ; Reserved
.word unused_interrupt              ; Reserved
.word unused_interrupt              ; COP
.word unused_interrupt              ; Reserved
.word unused_interrupt              ; Abort
.word unused_interrupt              ; NMI
.word reset                         ; Reset
.word unused_interrupt              ; IRQ/BRK
