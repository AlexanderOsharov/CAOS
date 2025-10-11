.include "macros.s"

.text
.globl main

main:
    INPUT_DATA
    CHECK_INPUT
    CALL_SQRT
    OUTPUT_RESULT

    li a7, 10
    ecall