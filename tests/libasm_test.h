#ifndef LIBASM_TEST_H
# define LIBASM_TEST_H

# include "libasm.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>

# define TMP_TEMPLATE "/tmp/libasm_test_XXXXXX"
# define LOREM "The quick brown fox jumps over the lazy dog"

# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define CYAN		"\033[36m"
# define GRAY		"\033[90m"
# define BOLD		"\033[1m"
# define ITALIC		"\033[3m"
# define RESET		"\033[0m"

extern int	g_passed;
extern int	g_failed;

void	section(const char *name);
void	check(int ok, const char *fmt, ...);
int		summary(void);

int		sign(int n);
int		tmp_file(char *path);
char	*guard_str(const char *s, void **map);
char	*guard_buf(size_t size, void **map);
void	guard_free(void *map);
int		child_ok(void (*fn)(void));

void	test_strlen(void);
void	test_strcpy(void);
void	test_strcmp(void);
void	test_write(void);
void	test_read(void);
void	test_strdup(void);

#endif
