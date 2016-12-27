#include "src/test.h"

void test_vm_array();
void test_vm_map();
void test_vm_char();

int main()
{
	run(test_vm_array);
	run(test_vm_map);
	run(test_vm_char);

	return 0;
}
