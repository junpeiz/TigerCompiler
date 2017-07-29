main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
addi $5, $0, 3
lw $6, 0($4)
sw $5, -4($6)
lw $10, 0($4)
lw $9, -4($10)
addi $9, $9, 1
lw $12, 0($4)
sw $9, -4($12)
j L1
L1:
