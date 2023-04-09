# Compiled by Amit Zohar & Ofek Ben Atar
	.data
i:	.word 0
a:	.word 0
d:	.word 0
	.text
	.globl main

main:

	.data
str0: .asciiz "Pls enter a number "
	.text

	# store pointer to string
	la $t0, str0

	# assigning value
	sw $t0, d

	# read input

	# input int 
	li $v0, 5   
	syscall     
	sw $v0, a  

	# loading immediate int
	li $t0, 1

	# assigning value
	sw $t0, i

while_0:

	lw $t1, i

	lw $t2, a

	# compare two ints
	slt $t1, $t1, $t2

	beq $t1, $zero, endwhile_0

	j for_each_stmt_0

for_each_increment_0:

	# loading immediate int
	li $t3, 1

	lw $t4, i

	# mathop two ints
	add $t3, $t3, $t4

	# assigning value
	sw $t3, i

	j while_0

for_each_stmt_0:

	lw $t4, i

	# printing 
	li $v0, 1   
	move $a0, $t4       
	syscall      

	j for_each_increment_0

endwhile_0:

	# exit the program
	li $v0, 10
	syscall
