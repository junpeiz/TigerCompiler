main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
addi $4, $0, 0
j L1
L1:
