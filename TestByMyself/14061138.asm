.data
a1: .word 0 
a2: .word 0 
a: .word 0 
f: .word 0:5 
d1: .word 0 
d2: .word 0 
d: .word 0 
g: .word 0:6 
$str0: .asciiz "i=0"
$str1: .asciiz "i!=0"
$str2: .asciiz "j!=0"
.text
j $end
f1:
subi $sp $sp 76
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
addi $fp $sp 76
lw $14 0($fp)
lw $16 -4($fp)
addi $15 $14 1
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
addi $sp $sp 76
jr $ra
f2:
subi $sp $sp 96
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
addi $fp $sp 96
lw $15 0($fp)
lw $16 -4($fp)
addi $17 $0 0
subi $14 $0 12
move $18 $14
addi $19 $0 320
addi $15 $0 45
addi $16 $0 50
addi $20 $0 47
addi $21 $0 0
addi $v0 $0 97
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
addi $sp $sp 96
jr $ra
recursion:
subi $sp $sp 84
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
addi $fp $sp 84
lw $14 0($fp)
seq $15 $14 1
beq $15 $0 $labe2
addi $v0 $0 1
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
addi $sp $sp 84
jr $ra
j $labe3
$labe2:
subi $16 $14 1
sw $16 0($sp)
jal recursion
move $17 $2
add $18 $14 $17
move $v0 $18
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
addi $sp $sp 84
jr $ra
$labe3:
f3:
subi $sp $sp 180
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
addi $fp $sp 180
addi $5 $0 0
addi $6 $5 0
sub $6 $fp $6
sw $0 0($6)
addi $5 $0 0
addi $6 $5 0
sub $6 $fp $6
lw $19 0($6)
addi $10 $19 5
sw $10 -64($fp)
addi $5 $0 0
lw $8 -64($fp)
addi $6 $5 4
sub $6 $fp $6
sw $8 0($6)
addi $5 $0 8
addi $6 $5 0
sub $6 $fp $6
sw $0 0($6)
addi $5 $0 8
addi $6 $5 4
sub $6 $fp $6
sw $0 0($6)
addi $14 $0 1
addi $16 $0 2
addi $18 $0 -3
addi $5 $0 8
addi $6 $5 4
sub $6 $fp $6
lw $20 0($6)
move $15 $20
add $21 $14 $16
sub $22 $18 $16
mul $23 $21 $22
addi $5 $0 0
addi $6 $5 0
sub $6 $fp $6
lw $24 0($6)
addi $5 $0 0
addi $6 $5 4
sub $6 $fp $6
lw $25 0($6)
sub $10 $24 $25
sw $10 -16($fp)
lw $9 -16($fp)
sub $10 $23 $9
sw $10 -20($fp)
addi $5 $0 8
addi $6 $5 0
sub $6 $fp $6
lw $10 0($6)
sw $10 -24($fp)
lw $8 -24($fp)
add $10 $8 $16
sw $10 -28($fp)
lw $8 -28($fp)
add $10 $8 $15
sw $10 -32($fp)
lw $8 -20($fp)
lw $9 -32($fp)
div $10 $8 $9
sw $10 -36($fp)
lw $8 -36($fp)
sub $10 $0 $8
sw $10 -40($fp)
lw $8 -40($fp)
mul $10 $8 $14
sw $10 -44($fp)
lw $8 -44($fp)
sub $10 $0 $8
sw $10 -48($fp)
div $10 $15 -3
sw $10 -52($fp)
lw $8 -48($fp)
lw $9 -52($fp)
sub $10 $8 $9
sw $10 -56($fp)
lw $8 -56($fp)
move $17 $8
li $v0 1
move $a0 $17
syscall
sw $14 0($sp)
sw $15 -4($sp)
jal f1
move $10 $2
sw $10 -60($fp)
lw $8 -60($fp)
move $18 $8
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
addi $sp $sp 180
jr $ra
main:
subi $sp $sp 140
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
addi $fp $sp 140
li $v0 5
syscall
move $14 $v0
li $v0 5
syscall
move $15 $v0
addi $21 $0 97
seq $22 $14 0
beq $22 $0 $labe4
li $v0 4
la $a0 $str0
syscall
j $labe5
$labe4:
li $v0 4
la $a0 $str1
syscall
$labe5:
beq $15 $0 $labe6
li $v0 4
la $a0 $str2
syscall
$labe6:
addi $14 $0 2
addi $15 $0 0
$labe7:
subi $16 $14 1
move $14 $16
addi $5 $0 0
sll $6 $15 2
add $6 $5 $6
sub $6 $fp $6
sw $14 0($6)
addi $17 $15 1
move $15 $17
sgt $18 $14 0
bne $18 $0 $labe7
li $v0 5
syscall
move $15 $v0
addi $14 $0 0
$labe8:
slt $19 $14 $15
beq $19 $0 $labe9
add $20 $14 $15
li $v0 1
move $a0 $20
syscall
addi $14 $14 1
j $labe8
$labe9:
add $23 $15 $14
sw $23 0($sp)
addi $13 $0 97
sw $13 -4($sp)
jal f1
move $24 $2
move $14 $24
li $v0 1
move $a0 $14
syscall
sw $14 0($sp)
addi $13 $0 99
sw $13 -4($sp)
jal f2
move $25 $2
move $21 $25
li $v0 11
move $a0 $21
syscall
jal f3
addi $13 $0 1
sw $13 0($sp)
jal recursion
move $10 $2
sw $10 -20($fp)
lw $8 -20($fp)
move $14 $8
li $v0 1
move $a0 $14
syscall
addi $13 $0 3
sw $13 0($sp)
jal recursion
move $10 $2
sw $10 -24($fp)
lw $8 -24($fp)
move $14 $8
li $v0 1
move $a0 $14
syscall
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
addi $sp $sp 140
jr $ra
$end: jal main
