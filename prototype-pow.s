.text

.global prototype

prototype:

	push %ebp
	mov %esp, %ebp

	push 12(%ebp)
	push 8(%ebp)
	push $0x00000000
	push $0x00000002
	call pow
	addl $8, %esp
	leave
	ret
