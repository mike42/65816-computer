.export test_wdcc_functions, __main, __forty_two

.segment "CODE"
test_wdcc_functions:
  .a16                            ; use 16-bit accumulator and index registers
  .i16
  rep #%00110000
  jsr __do_nothing
  rts

R0  :=  1
R1  :=  5
R2  :=  9
R3  :=  13
__do_nothing:
  .a16
  .i16
  tsc
  sec
  sbc  #L2
  tcs
  phd
  tcd
L4:
  pld
  tsc
  clc
  adc  #L2
  tcs
  rts
L2  :=  0
L3  :=  1

__do_nothing_about:
  .a16
  .i16
  tsc
  sec
  sbc  #L5
  tcs
  phd
  tcd
something_0  :=  3
L7:
  lda  <L5+1
  sta  <L5+1+2
  pld
  tsc
  clc
  adc  #L5+2
  tcs
  rts
L5  :=  0
L6  :=  1

__forty_two:
  .a16
  .i16
  tsc
  sec
  sbc  #L8
  tcs
  phd
  tcd
  lda  #$2a
L10:
  tay
  pld
  tsc
  clc
  adc  #L8
  tcs
  tya
  rts
L8  :=  0
L9  :=  1

__times_two:
  .a16
  .i16
  tsc
  sec
  sbc  #L11
  tcs
  phd
  tcd
inp_0  :=  3
  clc
  lda  <L11+inp_0
  adc  <L11+inp_0
L13:
  tay
  lda  <L11+1
  sta  <L11+1+2
  pld
  tsc
  clc
  adc  #L11+2
  tcs
  tya
  rts
L11  :=  0
L12  :=  1

__main:
  .a16
  .i16
  tsc
  sec
  sbc  #L14
  tcs
  phd
  tcd
the_number_1  :=  0
twice_the_number_1  :=  2
  jsr  __do_nothing
  pea  a:$5
  jsr  __do_nothing_about
  jsr  __forty_two
  sta  <L15+the_number_1
  pei  (<L15+the_number_1)
  jsr  __times_two
  sta  <L15+twice_the_number_1
L16:
  pld
  tsc
  clc
  adc  #L14
  tcs
  rts
L14  :=  4
L15  :=  1
