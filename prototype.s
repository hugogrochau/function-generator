.text

.global prototype

prototype:

	push %ebp
	mov %esp, %ebp
	add $10, %eax
	imul $2, %eax

	push $43532423
	push $432423
	push $1
	push 8(%ebp)
	push 8(%eax)
	push %ebx
	call foo
	addl $17, %esp 
	leave
	ret
	
foo:
	push %ebp
	mov %esp, %ebp
	leave
	ret
