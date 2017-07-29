L1: string
main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L3:
addi $5, $0, 1
lw $6, 0($4)
sw $5, -4($6)
lw $9, 0($4)
sw $L1, -8($9)
j L2
L2:
