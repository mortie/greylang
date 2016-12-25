#include "../vm.h"

void vm_var_function_init(vm_var_function *func)
{
	func->fptr = NULL;

	func->exprs = NULL;
	func->exprc = 0;

	func->argnames = NULL;
	func->argnamec = 0;
	func->self = NULL;
}

vm_var *vm_var_function_exec(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args,
		int infix)
{
	// TODO: execute function

	return vm->var_none;
}
