#include "../vm.h"

#include <stdlib.h>

void l_vm_var_string_free(l_vm_var_string* str)
{
	free(str->chars);
	free(str);
}
