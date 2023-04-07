	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
f:	.float 0
e:	.float 0
c:	.float 0
b:	.float 0
y:	.word 0
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

	# true register
	li $t1, 1

	# compare two floats and negate
	c.le.s $f0, $f1
	movt $t0, $zero
	movf $t0, $t1

	beq $t0, $zero, endwhile0

	# read input
	li $v0, 6    
	syscall      
	s.s $f0, b  

	j while0

endwhile0:

	# exit the program
	li $v0, 10
	syscall
