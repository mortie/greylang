#include "../../vm.h"

static void init(vm_var_function *func)
{
	func->parent = NULL;

	func->fptr = NULL;

	func->exprs = NULL;
	func->exprc = 0;

	func->argnames = NULL;
	func->argnamec = 0;
	func->scope = NULL;
	func->self = NULL;
	func->refs = 0;
}

void vm_var_function_init_self(
		vm_var_function *func,
		vm_var_function *parent,
		vm_var *self)
{
	init(func);
	func->parent = parent;
	func->self = self;
	vm_var_function_increfs(parent);
	vm_var_increfs(self);
}

void vm_var_function_init_fptr(
		vm_var_function *func,
		vm_var *(*fptr)(l_vm *vm, vm_var *self, vm_var_array *args, int infix))
{
	init(func);
	func->fptr = fptr;
}

void vm_var_function_init_scope(
		vm_var_function *func,
		vm_map *scope)
{
	init(func);
	func->scope = scope;
	vm_map_increfs(scope);
}

vm_var *vm_var_function_exec(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args,
		vm_var *self,
		int infix)
{
	// Find self if it exists
	if (self == NULL && func->self != NULL)
		self = func->self;

	// func->parent not being NULL means the function is just
	// a reference to another function, with a different self
	if (func->parent != NULL)
	{
		return vm_var_function_exec(
			vm, func->parent, args,
			func->self, infix);
	}

	// If args is null, just make an empty array
	vm_var_array fakeargs;
	if (args == NULL)
	{
		vm_var_array_init(&fakeargs, VAR_TYPE_NONE);
		args = &fakeargs;
	}
	else
	{
		vm_var_array_prepare(args);
	}

	// Just run the function pointer if that exists
	if (func->fptr != NULL)
	{
		return (*(func->fptr))(
			vm, func->self,
			args, infix);
	}

	// We don't want to free variables we aren't supposed to yet
	int prevoffset = vm->cleanup_offset;
	vm->cleanup_offset = vm->cleanupc;

	vm_map *scope = vm_map_create(func->scope);
	vm_map_increfs(scope);

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
	if (self)
		vm_map_set(scope, "self", self);

	// Execute expressions
	vm_var *var = vm_exec_exprs(vm, scope, func->exprs, func->exprc);
	var->refs += 1;
	vm_map_decrefs(scope);
	var->refs -= 1;

	vm->cleanup_offset = prevoffset;

	return var;
}

vm_var *vm_var_function_call(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args)
{
	return vm_var_function_exec(vm, func, args, NULL, 0);
}

void vm_var_function_free(vm_var_function *func)
{
	if (func->parent)
		vm_var_function_decrefs(func->parent);

	if (func->self)
		vm_var_decrefs(func->self);

	if (func->scope != NULL)
		vm_map_decrefs(func->scope);
}

void vm_var_function_increfs(vm_var_function *func)
{
	func->refs += 1;
}

void vm_var_function_decrefs(vm_var_function *func)
{
	func->refs -= 1;
	if (func->refs <= 0)
		vm_var_function_free(func);
}
