#include "libasm_test.h"
#include <stdarg.h>

int	g_passed = 0;
int	g_failed = 0;

void	section(const char *name)
{
	printf("\n " CYAN BOLD ITALIC "■" RESET "  %s\n", name);
}

void	check(int ok, const char *fmt, ...)
{
	va_list	ap;

	if (ok)
	{
		g_passed++;
		printf("    " GREEN "✓" RESET " " GRAY);
	}
	else
	{
		g_failed++;
		printf("    " RED BOLD "✗" RESET " " RED);
	}
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(RESET "\n");
}

int	summary(void)
{
	printf("\n " BOLD "%d" RESET " tests, " GREEN BOLD "%d passed" RESET,
		g_passed + g_failed, g_passed);
	if (g_failed)
		printf(", " RED BOLD "%d failed" RESET "\n\n", g_failed);
	else
		printf("\n\n");
	if (g_failed)
		return (1);
	return (0);
}

int	sign(int n)
{
	if (n > 0)
		return (1);
	if (n < 0)
		return (-1);
	return (0);
}
