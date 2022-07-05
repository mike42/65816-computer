ROM_PRINT_STRING := $02

.segment "CODE"
    .a16
    .i16
    ldx #test_string_1
    cop ROM_PRINT_STRING
    ldx #test_string_2
    cop ROM_PRINT_STRING
    stp

test_string_1: .asciiz "Test 1\r\n"
test_string_2: .asciiz "Test 2\r\n"

.segment "SIGNATURE"
    wdm $42                         ; Ensure x86 systems don't recognise this as bootable.
