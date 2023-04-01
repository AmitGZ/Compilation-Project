	.data
e:	.float 
c:	.float 
b:	.float 
a:	.float 
d:	.asciiz 

	.text                                                                                           
	.globl main                                                                                           

main:

	li $v0, 6   # set $v0 to indicate we want to read input
	syscall      # execute the syscall instruction to read the input
	sw $v0, a   # store the input

	li $v0, 6   # set $v0 to indicate we want to read input
	syscall      # execute the syscall instruction to read the input
	sw $v0, b   # store the input

	# allocate space on the stack for the string
	li $t0, 16
	addi $t0, $t0, 1 # add 1 for null terminator
	add $sp, $sp, -$t0

	# write the string "the result is " to the allocated space on the stack
	li $t1, '"'
	sw $t1, 0($sp)
	li $t1, 't'
	sw $t1, 0($sp)
	li $t1, 'h'
	sw $t1, 0($sp)
	li $t1, 'e'
	sw $t1, 0($sp)
	li $t1, ' '
	sw $t1, 0($sp)
	li $t1, 'r'
	sw $t1, 0($sp)
	li $t1, 'e'
	sw $t1, 0($sp)
	li $t1, 's'
	sw $t1, 0($sp)
	li $t1, 'u'
	sw $t1, 0($sp)
	li $t1, 'l'
	sw $t1, 0($sp)
	li $t1, 't'
	sw $t1, 0($sp)
	li $t1, ' '
	sw $t1, 0($sp)
	li $t1, 'i'
	sw $t1, 0($sp)
	li $t1, 's'
	sw $t1, 0($sp)
	li $t1, ' '
	sw $t1, 0($sp)
	li $t1, '"'
	sw $t1, 0($sp)

	# store pointer to string in $s0
	move $s0, $sp

	la $t0, d

	# exit the program                   
	li $v0, 10                   
	syscall