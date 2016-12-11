.data
a: .word 0:30 
$str0: .asciiz " "
.text
j $end
read:
subi $sp $sp 80
sw $ra 64($sp)
sw $fp 60($sp)
sw $14 56($sp)
sw $15 52($sp)
sw $16 48($sp)
sw $17 44($sp)
sw $18 40($sp)
sw $19 36($sp)
sw $20 32($sp)
sw $21 28($sp)
sw $22 24($sp)
sw $23 20($sp)
sw $24 16($sp)
sw $25 12($sp)
addi $fp $sp 80
li $v0 5
syscall
move $15 $v0
addi $14 $0 0
$labe2:
slt $17 $14 $15
beq $17 $0 $labe3
li $v0 5
syscall
move $16 $v0
sll $6 $14 2
sw $16 a($6)
addi $14 $14 1
j $labe2
$labe3:
move $v0 $15
lw $ra 64($sp)
lw $fp 60($sp)
lw $14 56($sp)
lw $15 52($sp)
lw $16 48($sp)
lw $17 44($sp)
lw $18 40($sp)
lw $19 36($sp)
lw $20 32($sp)
lw $21 28($sp)
lw $22 24($sp)
lw $23 20($sp)
lw $24 16($sp)
lw $25 12($sp)
addi $sp $sp 80
jr $ra
quick:
subi $sp $sp 176
sw $ra 64($sp)
sw $fp 60($sp)
sw $14 56($sp)
sw $15 52($sp)
sw $16 48($sp)
sw $17 44($sp)
sw $18 40($sp)
sw $19 36($sp)
sw $20 32($sp)
sw $21 28($sp)
sw $22 24($sp)
sw $23 20($sp)
sw $24 16($sp)
sw $25 12($sp)
addi $fp $sp 176
lw $16 0($fp)
lw $17 -4($fp)
slt $10 $16 $17
sw $10 -48($fp)
lw $8 -48($fp)
beq $8 $0 $labe4
move $15 $16
subi $10 $17 1
sw $10 -44($fp)
lw $8 -44($fp)
move $14 $8
$labe5:
$labe6:
addi $18 $0 0
sll $6 $16 2
lw $20 a($6)
sll $6 $15 2
lw $21 a($6)
sge $22 $20 $21
beq $22 $0 $labe7
slt $23 $15 $17
beq $23 $0 $labe8
addi $24 $15 1
move $15 $24
addi $18 $0 1
$labe8:
$labe7:
seq $25 $18 1
bne $25 $0 $labe6
$labe9:
addi $19 $0 0
sll $6 $16 2
lw $10 a($6)
sw $10 -8($fp)
sll $6 $14 2
lw $10 a($6)
sw $10 -12($fp)
lw $8 -8($fp)
lw $9 -12($fp)
sle $10 $8 $9
sw $10 -16($fp)
lw $8 -16($fp)
beq $8 $0 $labe10
sgt $10 $14 $16
sw $10 -20($fp)
lw $8 -20($fp)
beq $8 $0 $labe11
subi $10 $14 1
sw $10 -24($fp)
lw $8 -24($fp)
move $14 $8
addi $19 $0 1
$labe11:
$labe10:
seq $10 $19 1
sw $10 -28($fp)
lw $8 -28($fp)
bne $8 $0 $labe9
slt $10 $15 $14
sw $10 -32($fp)
lw $8 -32($fp)
beq $8 $0 $labe12
sll $6 $15 2
lw $10 a($6)
sw $10 -36($fp)
lw $8 -36($fp)
move $10 $8
sw $10 -68($fp)
sll $6 $14 2
lw $10 a($6)
sw $10 -60($fp)
lw $8 -60($fp)
sll $6 $15 2
sw $8 a($6)
lw $8 -68($fp)
sll $6 $14 2
sw $8 a($6)
$labe12:
slt $10 $15 $14
sw $10 -40($fp)
lw $8 -40($fp)
bne $8 $0 $labe5
sll $6 $16 2
lw $10 a($6)
sw $10 -52($fp)
lw $8 -52($fp)
move $10 $8
sw $10 -68($fp)
sll $6 $14 2
lw $10 a($6)
sw $10 -64($fp)
lw $8 -64($fp)
sll $6 $16 2
sw $8 a($6)
lw $8 -68($fp)
sll $6 $14 2
sw $8 a($6)
sw $16 0($sp)
sw $14 -4($sp)
jal quick
addi $10 $14 1
sw $10 -56($fp)
lw $8 -56($fp)
sw $8 0($sp)
sw $17 -4($sp)
jal quick
$labe4:
lw $ra 64($sp)
lw $fp 60($sp)
lw $14 56($sp)
lw $15 52($sp)
lw $16 48($sp)
lw $17 44($sp)
lw $18 40($sp)
lw $19 36($sp)
lw $20 32($sp)
lw $21 28($sp)
lw $22 24($sp)
lw $23 20($sp)
lw $24 16($sp)
lw $25 12($sp)
addi $sp $sp 176
jr $ra
print:
subi $sp $sp 80
sw $ra 64($sp)
sw $fp 60($sp)
sw $14 56($sp)
sw $15 52($sp)
sw $16 48($sp)
sw $17 44($sp)
sw $18 40($sp)
sw $19 36($sp)
sw $20 32($sp)
sw $21 28($sp)
sw $22 24($sp)
sw $23 20($sp)
sw $24 16($sp)
sw $25 12($sp)
addi $fp $sp 80
lw $15 0($fp)
addi $14 $0 0
$labe13:
slt $16 $14 $15
beq $16 $0 $labe14
li $v0 4
la $a0 $str0
syscall
sll $6 $14 2
lw $17 a($6)
li $v0 1
move $a0 $17
syscall
addi $14 $14 1
j $labe13
$labe14:
lw $ra 64($sp)
lw $fp 60($sp)
lw $14 56($sp)
lw $15 52($sp)
lw $16 48($sp)
lw $17 44($sp)
lw $18 40($sp)
lw $19 36($sp)
lw $20 32($sp)
lw $21 28($sp)
lw $22 24($sp)
lw $23 20($sp)
lw $24 16($sp)
lw $25 12($sp)
addi $sp $sp 80
jr $ra
main:
subi $sp $sp 72
sw $ra 64($sp)
sw $fp 60($sp)
sw $14 56($sp)
sw $15 52($sp)
sw $16 48($sp)
sw $17 44($sp)
sw $18 40($sp)
sw $19 36($sp)
sw $20 32($sp)
sw $21 28($sp)
sw $22 24($sp)
sw $23 20($sp)
sw $24 16($sp)
sw $25 12($sp)
addi $fp $sp 72
jal read
move $15 $2
move $14 $15
addi $13 $0 0
sw $13 0($sp)
sw $14 -4($sp)
jal quick
sw $14 0($sp)
jal print
lw $ra 64($sp)
lw $fp 60($sp)
lw $14 56($sp)
lw $15 52($sp)
lw $16 48($sp)
lw $17 44($sp)
lw $18 40($sp)
lw $19 36($sp)
lw $20 32($sp)
lw $21 28($sp)
lw $22 24($sp)
lw $23 20($sp)
lw $24 16($sp)
lw $25 12($sp)
addi $sp $sp 72
jr $ra
$end: jal main
