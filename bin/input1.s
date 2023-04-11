# Compiled by Amit Zohar 313307720 & Ofek Ben Atar 322208430
	.data
f:	.word 0
e:	.word 0
c:	.word 0
b:	.word 0
i:	.word 0
y:	.word 0
a:	.word 0
amit:	.float 3.4
	.text
	.globl main

main:

	# read input

	# input int 
	li $v0, 5     
	syscall       
	sw $v0, b    

	# read input

	# input int 
	li $v0, 5     
	syscall       
	sw $v0, c    

	lw $t0, b

	lw $t1, c

	# compare two ints
	sgt $t0, $t0, $t1

	beq $t0, $zero, else0

	lw $t2, c

	# printing 
	li $v0, 1   
	move $a0, $t2       
	syscall      

	j continue0

else0:

	lw $t3, c

	# printing 
	li $v0, 1   
	move $a0, $t3       
	syscall      

continue0:

	# exit the program
	li $v0, 10
	syscall
