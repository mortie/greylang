#include "test.h"

#define COLOR_SUCCESS "\033[32m"
#define COLOR_FAIL    "\033[31m"
#define COLOR_DESC    "\033[1m\033[33m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RESET   "\033[0m"

char __description[256];
int __done = 1;
int _ntests = 0;
int _npassed = 0;

char *__file;
int __line;

void done(char *comment)
{
	if (__done)
		return;

	__done = 1;
	_ntests += 1;
	_npassed += 1;

	printf(
		COLOR_BOLD COLOR_SUCCESS "✓ "
		COLOR_RESET COLOR_SUCCESS "Success: "
		COLOR_RESET COLOR_DESC "%s\n"
		COLOR_RESET, __description);
}

void fail(char *comment)
{
	if (__done)
		return;

	__done = 1;
	_ntests += 1;

	printf(
		COLOR_BOLD COLOR_FAIL "✕ "
		COLOR_RESET COLOR_FAIL "Failed: "
		COLOR_RESET COLOR_DESC "%s: "
		COLOR_RESET "%s\n", __description, comment);
	printf("    at %s:%i\n", __file, __line);
}
