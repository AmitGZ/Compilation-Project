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

	# read input
	li $v0, 5  
	syscall    
	sw $v0, a 

	# read input
	li $v0, 6    
	syscall      
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

	l.s $f0, b

	lw $t0, a

	# Move integer value to floating-point register
	mtc1 $t0, $f1
	cvt.s.w $f1, $f1

	# compare two floats and negate
	c.lt.s $f0, $f1
	movt $t0, $zero, 1
	movf $t0, $zero, 0

	beq $t0, $zero, else0

	lw $t1, a

	# printing 
	li $v0, 1   
	move $a0, $t1       
	syscall      

	j continue0
else0:
	l.s $f0, b

	# printing 
	li $v0, 2   
	mov.s $f12, $f0       
	syscall      

continue0:

	# exit the program
	li $v0, 10
	syscall
