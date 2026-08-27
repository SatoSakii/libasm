#include "libasm_test.h"

static void	cmp_len(const char *s, const char *name)
{
	size_t	expected;
	size_t	got;

	expected = strlen(s);
	got = ft_strlen(s);
	check(got == expected, "%s: got %zu, expected %zu", name, got, expected);
}

static void	long_string(void)
{
	char	*s;
	size_t	got;

	s = malloc(4097);
	if (!s)
		return ;
	memset(s, 'x', 4096);
	s[4096] = '\0';
	got = ft_strlen(s);
	check(got == 4096, "4096 bytes: got %zu, expected 4096", got);
	got = ft_strlen(s + 1);
	check(got == 4095, "unaligned + 1: got %zu, expected 4095", got);
	got = ft_strlen(s + 7);
	check(got == 4089, "unaligned + 7: got %zu, expected 4089", got);
	s[2048] = '\0';
	got = ft_strlen(s);
	check(got == 2048, "early terminator: got %zu, expected 2048", got);
	free(s);
}

static void	guard_child(void)
{
	void	*map;
	char	*s;

	s = guard_str(LOREM, &map);
	if (!s)
		_exit(0);
	if (ft_strlen(s) != strlen(LOREM))
		_exit(1);
	guard_free(map);
	s = guard_str("", &map);
	if (!s)
		_exit(0);
	if (ft_strlen(s) != 0)
		_exit(1);
	guard_free(map);
}

void	test_strlen(void)
{
	section("ft_strlen");
	cmp_len("", "empty string");
	cmp_len("a", "single char");
	cmp_len("Hello World!", "\"Hello World!\"");
	cmp_len(LOREM, "sentence with spaces");
	cmp_len("\t\n\r\v\f", "control characters");
	cmp_len("\x80\xff\xfe", "high bytes");
	long_string();
	check(child_ok(guard_child), "no read past the terminating null");
}
