BEGIN L1
L3:
lw $7, 0($4)
lw $6, 8($7)
lw $10, 0($4)
lw $9, 4($10)
add $6, $9, $6
lw $12, 0($4)
sw $6, 4($12)
j L2
L2:
END L1

main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L5:
lw $17, 0($1)
addi $18, $0, 2
sw $18, 8($17)
addi $19, $0, 3
sw $19, 4($17)
sw $4, 0($17)
subi $17, $17, 8
sw $17, 0($1)
call L1
lw $27, 0($1)
addi $27, $27, 8
sw $27, 0($1)
j L4
L4:
