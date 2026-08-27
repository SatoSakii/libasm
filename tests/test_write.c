#include "libasm_test.h"

static void	write_to_file(void)
{
	char	path[64];
	char	buf[64];
	int		fd;
	ssize_t	n;

	fd = tmp_file(path);
	if (fd < 0)
		return ;
	n = ft_write(fd, "Hello World!", 12);
	check(n == 12, "returns the byte count: got %zd, expected 12", n);
	n = ft_write(fd, "", 0);
	check(n == 0, "zero byte write: got %zd, expected 0", n);
	lseek(fd, 0, SEEK_SET);
	memset(buf, 0, sizeof(buf));
	if (read(fd, buf, sizeof(buf) - 1) < 0)
		return ;
	check(strcmp(buf, "Hello World!") == 0, "content written: \"%s\"", buf);
	close(fd);
	unlink(path);
}

static void	large_write(void)
{
	struct stat	st;
	char		path[64];
	char		*buf;
	int			fd;
	ssize_t		n;

	buf = malloc(1 << 20);
	fd = tmp_file(path);
	if (!buf || fd < 0)
		return ;
	memset(buf, 'x', 1 << 20);
	n = ft_write(fd, buf, 1 << 20);
	check(n == (1 << 20), "1 MiB write: got %zd, expected %d", n, 1 << 20);
	if (fstat(fd, &st) == 0)
		check(st.st_size == (1 << 20), "resulting file size: got %lld",
			(long long)st.st_size);
	close(fd);
	unlink(path);
	free(buf);
}

static void	errno_on_success(void)
{
	char	path[64];
	int		fd;

	fd = tmp_file(path);
	if (fd < 0)
		return ;
	errno = 42;
	ft_write(fd, "x", 1);
	check(errno == 42, "errno untouched on success: got %d, expected 42",
		errno);
	close(fd);
	unlink(path);
}

static void	bad_fd(void)
{
	ssize_t	n;

	errno = 0;
	n = ft_write(-1, "x", 1);
	check(n == -1, "negative fd returns -1: got %zd", n);
	check(errno == EBADF, "negative fd sets errno to EBADF: got %d", errno);
	errno = 0;
	n = ft_write(4242, "x", 1);
	check(n == -1 && errno == EBADF,
		"unopened fd returns -1 with EBADF: got %zd, errno %d", n, errno);
}

static void	readonly_fd(void)
{
	char	path[64];
	int		fd;
	int		ro;
	ssize_t	n;

	fd = tmp_file(path);
	if (fd < 0)
		return ;
	close(fd);
	ro = open(path, O_RDONLY);
	if (ro < 0)
		return ;
	errno = 0;
	n = ft_write(ro, "x", 1);
	check(n == -1 && errno == EBADF,
		"read only fd returns -1 with EBADF: got %zd, errno %d", n, errno);
	close(ro);
	unlink(path);
}

static void	broken_pipe(void)
{
	int		fds[2];
	ssize_t	n;

	signal(SIGPIPE, SIG_IGN);
	if (pipe(fds) < 0)
		return ;
	close(fds[0]);
	errno = 0;
	n = ft_write(fds[1], "x", 1);
	check(n == -1 && errno == EPIPE,
		"broken pipe returns -1 with EPIPE: got %zd, errno %d", n, errno);
	close(fds[1]);
	signal(SIGPIPE, SIG_DFL);
}

void	test_write(void)
{
	section("ft_write");
	write_to_file();
	large_write();
	errno_on_success();
	bad_fd();
	readonly_fd();
	broken_pipe();
}
