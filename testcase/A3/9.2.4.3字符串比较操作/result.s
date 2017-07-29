L2: world
L1: world
main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L4:
lw $8, 0($4)
sw $L1, -4($8)
lw $11, 0($4)
sw $L2, -8($11)
addi $13, $0, 0
lw $14, 0($4)
sw $13, -12($14)
lw $18, 0($4)
addi $18, $18, fffffff4
add $6, $18, $0
lw $23, 0($1)
lw $25, 0($4)
lw $24, -8($25)
sw $24, 4($23)
lw $28, 0($4)
lw $27, -4($28)
sw $27, 0($23)
subi $23, $23, 4
sw $23, 0($1)
call <Compare String>
lw $36, 0($1)
addi $36, $36, 4
sw $36, 0($1)
add $5, $<Compare String>, $0
sw $5, 0($6)
j L3
L3:
