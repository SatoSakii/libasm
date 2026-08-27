section .text

global ft_strlen

ft_strlen:
	xor rax, rax

.loop:
	cmp byte [rdi + rax], 0
	je .end
	add rax, 1
	jmp .loop

.end:
	ret