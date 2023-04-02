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
