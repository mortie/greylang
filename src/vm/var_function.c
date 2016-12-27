#include "../vm.h"

void vm_var_function_init(vm_var_function *func, vm_map *scope)
{
	func->fptr = NULL;

	func->exprs = NULL;
	func->exprc = 0;

	func->argnames = NULL;
	func->argnamec = 0;
	func->scope = scope;
	func->self = NULL;
}

vm_var *vm_var_function_exec(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args,
		int infix)
{
	// If args is null, just make an empty array
	vm_var_array fakeargs;
	if (args == NULL)
	{
		vm_var_array_init(&fakeargs, VAR_TYPE_NONE);
		args = &fakeargs;
	}

	// Just run the function pointer if that exists
	if (func->fptr != NULL)
	{
		return (*(func->fptr))(
			vm, func->self,
			args, infix);
	}

	vm_map *scope = vm_map_create(func->scope);

	// Define $n variables
	for (int i = 0; i < args->varc; ++i)
	{
		char name[16];
		name[0] = '$';
		snprintf(name + 1, 15, "%d", i + 1);
		vm_map_set(scope, name, args->vars[i]);
	}

	// Define variables
	if (func->argnames != NULL)
	{
		for (int i = 0; i < args->varc && i < func->argnamec; ++i)
		{
			vm_map_set(scope, func->argnames[i], args->vars[i]);
		}
	}

	// Define infix?
	if (infix)
		vm_map_set(scope, "infix?", vm->var_true);
	else
		vm_map_set(scope, "infix?", vm->var_false);

	// Define self
	vm_var *self;
	if (func->self == NULL)
		self = vm->var_none;
	else
		self = func->self;
	vm_map_set(scope, "self", self);

	// Execute expressions and return the result
	vm_var *ret = vm->var_none;
	for (int i = 0; i < func->exprc; ++i)
	{
		ret = vm_exec(vm, scope, func->exprs[i]);
	}
	return ret;
}
