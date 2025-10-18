.text
.globl show_digit
.include "macros.asm"

show_digit:
    addi sp, sp, -32
    sw ra, 28(sp)
    sw s0, 24(sp)
    sw s1, 20(sp)
    mv s0, a0
    mv s1, a1

    # --- ЛОГ: enter ---
    la a0, enter_show
    li a7, 4
    ecall

    la a0, val_msg
    li a7, 4
    ecall

    mv a0, s0
    li a7, 1
    ecall

    la a0, addr_msg
    li a7, 4
    ecall

    mv a0, s1
    li a7, 1
    ecall

    la a0, nl
    li a7, 4
    ecall

    # --- Основная логика ---
    li t0, 16
    blt s0, t0, no_dot
    li t1, 0x80
    j got_index
no_dot:
    li t1, 0
got_index:
    andi t2, s0, 0xF
    la t3, sevseg_table
    add t3, t3, t2
    lb t4, 0(t3)
    or t4, t4, t1
    sb t4, 0(s1)

    # --- ЛОГ: exit ---
    la a0, exit_show
    li a7, 4
    ecall

    lw s1, 20(sp)
    lw s0, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32
    ret