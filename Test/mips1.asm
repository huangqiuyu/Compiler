.data
a: .word 0:100
str0: .asciiz " "
str1: .asciiz "Please enter n integers to sort:"
.text
j $end
sort:
move $fp $sp
subi $sp $sp 204
sw $ra 160($sp)
sw $fp 156($sp)
sw $5 152($sp)
sw $6 148($sp)
sw $7 144($sp)
sw $8 140($sp)
sw $9 136($sp)
sw $10 132($sp)
sw $11 128($sp)
sw $12 124($sp)
sw $13 120($sp)
sw $14 116($sp)
sw $15 112($sp)
sw $16 108($sp)
sw $17 104($sp)
sw $18 100($sp)
sw $19 96($sp)
sw $20 92($sp)
sw $21 88($sp)
sw $22 84($sp)
sw $23 80($sp)
sw $24 76($sp)
sw $25 72($sp)
lw $5 -4($fp)
addi $5 $0 0
$labe2:
lw $6 -16($fp)
lw $7 0($fp)
slt $6 $5 $7
beq $6 $0 $labe3
lw $8 -8($fp)
move $8 $5
$labe4:
lw $9 -20($fp)
slt $9 $8 $7
beq $9 $0 $labe5
lw $10 -24($fp)
addi $11 $0 0
sll $11 $5 2
lw $10 a($11)
lw $12 -28($fp)
addi $13 $0 0
sll $13 $8 2
lw $12 a($13)
lw $14 -32($fp)
sge $14 $10 $12
beq $14 $0 $labe6
lw $15 -36($fp)
addi $16 $0 0
sll $16 $5 2
lw $15 a($16)
lw $17 -12($fp)
move $17 $15
lw $18 -40($fp)
addi $19 $0 0
sll $19 $8 2
lw $18 a($19)
addi $20 $0 0
sll $20 $5 2
sw $18 a($20)
addi $21 $0 0
sll $21 $8 2
sw $17 a($21)
$labe6:
addi $8 $8 1
j $labe4
$labe5:
addi $5 $5 1
j $labe2
$labe3:
sw $5 -4($fp)
sw $6 0($fp)
sw $7 0($fp)
sw $8 -8($fp)
sw $9 -4($fp)
sw $10 -8($fp)
sw $12 -12($fp)
sw $14 -16($fp)
sw $15 -20($fp)
sw $17 -12($fp)
sw $18 -24($fp)
lw $ra 160($sp)
lw $fp 156($sp)
lw $5 152($sp)
lw $6 148($sp)
lw $7 144($sp)
lw $8 140($sp)
lw $9 136($sp)
lw $10 132($sp)
lw $11 128($sp)
lw $12 124($sp)
lw $13 120($sp)
lw $14 116($sp)
lw $15 112($sp)
lw $16 108($sp)
lw $17 104($sp)
lw $18 100($sp)
lw $19 96($sp)
lw $20 92($sp)
lw $21 88($sp)
lw $22 84($sp)
lw $23 80($sp)
lw $24 76($sp)
lw $25 72($sp)
addi $sp $sp 204
jr $ra
output:
move $fp $sp
subi $sp $sp 176
sw $ra 160($sp)
sw $fp 156($sp)
sw $5 152($sp)
sw $6 148($sp)
sw $7 144($sp)
sw $8 140($sp)
sw $9 136($sp)
sw $10 132($sp)
sw $11 128($sp)
sw $12 124($sp)
sw $13 120($sp)
sw $14 116($sp)
sw $15 112($sp)
sw $16 108($sp)
sw $17 104($sp)
sw $18 100($sp)
sw $19 96($sp)
sw $20 92($sp)
sw $21 88($sp)
sw $22 84($sp)
sw $23 80($sp)
sw $24 76($sp)
sw $25 72($sp)
lw $5 -4($fp)
addi $5 $0 0
$labe7:
lw $6 -8($fp)
lw $7 0($fp)
slt $6 $5 $7
beq $6 $0 $labe8
lw $8 -12($fp)
addi $9 $0 0
sll $9 $5 2
lw $8 a($9)
li $v0 1
move $a0 $8
syscall
li $v0 4
la $a0 str0
syscall
addi $5 $5 1
j $labe7
$labe8:
sw $5 -4($fp)
sw $6 -28($fp)
sw $7 0($fp)
sw $8 -32($fp)
lw $ra 160($sp)
lw $fp 156($sp)
lw $5 152($sp)
lw $6 148($sp)
lw $7 144($sp)
lw $8 140($sp)
lw $9 136($sp)
lw $10 132($sp)
lw $11 128($sp)
lw $12 124($sp)
lw $13 120($sp)
lw $14 116($sp)
lw $15 112($sp)
lw $16 108($sp)
lw $17 104($sp)
lw $18 100($sp)
lw $19 96($sp)
lw $20 92($sp)
lw $21 88($sp)
lw $22 84($sp)
lw $23 80($sp)
lw $24 76($sp)
lw $25 72($sp)
addi $sp $sp 176
jr $ra
main:
move $fp $sp
subi $sp $sp 176
sw $ra 160($sp)
sw $fp 156($sp)
sw $5 152($sp)
sw $6 148($sp)
sw $7 144($sp)
sw $8 140($sp)
sw $9 136($sp)
sw $10 132($sp)
sw $11 128($sp)
sw $12 124($sp)
sw $13 120($sp)
sw $14 116($sp)
sw $15 112($sp)
sw $16 108($sp)
sw $17 104($sp)
sw $18 100($sp)
sw $19 96($sp)
sw $20 92($sp)
sw $21 88($sp)
sw $22 84($sp)
sw $23 80($sp)
sw $24 76($sp)
sw $25 72($sp)
lw $5 -8($fp)
li $v0 5
syscall
move $5 $v0
li $v0 4
la $a0 str1
syscall
lw $6 0($fp)
addi $6 $0 0
$labe9:
lw $7 -12($fp)
slt $7 $6 $5
beq $7 $0 $labe10
lw $8 -4($fp)
li $v0 5
syscall
move $8 $v0
addi $9 $0 0
sll $9 $6 2
sw $8 a($9)
addi $6 $6 1
j $labe9
$labe10:
sw $5 0($sp)
jal sort
sw $5 0($sp)
jal output
sw $5 -8($fp)
sw $6 0($fp)
sw $7 -36($fp)
sw $8 -4($fp)
lw $ra 160($sp)
lw $fp 156($sp)
lw $5 152($sp)
lw $6 148($sp)
lw $7 144($sp)
lw $8 140($sp)
lw $9 136($sp)
lw $10 132($sp)
lw $11 128($sp)
lw $12 124($sp)
lw $13 120($sp)
lw $14 116($sp)
lw $15 112($sp)
lw $16 108($sp)
lw $17 104($sp)
lw $18 100($sp)
lw $19 96($sp)
lw $20 92($sp)
lw $21 88($sp)
lw $22 84($sp)
lw $23 80($sp)
lw $24 76($sp)
lw $25 72($sp)
addi $sp $sp 176
jr $ra
$end: jal main