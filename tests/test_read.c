#include "libasm_test.h"

static int	filled_tmp(char *path, const char *content)
{
	int	fd;

	fd = tmp_file(path);
	if (fd < 0)
		return (-1);
	if (write(fd, content, strlen(content)) < 0)
	{
		close(fd);
		return (-1);
	}
	lseek(fd, 0, SEEK_SET);
	return (fd);
}

static void	read_whole_file(void)
{
	char	path[64];
	char	buf[128];
	int		fd;
	ssize_t	n;

	fd = filled_tmp(path, LOREM);
	if (fd < 0)
		return ;
	memset(buf, 0, sizeof(buf));
	n = ft_read(fd, buf, sizeof(buf) - 1);
	check(n == (ssize_t)strlen(LOREM), "returns the byte count: got %zd, "
		"expected %zu", n, strlen(LOREM));
	check(strcmp(buf, LOREM) == 0, "content read: \"%s\"", buf);
	n = ft_read(fd, buf, sizeof(buf) - 1);
	check(n == 0, "read at EOF returns 0: got %zd", n);
	close(fd);
	unlink(path);
}

static void	partial_read(void)
{
	char	path[64];
	char	buf[64];
	int		fd;
	ssize_t	n;

	fd = filled_tmp(path, LOREM);
	if (fd < 0)
		return ;
	memset(buf, 'A', sizeof(buf));
	n = ft_read(fd, buf, 5);
	check(n == 5, "partial read: got %zd, expected 5", n);
	check(memcmp(buf, LOREM, 5) == 0, "first 5 bytes read");
	check(buf[5] == 'A', "nothing written past count");
	n = ft_read(fd, buf, 0);
	check(n == 0, "zero byte read: got %zd", n);
	n = ft_read(fd, buf, sizeof(buf));
	check(n == (ssize_t)strlen(LOREM) - 5, "resumes at the file offset: "
		"got %zd, expected %zu", n, strlen(LOREM) - 5);
	close(fd);
	unlink(path);
}

static void	read_from_pipe(void)
{
	char	buf[64];
	int		fds[2];
	ssize_t	n;

	if (pipe(fds) < 0)
		return ;
	if (write(fds[1], "Hello World!", 12) < 0)
		return ;
	memset(buf, 0, sizeof(buf));
	n = ft_read(fds[0], buf, sizeof(buf) - 1);
	check(n == 12, "read from a pipe: got %zd, expected 12", n);
	check(strcmp(buf, "Hello World!") == 0, "pipe content: \"%s\"", buf);
	close(fds[0]);
	close(fds[1]);
}

static void	errno_on_success(void)
{
	char	path[64];
	char	buf[64];
	int		fd;

	fd = filled_tmp(path, LOREM);
	if (fd < 0)
		return ;
	errno = 42;
	ft_read(fd, buf, sizeof(buf));
	check(errno == 42, "errno untouched on success: got %d, expected 42",
		errno);
	close(fd);
	unlink(path);
}

static void	bad_fd(void)
{
	char	buf[64];
	ssize_t	n;

	errno = 0;
	n = ft_read(-1, buf, sizeof(buf));
	check(n == -1, "negative fd returns -1: got %zd", n);
	check(errno == EBADF, "negative fd sets errno to EBADF: got %d", errno);
	errno = 0;
	n = ft_read(4242, buf, sizeof(buf));
	check(n == -1 && errno == EBADF,
		"unopened fd returns -1 with EBADF: got %zd, errno %d", n, errno);
}

static void	writeonly_fd(void)
{
	char	path[64];
	char	buf[64];
	int		fd;
	int		wo;
	ssize_t	n;

	fd = tmp_file(path);
	if (fd < 0)
		return ;
	close(fd);
	wo = open(path, O_WRONLY);
	if (wo < 0)
		return ;
	errno = 0;
	n = ft_read(wo, buf, sizeof(buf));
	check(n == -1 && errno == EBADF,
		"write only fd returns -1 with EBADF: got %zd, errno %d", n, errno);
	close(wo);
	unlink(path);
}

void	test_read(void)
{
	section("ft_read");
	read_whole_file();
	partial_read();
	read_from_pipe();
	errno_on_success();
	bad_fd();
	writeonly_fd();
}
