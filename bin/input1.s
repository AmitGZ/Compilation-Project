	.data
f:	.float 0
e:	.float 0
c:	.float 0
b:	.float 0
i:	.word 0
y:	.word 0
a:	.word 0
amit:	.float 5.8

	.text                   
	.globl main                   

main:

	l.s $f4, amit

	li $t0, 0x40800000
	mtc1 $t0, $f5

	# mathop two floats
	mul.s $f4, $f4, $f5

	# assigning value
	s.s $f4, b

	l.s $f4, amit

	# printing 
	li $v0, 2   
	mov.s $f12, $f4       
	syscall      

	l.s $f4, b

	# printing 
	li $v0, 2   
	mov.s $f12, $f4       
	syscall      

	# exit the program
	li $v0, 10
	syscall
