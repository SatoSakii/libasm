section .text

global ft_strcpy

ft_strcpy:
	xor rax, rax

.loop:
	mov cl, byte [rsi + rax]
	cmp cl, 0
	je .end
	mov byte [rdi + rax], cl
	add rax, 1
	jmp .loop

.end:
	mov byte [rdi + rax], 0
	mov rax, rdi
	ret