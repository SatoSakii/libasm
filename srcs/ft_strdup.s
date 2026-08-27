section .text

global ft_strdup

extern ft_strlen
extern ft_strcpy
extern malloc

ft_strdup:
	xor rax, rax
	push rbx
	call ft_strlen
	mov rbx, rdi
	lea rdi, [rax + 1]
	call malloc wrt ..plt
	cmp rax, 0
	je .end
	mov rdi, rax
	mov rsi, rbx
	call ft_strcpy

.end:
	pop rbx
	ret