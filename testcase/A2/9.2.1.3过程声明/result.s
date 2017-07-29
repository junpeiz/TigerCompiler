BEGIN L1
L3:
lw $7, 0($4)
lw $6, 4($7)
addi $6, $6, 1
lw $9, 0($4)
sw $6, 4($9)
j L2
L2:
END L1

main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L5:
addi $12, $0, 0
j L4
L4:
