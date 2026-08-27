section .text

global ft_write

extern __errno_location

ft_write:
	mov rax, 1
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