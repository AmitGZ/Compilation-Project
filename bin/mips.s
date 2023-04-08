	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
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

	.data
str0: .asciiz "assignment test  55"
	.text

	# store pointer to string
	la $t0, str0

	# assigning string pointer
	sw $t0, a

	lw $t0, a

	# assigning string pointer
	sw $t0, y

	lw $t0, y

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	# exit the program
	li $v0, 10
	syscall
