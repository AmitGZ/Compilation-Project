	.data
buffer: .space 256   # allocate 256 bytes for the input buffer
f:	.float 0
e:	.float 0
c:	.float 0
b:	.float 0
i:	.word 0
y:	.word 0
a:	.word 0
d:	.word 0

	.text                   
	.globl main                   

main:

	.data
str0: .asciiz "abc"
	.text

	# store pointer to string
	la $t0, str0

	# assigning string pointer
	sw $t0, d

	# read input
	li $v0, 5  
	syscall    
	sw $v0, a 

	li $t0, 0

	# assigning integer value
	sw $t0, i

while0:

	lw $t1, i

	li $t2, 19

	# compare two ints
	slt $t1, $t1, $t2

	beq $t1, $zero, endwhile0

	j for_each_stmt0

for_each_increment0:

	li $t3, 1

	lw $t4, i

	# mathop two ints
	add $t3, $t3, $t4

	# assigning integer value
	sw $t3, i

	j while0

for_each_stmt0:

	lw $t4, i

	# printing 
	li $v0, 1   
	move $a0, $t4       
	syscall      

	j for_each_increment0

endwhile0:

	la $t0, d

	# printing 
	li $v0, 4   
	move $a0, $t0       
	syscall      

	# exit the program
	li $v0, 10
	syscall
