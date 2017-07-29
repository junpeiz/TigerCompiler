main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L7:
addi $6, $0, 5
lw $7, 0($4)
sw $6, -4($7)
L2:
addi $10, $0, 1
add $5, $10, $0
lw $12, 0($4)
lw $11, -4($12)
addi $14, $0, 0
bne $11, $14, L4
L5:
addi $17, $0, 0
add $5, $17, $0
L4:
addi $19, $0, 0
beq $5, $19, L1
L3:
lw $23, 0($4)
lw $22, -4($23)
subi $22, $22, 1
j L2
L1:
j L6
L6:
