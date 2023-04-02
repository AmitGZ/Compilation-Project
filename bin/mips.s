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

	l.s $f0, c

	l.s $f1, e

	# multiply two floats
	mul.s $f0, $f0, $f1

	.data
input_prompt: .asciiz "the result is "
	.text

	# store pointer to string in $s0
	la $s0, input_prompt

	# assigning value
	sw $s0, d

	# printing  
	li $v0, 4   
	lw $a0, d       
	syscall      

	lw $t0, a

	l.s $f1, b

	# compare two floats
	slt.s $f0, $t0, $f1

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
