	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
e:	.float 0
c:	.float 0
b:	.float 0
a:	.word 0
d:	.word 0

	.text                                                                                           
	.globl main                                                                                           

main:

	li $v0, 5   # set $v0 to indicate we want to read input
	syscall       # execute the syscall instruction to read the input
	sw $v0, a   # store the input

	li $v0, 6   # set $v0 to indicate we want to read input
	syscall       # execute the syscall instruction to read the input
	s.s $f0, b  # store the input
	.data
input_prompt: .asciiz "the result is "
	.text

	# store pointer to string in $s0
	la $s0, input_prompt

	sw $s0, d
	la $t0, d

	li $v0, 4    # system call for printing 
	lw $a0, d      # set argument print 
	syscall         # execute system call

	li $v0, 1    # system call for printing 
	lw $a0, a      # set argument print 
	syscall         # execute system call

	li $v0, 2    # system call for printing 
	l.s $f12, b      # set argument print 
	syscall         # execute system call

	# exit the program                   
	li $v0, 10                   
	syscall