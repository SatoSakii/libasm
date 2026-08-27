section .text

global ft_read

extern __errno_location

ft_read:
	xor rax, rax
	syscall
	cmp rax, 0
	jge .end
	mov rdx, rax
	call __errno_location wrt ..plt
	neg rdx
	mov dword [rax], edx
	mov rax, -1

.end:
	ret