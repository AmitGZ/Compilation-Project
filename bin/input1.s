# Compiled by Amit Zohar & Ofek Ben Atar
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

	# exit the program
	li $v0, 10
	syscall
