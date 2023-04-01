	.data
e:	.float 0
c:	.float 0
b:	.float 0
a:	.float 0
d:	.word 0

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
	sub $sp, $sp, $t0

	# write the string "the result is " to the allocated space on the stack
	li $t1, '"'
	sb $t1, 0($sp)
	li $t1, 't'
	sb $t1, 1($sp)
	li $t1, 'h'
	sb $t1, 2($sp)
	li $t1, 'e'
	sb $t1, 3($sp)
	li $t1, ' '
	sb $t1, 4($sp)
	li $t1, 'r'
	sb $t1, 5($sp)
	li $t1, 'e'
	sb $t1, 6($sp)
	li $t1, 's'
	sb $t1, 7($sp)
	li $t1, 'u'
	sb $t1, 8($sp)
	li $t1, 'l'
	sb $t1, 9($sp)
	li $t1, 't'
	sb $t1, 10($sp)
	li $t1, ' '
	sb $t1, 11($sp)
	li $t1, 'i'
	sb $t1, 12($sp)
	li $t1, 's'
	sb $t1, 13($sp)
	li $t1, ' '
	sb $t1, 14($sp)
	li $t1, '"'
	sb $t1, 15($sp)

	# store pointer to string in $s0
	move $s0, $sp

	sw $s0, d
	la $t0, d

	li $v0, 4    # system call for printing 
	lw $a0, d      # set argument print 
	syscall         # execute system call

	li $v0, 2    # system call for printing 
	lw $a0, a      # set argument print 
	syscall         # execute system call

	li $v0, 2    # system call for printing 
	lw $a0, b      # set argument print 
	syscall         # execute system call

	# exit the program                   
	li $v0, 10                   
	syscall