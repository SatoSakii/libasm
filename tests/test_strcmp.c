#include "libasm_test.h"

static void	cmp_pair(const char *s1, const char *s2, const char *name)
{
	int	expected;
	int	got;

	expected = strcmp(s1, s2);
	got = ft_strcmp(s1, s2);
	check(sign(got) == sign(expected), "%s: got %d, expected sign %d",
		name, got, sign(expected));
}

static void	both_ways(const char *s1, const char *s2, const char *name)
{
	cmp_pair(s1, s2, name);
	cmp_pair(s2, s1, name);
}

static void	long_strings(void)
{
	char	*a;
	char	*b;

	a = malloc(4097);
	b = malloc(4097);
	if (!a || !b)
		return ;
	memset(a, 'x', 4096);
	memset(b, 'x', 4096);
	a[4096] = '\0';
	b[4096] = '\0';
	check(ft_strcmp(a, b) == 0, "4096 identical bytes: expected 0");
	b[4095] = 'y';
	check(sign(ft_strcmp(a, b)) == sign(strcmp(a, b)),
		"difference on the last byte");
	b[0] = 'y';
	check(sign(ft_strcmp(a, b)) == sign(strcmp(a, b)),
		"difference on the first byte");
	free(a);
	free(b);
}

static void	unsigned_compare(void)
{
	int	got;

	got = ft_strcmp("\x80", "\x01");
	check(got > 0, "bytes compared as unsigned: got %d, expected > 0", got);
	got = ft_strcmp("\x01", "\x80");
	check(got < 0, "bytes compared as unsigned: got %d, expected < 0", got);
	got = ft_strcmp("\xff", "\x7f");
	check(got > 0, "0xff > 0x7f: got %d, expected > 0", got);
}

static void	guard_child(void)
{
	void	*map1;
	void	*map2;
	char	*s1;
	char	*s2;

	s1 = guard_str(LOREM, &map1);
	s2 = guard_str(LOREM, &map2);
	if (!s1 || !s2)
		_exit(0);
	if (ft_strcmp(s1, s2) != 0)
		_exit(1);
	guard_free(map1);
	guard_free(map2);
}

void	test_strcmp(void)
{
	section("ft_strcmp");
	cmp_pair("", "", "two empty strings");
	cmp_pair("a", "a", "identical single char");
	cmp_pair(LOREM, LOREM, "identical sentences");
	both_ways("", "a", "empty against non empty");
	both_ways("abc", "abd", "difference on the last byte");
	both_ways("abc", "abcd", "prefix");
	both_ways("Hello World!", "Hello Wierdo!", "difference in the middle");
	both_ways("A", "a", "case difference");
	unsigned_compare();
	long_strings();
	check(child_ok(guard_child), "no read past the terminating null");
}
