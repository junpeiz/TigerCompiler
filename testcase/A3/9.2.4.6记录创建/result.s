main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L2:
lw $9, 0($5)
addi $9, $9, fffffffc
add $6, $9, $0
lw $14, 0($1)
addi $15, $0, 8
sw $15, 0($14)
subi $14, $14, 0
sw $14, 0($1)
call <Init Record>
lw $22, 0($1)
addi $22, $22, 0
sw $22, 0($1)
add $4, $<Init Record>, $0
addi $23, $0, 1
sw $23, 0($4)
addi $25, $0, 2
sw $25, 4($4)
sw $4, 0($6)
j L1
L1:
