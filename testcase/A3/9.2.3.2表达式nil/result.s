main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L5:
lw $9, 0($5)
addi $9, $9, fffffffc
add $6, $9, $0
lw $14, 0($1)
addi $15, $0, 0
sw $15, 0($14)
subi $14, $14, 0
sw $14, 0($1)
call <Init Record>
lw $22, 0($1)
addi $22, $22, 0
sw $22, 0($1)
add $4, $<Init Record>, $0
sw $4, 0($6)
lw $26, 0($5)
lw $25, -4($26)
beq $25, $4, L1
L2:
j L4
L1:
addi $31, $0, 2
lw $33, 0($5)
lw $32, -4($33)
sw $31, 0($32)
j L2
L4:
