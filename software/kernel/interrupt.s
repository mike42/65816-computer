.import main

.segment "CODE"
reset:
  clc                             ; switch to native mode
  xce
  jmp main                        ; go to main method

nmi_interrupt:
	rti

irq_interrrupt:
  rti

cop_interrupt:
  rti

unused_interrupt:                 ; Probably make this into a crash.
  rti

.segment "VECTORS"
; native mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word cop_interrupt               ; COP
.word unused_interrupt            ; BRK
.word unused_interrupt            ; Abort
.word nmi_interrupt               ; NMI
.word unused_interrupt            ; Reserved
.word irq_interrrupt              ; IRQ

; emulation mode interrupt vectors
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; COP
.word unused_interrupt            ; Reserved
.word unused_interrupt            ; Abort
.word unused_interrupt            ; NMI
.word reset                       ; Reset
.word unused_interrupt            ; IRQ/BRK
