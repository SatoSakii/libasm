section .text

global ft_strcmp

ft_strcmp:
	xor rax, rax

.loop:
	mov dl, byte [rdi + rax]
	cmp dl, 0
	jne .end
	cmp dl, byte [rsi + rax]
	je .end
	add rax, 1
	jmp .loop

.end:
	movzx rcx, byte [rdi + rax]
	movzx rdx, byte [rsi + rax]
	sub rcx, rdx
	mov rax, rcx
	ret