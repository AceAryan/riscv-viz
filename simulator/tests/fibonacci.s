# fibonacci: compute fib(8) = 21
addi x1, x0, 0     # prev = 0
addi x2, x0, 1     # curr = 1
addi x3, x0, 7     # iterate 7 more times to reach fib(8)

addi x4, x2, 0     # temp = curr
add  x2, x1, x2    # curr = prev + curr
addi x1, x4, 0     # prev = temp
addi x3, x3, -1    # counter--
bne  x3, x0, -16   # loop back