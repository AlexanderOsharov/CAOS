.text
.globl sqrt_iter

sqrt_iter:
    fmv.d ft8, fa0
    fmv.d ft9, fa1

    fmv.d ft0, fa0

    li t0, 2
    fcvt.d.w ft3, t0

loop:
    fdiv.d ft1, ft8, ft0
    fadd.d ft2, ft0, ft1
    fdiv.d ft2, ft2, ft3

    fsub.d ft4, ft2, ft0
    fabs.d ft4, ft4

    flt.d t1, ft4, ft9
    bnez t1, done

    fmv.d ft0, ft2
    j loop

done:
    fmv.d fa0, ft2
    jr ra