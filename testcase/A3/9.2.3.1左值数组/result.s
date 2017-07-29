main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
lw $9, 0($4)
addi $9, $9, fffffffc
add $6, $9, $0
lw $14, 0($1)
addi $15, $0, 0
sw $15, 4($14)
addi $16, $0, 3
sw $16, 0($14)
subi $14, $14, 4
sw $14, 0($1)
call <Init Array>
lw $23, 0($1)
addi $23, $23, 4
sw $23, 0($1)
add $5, $<Init Array>, $0
sw $5, 0($6)
j L1
L1:
