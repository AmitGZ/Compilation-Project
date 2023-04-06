	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
f:	.float 0
e:	.float 0
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
	li $v0, 5  
	syscall    
	sw $v0, a 

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, c  

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, e  

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, f  

	l.s $f0, b

	lw $t0, a

	# Move integer value to floating-point register
	mtc1 $t0, $f1
	cvt.s.w $f1, $f1

	# mathop two floats
	sub.s $f0, $f0, $f1

	# assigning float value
	s.s $f0, b

	l.s $f0, b

	lw $t0, a

	# Move integer value to floating-point register
	mtc1 $t0, $f1
	cvt.s.w $f1, $f1

	# mathop two floats
	sub.s $f0, $f0, $f1

	# assigning float value
	s.s $f0, b

	.data
str0: .asciiz "the result is "
	.text

	# store pointer to string
	la $t0, str0

	# assigning string pointer
	sw $t0, d

	la $t0, d

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	lw $t0, a

	l.s $f0, b

	# Move integer value to floating-point register
	mtc1 $t0, $f1
	cvt.s.w $f1, $f1

	# compare two floats and negate
	c.lt.s $f1, $f0
	movt $t0, $zero, 1
	movf $t0, $zero, 0

	l.s $f0, b

	l.s $f1, c

	# compare two floats and negate
	c.lt.s $f0, $f1
	movt $t1, $zero, 1
	movf $t1, $zero, 0
	and $t0, $t0, $t1

	beq $t0,$0, else

	lw $t2, a

	# printing 
	li $v0, 1   
	move $a0, $t2       
	syscall      

	j continue
	else:
	l.s $f0, b

	# printing 
	li $v0, 2   
	mov.s $f12, $f0       
	syscall      

	continue:

	# exit the program
	li $v0, 10
	syscall
