.data
	e:	.float 
	c:	.float 
	b:	.float 
	a:	.float 
	d:	.asciiz 
	li $v0, 6    # set $v0 to indicate we want to read input
	syscall      # execute the syscall instruction to read the input
	sw $v0, a   # store the input

	li $v0, 6    # set $v0 to indicate we want to read input
	syscall      # execute the syscall instruction to read the input
	sw $v0, b   # store the input

