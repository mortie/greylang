#include "src/test.h"

void test_foo();
void test_bar();

int main()
{
	run(test_foo);
	run(test_bar);

	return 0;
}
