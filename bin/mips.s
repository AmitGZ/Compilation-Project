	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
c:	.float 0
b:	.float 0
a:	.word 0
d:	.word 0

	.text                                                                               
	.globl main                                                                               

main:

	li $t0, 5

	# Move integer value to floating-point register
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0

	# assigning float value
	s.s $f0, b

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, c  

	l.s $f1, b

	l.s $f2, c

	# mathop two floats
	mul.s $f1, $f1, $f2

	# assigning float value
	s.s $f1, b

	.data
str0: .asciiz "the result is "
	.text

	# store pointer to string in $t
	la $t0, str0

	# assigning string pointer
	sw $t0, d

	la $t1, d

	# printing 
	li $v0, 4   
	move $a0, $t1       
	syscall      

	lw $t2, a

	l.s $f2, b

	# Move integer value to floating-point register
	mtc1 $t2, $f3
	cvt.s.w $f3, $f3

	# compare two floats and negate
	c.lt.s $f3, $f2
	movt $t3, $zero, 1
	movf $t3, $zero, 0

	lw $t3, a

	# printing 
	li $v0, 1   
	move $a0, $t3       
	syscall      

	l.s $f2, b

	# printing 
	li $v0, 2   
	mov.s $f12, $f2       
	syscall      

	# exit the program
	li $v0, 10
	syscall
