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

	# read input
	li $v0, 5  
	syscall    
	sw $v0, a 
# start switch 0
case_0_0:

	lw $t0, a

	li $t1, 1

	# compare two ints
	seq $t0, $t0, $t1

	beq $t0, $zero, end_case_0_0

	li $t2, 21

	# assigning integer value
	sw $t2, a

	j end_switch_0
end_case_0_0:

case_0_1:

	lw $t0, a

	li $t1, 2

	# compare two ints
	seq $t0, $t0, $t1

	beq $t0, $zero, end_case_0_1

	li $t2, 22

	# assigning integer value
	sw $t2, a

	j end_switch_0
end_case_0_1:

# default:

	li $t0, 23

	# assigning integer value
	sw $t0, a

end_switch_0:

	# read input
	li $v0, 5  
	syscall    
	sw $v0, y 
# start switch 1
case_1_2:

	lw $t0, y

	li $t1, 1

	# compare two ints
	seq $t0, $t0, $t1

	beq $t0, $zero, end_case_1_2

	li $t2, 31

	# assigning integer value
	sw $t2, y

	j end_switch_1
end_case_1_2:

case_1_3:

	lw $t0, y

	li $t1, 2

	# compare two ints
	seq $t0, $t0, $t1

	beq $t0, $zero, end_case_1_3

	li $t2, 32

	# assigning integer value
	sw $t2, y

	j end_switch_1
end_case_1_3:

# default:

	li $t0, 33

	# assigning integer value
	sw $t0, y

end_switch_1:

	lw $t0, a

	# printing 
	li $v0, 1   
	move $a0, $t0       
	syscall      

	lw $t0, y

	# printing 
	li $v0, 1   
	move $a0, $t0       
	syscall      

	# exit the program
	li $v0, 10
	syscall
