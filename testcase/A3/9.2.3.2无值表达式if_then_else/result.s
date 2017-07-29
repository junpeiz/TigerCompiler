main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L7:
addi $7, $0, 0
lw $8, 0($4)
sw $7, -4($8)
addi $11, $0, 1
add $5, $11, $0
lw $13, 0($4)
lw $12, -4($13)
addi $15, $0, 1
sub $16, $12, $15
bltz $16, L1
L2:
addi $18, $0, 0
add $5, $18, $0
L1:
addi $20, $0, 1
beq $5, $20, L3
L4:
addi $22, $0, 3
lw $23, 0($4)
sw $22, -4($23)
L5:
j L6
L3:
addi $26, $0, 2
lw $27, 0($4)
sw $26, -4($27)
j L5
L6:
