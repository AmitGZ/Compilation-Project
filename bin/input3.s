# Compiled by Amit Zohar 313307720 & Ofek Ben Atar 322208430
	.data
x:	.word 0
	.text
	.globl main

main:

	# read input

	# input int 
	li $v0, 5     
	syscall       
	sw $v0, x    

	lw $t0, x

	# loading immediate int
	li $t1, 2

	# compare two ints
	sge $t0, $t0, $t1

	beq $t0, $zero, else1

	lw $t2, x

	# loading immediate int
	li $t3, 5

	# compare two ints
	slt $t2, $t2, $t3

	beq $t2, $zero, else2

	lw $t4, x

	# loading immediate int
	li $t5, 7

	# mathop two ints
	add $t4, $t4, $t5

	# printing 
	li $v0, 1   
	move $a0, $t4       
	syscall      

	j continue2

else2:

	lw $t5, x

	# printing 
	li $v0, 1   
	move $a0, $t5       
	syscall      

continue2:

	j continue1

else1:

	# loading immediate int
	li $t6, 100

	lw $t7, x

	# mathop two ints
	sub $t6, $t6, $t7

	# printing 
	li $v0, 1   
	move $a0, $t6       
	syscall      

continue1:

	# exit the program
	li $v0, 10
	syscall
