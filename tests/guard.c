#include "libasm_test.h"

int	tmp_file(char *path)
{
	strcpy(path, TMP_TEMPLATE);
	return (mkstemp(path));
}

char	*guard_buf(size_t size, void **map)
{
	size_t	page;
	char	*mem;

	page = (size_t)sysconf(_SC_PAGESIZE);
	if (size > page)
		return (NULL);
	mem = mmap(NULL, page * 2, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mem == MAP_FAILED)
		return (NULL);
	if (mprotect(mem + page, page, PROT_NONE) != 0)
	{
		munmap(mem, page * 2);
		return (NULL);
	}
	*map = mem;
	return (mem + page - size);
}

char	*guard_str(const char *s, void **map)
{
	size_t	len;
	char	*dst;

	len = strlen(s);
	dst = guard_buf(len + 1, map);
	if (!dst)
		return (NULL);
	memcpy(dst, s, len + 1);
	return (dst);
}

void	guard_free(void *map)
{
	munmap(map, (size_t)sysconf(_SC_PAGESIZE) * 2);
}

int	child_ok(void (*fn)(void))
{
	pid_t	pid;
	int		status;

	fflush(stdout);
	pid = fork();
	if (pid < 0)
		return (0);
	if (pid == 0)
	{
		fn();
		_exit(0);
	}
	if (waitpid(pid, &status, 0) < 0)
		return (0);
	return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}
