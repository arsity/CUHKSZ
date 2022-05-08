.text
fibonacci:
addi $sp, $sp, -12  #0
sw $ra, 8($sp)      
sw $s0, 4($sp)      #8
sw $s1, 0($sp)
add $s0, $a0, $zero    #0
addi $v0, $zero, 1
slti $t7, $s0, 2       #8
bne $t7, $zero, fibonacciExit
addi $a0, $s0, -1      #0
jal fibonacci
add $s1, $zero, $v0    #8
addi $a0, $s0, -2 
jal fibonacci         #0
add $v0, $s1, $v0
fibonacciExit:          #8
lw $ra, 8($sp)
lw $s0, 4($sp)
lw $s1, 0($sp)
addi $sp, $sp, 12
jr $ra
