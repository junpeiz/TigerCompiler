main:
lui  $4, 0x7fff
ori $4, $4, 0xf4c0
sw $4, 0($4)
L14:
addi $17, $0, 0
lw $18, 0($4)
sw $17, -4($18)
lw $22, 0($4)
addi $22, $22, fffffffc
add $16, $22, $0
addi $25, $0, 1
add $5, $25, $0
addi $26, $0, 1
addi $27, $0, 2
beq $26, $27, L1
L2:
addi $30, $0, 0
add $5, $30, $0
L1:
sw $5, 0($16)
lw $35, 0($4)
addi $35, $35, fffffffc
add $15, $35, $0
addi $38, $0, 1
add $6, $38, $0
addi $39, $0, 1
addi $40, $0, 2
bne $39, $40, L3
L4:
addi $43, $0, 0
add $6, $43, $0
L3:
sw $6, 0($15)
lw $48, 0($4)
addi $48, $48, fffffffc
add $14, $48, $0
addi $51, $0, 1
add $7, $51, $0
addi $52, $0, 1
addi $53, $0, 2
sub $54, $52, $53
bltz $54, L5
L6:
addi $56, $0, 0
add $7, $56, $0
L5:
sw $7, 0($14)
lw $61, 0($4)
addi $61, $61, fffffffc
add $13, $61, $0
addi $64, $0, 1
add $8, $64, $0
addi $65, $0, 1
addi $66, $0, 2
sub $67, $65, $66
bgtz $67, L7
L8:
addi $69, $0, 0
add $8, $69, $0
L7:
sw $8, 0($13)
lw $74, 0($4)
addi $74, $74, fffffffc
add $12, $74, $0
addi $77, $0, 1
add $9, $77, $0
addi $78, $0, 1
addi $79, $0, 2
sub $80, $78, $79
bgez $80, L9
L10:
addi $82, $0, 0
add $9, $82, $0
L9:
sw $9, 0($12)
lw $87, 0($4)
addi $87, $87, fffffffc
add $11, $87, $0
addi $90, $0, 1
add $10, $90, $0
addi $91, $0, 1
addi $92, $0, 2
sub $93, $91, $92
blez $93, L11
L12:
addi $95, $0, 0
add $10, $95, $0
L11:
sw $10, 0($11)
j L13
L13:
