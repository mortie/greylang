#include "../vm.h"

vm_var *vm_std_tostring(vm_var *var)
{
	return vm_var_create(VAR_TYPE_NONE);
}
