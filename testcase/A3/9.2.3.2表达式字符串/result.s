L2: 1234
L1: 1234
main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L4:
lw $6, 0($4)
sw $L1, -4($6)
lw $9, 0($4)
sw $L2, -4($9)
j L3
L3:
