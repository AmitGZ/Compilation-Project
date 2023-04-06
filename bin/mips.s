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
	li $v0, 6    
	syscall      
	s.s $f0, b  

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, c  

	.data
str0: .asciiz "the result is "
	.text

	# store pointer to string
	la $t0, str0

	# assigning string pointer
	sw $t0, d

while0:
	l.s $f0, b

	l.s $f1, c

	# compare two floats and negate
	c.le.s $f0, $f1
	movt $t0, $zero, 0
	movf $t0, $zero, 1

	Beq $t0, 0, endwhile0
	# read input
	li $v0, 6    
	syscall      
	s.s $f0, b  

	j while0
endwhile0:

	l.s $f0, b

	l.s $f1, c

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
