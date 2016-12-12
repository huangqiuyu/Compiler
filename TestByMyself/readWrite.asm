.data
$str0: .asciiz " please input one number:"
$str1: .asciiz "  the number you input is:"
$str2: .asciiz "  please input one character:"
$str3: .asciiz "  the character you input is:"
$str4: .asciiz "  please input one number and one character:"
$str5: .asciiz "  the number you input is:"
$str6: .asciiz "  the character you input is:"
$str7: .asciiz "  please input one character and one number:"
$str8: .asciiz "  the character you input is:"
$str9: .asciiz "  the number you input is:"
$str10: .asciiz "  please input two character and two number:"
$str11: .asciiz "  the sum of your input numbers "
$str12: .asciiz "  "
$str13: .asciiz "  calculate four numbers "
$str14: .asciiz "  the const c is: "
.text
j $end
read:
subi $sp $sp 304
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
addi $fp $sp 304
li $v0 1
addi $a0 $0 14061176
syscall
li $v0 11
addi $a0 $0 97
syscall
li $v0 11
addi $a0 $0 98
syscall
li $v0 11
addi $a0 $0 99
syscall
li $v0 1
addi $a0 $0 403
syscall
li $v0 4
la $a0 $str0
syscall
li $v0 5
syscall
move $15 $v0
li $v0 4
la $a0 $str1
syscall
li $v0 1
move $a0 $15
syscall
li $v0 4
la $a0 $str2
syscall
li $v0 12
syscall
move $17 $v0
li $v0 4
la $a0 $str3
syscall
li $v0 11
move $a0 $17
syscall
li $v0 4
la $a0 $str4
syscall
li $v0 5
syscall
move $14 $v0
li $v0 12
syscall
move $16 $v0
li $v0 4
la $a0 $str5
syscall
li $v0 1
move $a0 $14
syscall
li $v0 4
la $a0 $str6
syscall
li $v0 11
move $a0 $16
syscall
li $v0 4
la $a0 $str7
syscall
li $v0 12
syscall
move $16 $v0
li $v0 5
syscall
move $14 $v0
li $v0 4
la $a0 $str8
syscall
li $v0 11
move $a0 $16
syscall
li $v0 4
la $a0 $str9
syscall
li $v0 1
move $a0 $14
syscall
li $v0 4
la $a0 $str10
syscall
li $v0 5
syscall
move $15 $v0
li $v0 5
syscall
move $14 $v0
li $v0 12
syscall
move $17 $v0
li $v0 12
syscall
move $16 $v0
li $v0 1
move $a0 $15
syscall
li $v0 1
move $a0 $14
syscall
li $v0 11
move $a0 $17
syscall
li $v0 11
move $a0 $16
syscall
li $v0 4
la $a0 $str11
syscall
li $v0 5
syscall
move $15 $v0
li $v0 5
syscall
move $14 $v0
add $18 $15 $14
li $v0 1
move $a0 $18
syscall
li $v0 4
la $a0 $str12
syscall
add $19 $15 $14
mul $20 $15 $14
add $21 $19 $20
mul $22 $15 $15
add $23 $21 $22
mul $24 $14 $14
add $25 $23 $24
addi $10 $25 1
sw $10 0($fp)
lw $8 0($fp)
addi $10 $8 10
sw $10 -4($fp)
lw $8 -4($fp)
subi $10 $8 13
sw $10 -8($fp)
div $10 $14 2
sw $10 -12($fp)
lw $8 -8($fp)
lw $9 -12($fp)
sub $10 $8 $9
sw $10 -16($fp)
div $10 $15 2
sw $10 -20($fp)
lw $8 -16($fp)
lw $9 -20($fp)
sub $10 $8 $9
sw $10 -24($fp)
mul $10 $14 $14
sw $10 -28($fp)
lw $8 -28($fp)
mul $10 $8 $15
sw $10 -32($fp)
lw $8 -32($fp)
mul $10 $8 $14
sw $10 -36($fp)
lw $8 -36($fp)
div $10 $8 2
sw $10 -40($fp)
lw $8 -24($fp)
lw $9 -40($fp)
add $10 $8 $9
sw $10 -44($fp)
lw $8 -44($fp)
li $v0 1
move $a0 $8
syscall
li $v0 4
la $a0 $str13
syscall
addi $10 $0 220
sw $10 -48($fp)
lw $8 -48($fp)
mul $10 $8 $14
sw $10 -52($fp)
lw $8 -52($fp)
div $10 $8 2
sw $10 -56($fp)
lw $9 -56($fp)
add $10 $15 $9
sw $10 -60($fp)
lw $8 -60($fp)
sub $10 $8 $14
sw $10 -64($fp)
mul $10 $15 $14
sw $10 -68($fp)
lw $8 -64($fp)
lw $9 -68($fp)
sub $10 $8 $9
sw $10 -72($fp)
mul $10 $15 $15
sw $10 -76($fp)
lw $8 -72($fp)
lw $9 -76($fp)
add $10 $8 $9
sw $10 -80($fp)
mul $10 $15 $14
sw $10 -84($fp)
lw $8 -84($fp)
addi $10 $8 3
sw $10 -88($fp)
lw $8 -88($fp)
mul $10 $8 9
sw $10 -92($fp)
lw $8 -92($fp)
div $10 $8 3
sw $10 -96($fp)
lw $8 -96($fp)
mul $10 $8 10
sw $10 -100($fp)
lw $8 -80($fp)
lw $9 -100($fp)
add $10 $8 $9
sw $10 -104($fp)
addi $10 $14 22
sw $10 -108($fp)
lw $8 -108($fp)
mul $10 $8 $15
sw $10 -112($fp)
lw $8 -112($fp)
mul $10 $8 $14
sw $10 -116($fp)
addi $10 $0 12
sw $10 -120($fp)
lw $8 -120($fp)
sub $10 $8 $14
sw $10 -124($fp)
lw $8 -116($fp)
lw $9 -124($fp)
mul $10 $8 $9
sw $10 -128($fp)
lw $8 -104($fp)
lw $9 -128($fp)
add $10 $8 $9
sw $10 -132($fp)
mul $10 $14 $14
sw $10 -136($fp)
lw $8 -132($fp)
lw $9 -136($fp)
sub $10 $8 $9
sw $10 -140($fp)
lw $8 -140($fp)
addi $10 $8 1
sw $10 -144($fp)
lw $8 -144($fp)
addi $10 $8 10
sw $10 -148($fp)
lw $8 -148($fp)
subi $10 $8 13
sw $10 -152($fp)
div $10 $14 2
sw $10 -156($fp)
lw $8 -152($fp)
lw $9 -156($fp)
sub $10 $8 $9
sw $10 -160($fp)
div $10 $15 2
sw $10 -164($fp)
lw $8 -160($fp)
lw $9 -164($fp)
sub $10 $8 $9
sw $10 -168($fp)
mul $10 $14 $14
sw $10 -172($fp)
lw $8 -172($fp)
mul $10 $8 $15
sw $10 -176($fp)
lw $8 -176($fp)
mul $10 $8 $14
sw $10 -180($fp)
lw $8 -180($fp)
div $10 $8 2
sw $10 -184($fp)
lw $8 -168($fp)
lw $9 -184($fp)
add $10 $8 $9
sw $10 -188($fp)
lw $8 -188($fp)
li $v0 1
move $a0 $8
syscall
li $v0 4
la $a0 $str14
syscall
li $v0 11
addi $a0 $0 65
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
addi $sp $sp 304
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
jal read
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
