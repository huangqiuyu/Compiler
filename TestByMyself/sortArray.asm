.data
a: .word 0:100 
$str0: .asciiz " "
$str1: .asciiz "Please enter n integers to sort:"
.text
j $end
sort:
subi $sp $sp 108
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
addi $fp $sp 108
lw $16 0($fp)
addi $15 $0 0
$labe2:
slt $22 $15 $16
beq $22 $0 $labe3
move $14 $15
$labe4:
slt $17 $14 $16
beq $17 $0 $labe5
sll $6 $15 2
lw $18 a($6)
sll $6 $14 2
lw $19 a($6)
sge $20 $18 $19
beq $20 $0 $labe6
sll $6 $15 2
lw $21 a($6)
move $23 $21
sll $6 $14 2
lw $24 a($6)
sll $6 $15 2
sw $24 a($6)
sll $6 $14 2
sw $23 a($6)
$labe6:
addi $14 $14 1
j $labe4
$labe5:
addi $15 $15 1
j $labe2
$labe3:
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
addi $sp $sp 108
jr $ra
output:
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
$labe7:
slt $16 $14 $15
beq $16 $0 $labe8
sll $6 $14 2
lw $17 a($6)
li $v0 1
move $a0 $17
syscall
li $v0 4
la $a0 $str0
syscall
addi $14 $14 1
j $labe7
$labe8:
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
li $v0 4
la $a0 $str1
syscall
addi $14 $0 0
$labe9:
slt $17 $14 $15
beq $17 $0 $labe10
li $v0 5
syscall
move $16 $v0
sll $6 $14 2
sw $16 a($6)
addi $14 $14 1
j $labe9
$labe10:
sw $15 0($sp)
jal sort
sw $15 0($sp)
jal output
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
$end: jal main
