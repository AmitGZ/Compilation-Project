	.data
f:	.float 0
e:	.float 0
c:	.float 0
b:	.float 0
i:	.word 0
y:	.word 0
a:	.word 0

	.text                   
	.globl main                   

main:

	# read input

	.data
buffer0: .space 256 # allocate buffer to read input
	.text
	li $v0, 8         
	la $a0, buffer0 
	li $a1, 256        
	sw $a0, a        
	syscall           

	# read input

	.data
buffer1: .space 256 # allocate buffer to read input
	.text
	li $v0, 8         
	la $a0, buffer1 
	li $a1, 256        
	sw $a0, y        
	syscall           

	# read input

	.data
buffer2: .space 256 # allocate buffer to read input
	.text
	li $v0, 8         
	la $a0, buffer2 
	li $a1, 256        
	sw $a0, i        
	syscall           

	.data
str0: .asciiz "test"
	.text

	# store pointer to string
	la $t0, str0

	# assigning string pointer
	sw $t0, y

	lw $t0, a

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	lw $t0, y

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	lw $t0, i

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	lw $t0, y

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	# exit the program
	li $v0, 10
	syscall
