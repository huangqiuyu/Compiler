.data
j: .word 0 
n: .word 0 
a: .word 0:30 
$str0: .asciiz " do-while begin: "
$str1: .asciiz " result is: "
$str2: .asciiz "  "
$str3: .asciiz " for loop begin: "
$str4: .asciiz "  "
$str5: .asciiz " mixtra loop begin: "
$str6: .asciiz " before: "
$str7: .asciiz "  "
$str8: .asciiz " after: "
$str9: .asciiz "  "
.text
j $end
dowhile:
subi $sp $sp 236
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
addi $fp $sp 236
li $v0 4
la $a0 $str0
syscall
li $v0 5
syscall
move $10 $v0
sw $10 n($0)
li $v0 5
syscall
move $15 $v0
addi $14 $0 0
$labe2:
addi $5 $0 0
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
sw $14 0($6)
addi $21 $14 1
move $14 $21
slti $20 $14 30
bne $20 $0 $labe2
li $v0 4
la $a0 $str1
syscall
$labe3:
addi $14 $0 0
$labe4:
addi $5 $0 0
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
lw $16 0($6)
add $10 $16 $15
sw $10 -120($fp)
addi $5 $0 0
lw $8 -120($fp)
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
sw $8 0($6)
addi $5 $0 0
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
lw $17 0($6)
li $v0 1
move $a0 $17
syscall
li $v0 4
la $a0 $str2
syscall
addi $18 $14 1
move $14 $18
slti $19 $14 30
bne $19 $0 $labe4
addi $22 $15 1
move $15 $22
lw $9 n($0)
slt $23 $15 $9
bne $23 $0 $labe3
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
addi $sp $sp 236
jr $ra
forfun:
subi $sp $sp 224
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
addi $fp $sp 224
li $v0 4
la $a0 $str3
syscall
li $v0 5
syscall
move $18 $v0
addi $15 $0 0
$labe5:
slti $19 $15 30
beq $19 $0 $labe6
addi $5 $0 0
sll $6 $15 2
add $6 $5 $6
sub $6 $fp $6
sw $15 0($6)
addi $15 $15 1
j $labe5
$labe6:
addi $15 $0 0
$labe7:
slt $20 $15 $18
beq $20 $0 $labe8
addi $14 $0 0
$labe9:
slti $16 $14 30
beq $16 $0 $labe10
addi $5 $0 0
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
lw $17 0($6)
add $23 $17 $15
addi $5 $0 0
sll $6 $14 2
add $6 $5 $6
sub $6 $fp $6
sw $23 0($6)
addi $14 $14 1
j $labe9
$labe10:
addi $15 $15 1
j $labe7
$labe8:
addi $15 $0 0
$labe11:
slti $21 $15 30
beq $21 $0 $labe12
addi $5 $0 0
sll $6 $15 2
add $6 $5 $6
sub $6 $fp $6
lw $22 0($6)
li $v0 1
move $a0 $22
syscall
li $v0 4
la $a0 $str4
syscall
addi $15 $15 1
j $labe11
$labe12:
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
addi $sp $sp 224
jr $ra
fun:
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
addi $15 $0 10
add $16 $14 $15
move $v0 $16
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
loop:
subi $sp $sp 152
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
addi $fp $sp 152
li $v0 4
la $a0 $str5
syscall
addi $21 $0 0
$labe13:
slti $10 $21 30
sw $10 -4($fp)
lw $8 -4($fp)
beq $8 $0 $labe14
sll $6 $21 2
sw $21 a($6)
addi $15 $0 0
$labe15:
sll $6 $21 2
lw $23 a($6)
sw $23 0($sp)
jal fun
move $24 $2
add $10 $24 $15
sw $10 -36($fp)
lw $8 -36($fp)
sll $6 $21 2
sw $8 a($6)
addi $25 $15 1
move $15 $25
slti $10 $15 30
sw $10 0($fp)
lw $8 0($fp)
bne $8 $0 $labe15
addi $21 $21 1
j $labe13
$labe14:
li $v0 4
la $a0 $str6
syscall
addi $21 $0 0
$labe16:
slti $10 $21 30
sw $10 -8($fp)
lw $8 -8($fp)
beq $8 $0 $labe17
sll $6 $21 2
lw $10 a($6)
sw $10 -12($fp)
lw $8 -12($fp)
li $v0 1
move $a0 $8
syscall
li $v0 4
la $a0 $str7
syscall
addi $21 $21 1
j $labe16
$labe17:
addi $21 $0 0
$labe18:
slti $10 $21 30
sw $10 -16($fp)
lw $8 -16($fp)
beq $8 $0 $labe19
move $15 $21
$labe20:
slti $22 $15 30
beq $22 $0 $labe21
addi $14 $0 0
$labe22:
sll $6 $14 2
lw $16 a($6)
add $17 $16 $15
sw $14 0($sp)
jal fun
move $18 $2
add $10 $17 $18
sw $10 -32($fp)
lw $8 -32($fp)
sll $6 $14 2
sw $8 a($6)
addi $19 $14 1
move $14 $19
slti $20 $14 30
bne $20 $0 $labe22
addi $15 $15 1
j $labe20
$labe21:
addi $21 $21 1
j $labe18
$labe19:
li $v0 4
la $a0 $str8
syscall
addi $21 $0 0
$labe23:
sll $6 $21 2
lw $10 a($6)
sw $10 -20($fp)
lw $8 -20($fp)
li $v0 1
move $a0 $8
syscall
li $v0 4
la $a0 $str9
syscall
addi $10 $21 1
sw $10 -24($fp)
lw $8 -24($fp)
move $21 $8
slti $10 $21 30
sw $10 -28($fp)
lw $8 -28($fp)
bne $8 $0 $labe23
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
addi $sp $sp 152
jr $ra
main:
subi $sp $sp 64
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
addi $fp $sp 64
jal dowhile
jal forfun
jal loop
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
addi $sp $sp 64
jr $ra
$end: jal main
