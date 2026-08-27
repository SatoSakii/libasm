#include "libasm_test.h"

static void	cmp_dup(const char *src, const char *name)
{
	char	*dup;

	dup = ft_strdup(src);
	check(dup != NULL, "%s: returns a non NULL pointer", name);
	if (!dup)
		return ;
	check(dup != src, "%s: returns a new pointer", name);
	check(strcmp(dup, src) == 0, "%s: content duplicated", name);
	check(ft_strlen(dup) == strlen(src), "%s: same length", name);
	free(dup);
}

static void	independent_copy(void)
{
	char	src[16];
	char	*dup;

	strcpy(src, "Hello World!");
	dup = ft_strdup(src);
	if (!dup)
		return ;
	memset(src, 'A', 12);
	check(strcmp(dup, "Hello World!") == 0,
		"copy survives a write to the source");
	memset(dup, 'z', strlen(dup) + 1);
	check(dup[0] == 'z', "copy is writable up to the terminating null");
	free(dup);
}

static void	long_string(void)
{
	char	*src;
	char	*dup;

	src = malloc(4097);
	if (!src)
		return ;
	memset(src, 'x', 4096);
	src[4096] = '\0';
	dup = ft_strdup(src);
	check(dup != NULL && strcmp(dup, src) == 0, "4096 bytes duplicated");
	free(dup);
	free(src);
}

static void	errno_on_success(void)
{
	char	*dup;

	errno = 42;
	dup = ft_strdup(LOREM);
	check(errno == 42, "errno untouched on success: got %d, expected 42",
		errno);
	free(dup);
}

static void	guard_child(void)
{
	void	*map;
	char	*src;
	char	*dup;

	src = guard_str(LOREM, &map);
	if (!src)
		_exit(0);
	dup = ft_strdup(src);
	if (!dup || strcmp(dup, LOREM) != 0)
		_exit(1);
	free(dup);
	guard_free(map);
}

void	test_strdup(void)
{
	section("ft_strdup");
	cmp_dup("", "empty string");
	cmp_dup("a", "single char");
	cmp_dup("Hello World!", "\"Hello World!\"");
	cmp_dup(LOREM, "sentence with spaces");
	cmp_dup("\x80\xff\xfe", "high bytes");
	independent_copy();
	long_string();
	errno_on_success();
	check(child_ok(guard_child), "no read past the terminating null");
}
