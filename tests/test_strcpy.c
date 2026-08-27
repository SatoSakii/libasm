#include "libasm_test.h"

static void	cmp_copy(const char *src, const char *name)
{
	char	dst[256];
	char	*ret;

	memset(dst, 'A', sizeof(dst));
	ret = ft_strcpy(dst, src);
	check(ret == dst, "%s: returns dst", name);
	check(memcmp(dst, src, strlen(src) + 1) == 0, "%s: content copied", name);
	check(dst[strlen(src) + 1] == 'A', "%s: nothing written past null", name);
}

static void	long_string(void)
{
	char	*src;
	char	*dst;

	src = malloc(4097);
	dst = malloc(4097);
	if (!src || !dst)
		return ;
	memset(src, 'z', 4096);
	src[4096] = '\0';
	memset(dst, 'A', 4097);
	check(ft_strcpy(dst, src) == dst, "4096 bytes: returns dst");
	check(strcmp(dst, src) == 0, "4096 bytes: content copied");
	memset(dst, 'A', 4097);
	ft_strcpy(dst + 3, src + 5);
	check(strcmp(dst + 3, src + 5) == 0, "unaligned src and dst");
	free(src);
	free(dst);
}

static void	embedded_null(void)
{
	char	dst[16];
	char	src[16];

	memset(dst, 'A', sizeof(dst));
	memcpy(src, "abc\0defghijklmn", 16);
	ft_strcpy(dst, src);
	check(memcmp(dst, "abc", 4) == 0 && dst[4] == 'A' && dst[15] == 'A',
		"stops at the first null of src");
}

static void	guard_child(void)
{
	void	*src_map;
	void	*dst_map;
	char	*src;
	char	*dst;

	src = guard_str(LOREM, &src_map);
	dst = guard_buf(strlen(LOREM) + 1, &dst_map);
	if (!src || !dst)
		_exit(0);
	if (ft_strcpy(dst, src) != dst || strcmp(dst, LOREM) != 0)
		_exit(1);
	guard_free(src_map);
	guard_free(dst_map);
}

void	test_strcpy(void)
{
	section("ft_strcpy");
	cmp_copy("", "empty string");
	cmp_copy("a", "single char");
	cmp_copy("Hello World!", "\"Hello World!\"");
	cmp_copy(LOREM, "sentence with spaces");
	cmp_copy("\x80\xff\xfe", "high bytes");
	embedded_null();
	long_string();
	check(child_ok(guard_child), "no access past the terminating null");
}
