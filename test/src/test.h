#ifndef TEST_H
#define TEST_H

#include <string.h>
#include <stdio.h>

/*
 * Example:
 *
 * void test_something() {
 *     it("does a") {
 *         asserteq(1, 1);
 *     }
 *
 *     it("does b") {
 *         assert(5 == 5);
 *     }
 *
 *     done();
 * }
 *
 * int main() {
 *     run(test_something);
 * }
 */

extern char __description[256];
extern int __done;
extern int _ntests;
extern int _npassed;

extern char *__file;
extern int __line;

void fail();
void done();

#define it(desc) \
	done(); \
	__file = __FILE__; \
	__line = __LINE__; \
	strncpy(__description, desc, sizeof(__description)); \
	__done = 0;

#define run(name) \
	do { \
		_ntests = 0; \
		_npassed = 0; \
		printf("\n\033[1mRunning %s:\033[0m\n\n", #name); \
		name(); \
		printf("\n\033[1mPassed %i/%i tests.\033[0m\n\n", _npassed, _ntests); \
	} while (0)

#define assert(x) \
	do { \
		if (!(x)) { \
			fail(#x); \
		} \
	} while (0)

#define asserteq_str(a, b) \
	do { \
		if (strcmp((a), (b)) != 0) { \
			fail("Expected " #a " to equal " #b); \
		} \
	} while (0)

#define asserteq(a, b) \
	do { \
		if ((a) != (b)) { \
			fail("Expected " #a " to equal " #b); \
		} \
	} while (0)

#endif
