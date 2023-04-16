# Compiled by Amit Zohar 313307720 & Ofek Ben Atar 322208430
	.data
y:	.word 0
j:	.word 0
i:	.word 0
x:	.word 0
	.text
	.globl main

main:

	# read input

	# input int 
	li $v0, 5     
	syscall       
	sw $v0, x    

	# read input

	# input int 
	li $v0, 5     
	syscall       
	sw $v0, y    

	# loading immediate int
	li $t0, 0

	# assigning value
	sw $t0, i

while_1:

	lw $t1, i

	lw $t2, x

	# compare two ints
	slt $t1, $t1, $t2

	beq $t1, $zero, endwhile_1

	j for_each_stmt_1

for_each_increment_1:

	# loading immediate int
	li $t3, 1

	lw $t4, i

	# mathop two ints
	add $t3, $t3, $t4

	# assigning value
	sw $t3, i

	j while_1

for_each_stmt_1:

	# loading immediate int
	li $t4, 0

	# assigning value
	sw $t4, j

while_2:

	lw $t5, j

	lw $t6, y

	# compare two ints
	slt $t5, $t5, $t6

	beq $t5, $zero, endwhile_2

	j for_each_stmt_2

for_each_increment_2:

	# loading immediate int
	li $t7, 1

	lw $t8, j

	# mathop two ints
	add $t7, $t7, $t8

	# assigning value
	sw $t7, j

	j while_2

for_each_stmt_2:

	lw $t8, j

	# printing 
	li $v0, 1   
	move $a0, $t8       
	syscall      

	j for_each_increment_2

endwhile_2:

	j for_each_increment_1

endwhile_1:

	# exit the program
	li $v0, 10
	syscall
