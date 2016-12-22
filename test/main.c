#include "src/test.h"

void test_vm_array();
void test_vm_map();

int main()
{
	run(test_vm_array);
	run(test_vm_map);

	return 0;
}
