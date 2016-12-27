#include "../vm.h"

vm_var *vm_std_test(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	return vm->var_true;
}
