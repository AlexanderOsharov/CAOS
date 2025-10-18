.eqv RIGHT_DIGIT 0xFFFF0010
.eqv LEFT_DIGIT  0xFFFF0011
.eqv DOT_BIT     0x80

.data
.align 4

sevseg_table:
    #.byte 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07
    #.byte 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71
    .byte 0x3F   # 0
    .byte 0x06   # 1
    .byte 0x5B   # 2
    .byte 0x4F   # 3
    .byte 0x66   # 4
    .byte 0x6D   # 5
    .byte 0x7D   # 6
    .byte 0x07   # 7
    .byte 0x7F   # 8
    .byte 0x6F   # 9
    .byte 0x77   # A
    .byte 0x7C   # b
    .byte 0x58   # C
    .byte 0x5E   # d
    .byte 0x79   # E
    .byte 0x71   # F

enter_show: .string "DEBUG: enter show_digit\n"
val_msg:    .string "DEBUG: value = "
addr_msg:   .string " DEBUG: addr = "
exit_show:  .string " DEBUG: exit show_digit\n"
nl:         .string "\n"

.text
