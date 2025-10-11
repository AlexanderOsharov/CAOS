# macros.s

.macro INPUT_DATA
    li a7, 7
    ecall
    fmv.d ft0, fa0

    li a7, 7
    ecall
    fmv.d fa1, fa0

    fmv.d fa0, ft0
.end_macro

.macro CHECK_INPUT
    li t0, 0
    fcvt.d.w ft1, t0

    fmv.d ft0, fa0
    flt.d t1, ft1, ft0
    beqz t1, check_fail

    fmv.d ft0, fa1
    flt.d t1, ft1, ft0
    beqz t1, check_fail
    j check_ok

check_fail:
    li a7, 4
    la a0, err_msg
    ecall
    li a7, 10
    ecall

check_ok:
.end_macro

.macro CALL_SQRT
    jal ra, sqrt_iter
.end_macro

.macro OUTPUT_RESULT
    li a7, 4
    la a0, res_msg
    ecall

    li a7, 3
    ecall

    li a7, 4
    la a0, newline
    ecall
.end_macro

.data
err_msg: .asciz "Ошибка: число и точность должны быть положительными!\n"
res_msg: .asciz "Корень квадратный: "
newline: .asciz "\n"