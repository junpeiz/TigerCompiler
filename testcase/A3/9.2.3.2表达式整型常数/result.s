main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
addi $5, $0, 0
lw $6, 0($4)
sw $5, -4($6)
addi $8, $0, 123
lw $9, 0($4)
sw $8, -4($9)
j L1
L1:
