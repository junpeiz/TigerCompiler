main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
addi $5, $0, 0
lw $6, 0($4)
sw $5, -4($6)
addi $12, $0, 3
muli $12, $12, 2
addi $12, $12, 2
divi $12, $12, 4
addi $12, $12, 1
lw $13, 0($4)
sw $12, -4($13)
j L1
L1:
