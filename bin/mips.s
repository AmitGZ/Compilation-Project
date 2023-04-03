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

	# read input
	li $v0, 5  
	syscall    
	sw $v0, a 

	# read input
	li $v0, 6  
	syscall      
	s.s $f0, b  

	# read input
	li $v0, 6  
	syscall      
	s.s $f0, c  

	# read input
	li $v0, 6  
	syscall      
	s.s $f0, e  

	l.s $f1, b

	lw $t0, a

	# Move integer value to floating-point register
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0

	# mathop two floats
	add.s $f0, $f0, $f1

	l.s $f1, c

	mov.s $f0, $f0

	# mathop two floats
	mul.s $f0, $f0, $f1

	l.s $f1, e

	mov.s $f0, $f0

	# mathop two floats
	div.s $f0, $f0, $f1

	l.s $f1, c

	l.s $f0, b

	# mathop two floats
	sub.s $f0, $f0, $f1

	mov.s $f1, $f0

	mov.s $f0, $f0

	# mathop two floats
	sub.s $f0, $f0, $f1

	# assigning float value
	s.s $f0, b

	.data
str0: .asciiz "the result is "
	.text

	# store pointer to string in $s0
	la $s0, str0

	# assigning string pointer
	sw $s0, d

	# printing  
	li $v0, 4   
	lw $a0, d       
	syscall      

	l.s $f1, b

	lw $t0, a

	# Move integer value to floating-point register
	mtc1 $t0, $f0
	cvt.s.w $f0, $f0

	# compare two floats
	c.lt.s $f0, $f1
	movt $t0, $zero, 1
	movf $t0, $zero, 0

	# printing  
	li $v0, 1   
	lw $a0, a       
	syscall      

	# printing  
	li $v0, 2   
	l.s $f12, b       
	syscall      

	# exit the program
	li $v0, 10
	syscall
