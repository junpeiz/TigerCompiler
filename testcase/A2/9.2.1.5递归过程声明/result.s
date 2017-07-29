BEGIN L1
L3:
lw $7, 0($1)
lw $10, 0($4)
lw $9, 4($10)
subi $9, $9, 1
sw $9, 4($7)
lw $12, 0($4)
sw $12, 0($7)
subi $7, $7, 4
sw $7, 0($1)
call L1
lw $20, 0($1)
addi $20, $20, 4
sw $20, 0($1)
j L2
L2:
END L1

main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L5:
addi $22, $0, 0
j L4
L4:
