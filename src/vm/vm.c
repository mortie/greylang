#include "../vm.h"

#include <stdlib.h>
#include <stdio.h>

l_vm *l_vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->base = vm_map_create(NULL);
	vm->root = vm_map_create(vm->base);

	vm->var_none = vm_var_create(VAR_TYPE_NONE);
	vm->var_true = vm_var_create(VAR_TYPE_BOOL);
	vm->var_true->var.boolean = 1;
	vm->var_false = vm_var_create(VAR_TYPE_BOOL);
	vm->var_false->var.boolean = 0;

	vm_map_define(vm->base, "none", vm->var_none);
	vm_map_define(vm->base, "true", vm->var_true);
	vm_map_define(vm->base, "false", vm->var_false);

#define STD(name, fn) \
	do { \
		vm_var_function *func = malloc(sizeof(*func)); \
		vm_var_function_init(func, NULL); \
		func->fptr = fn; \
		vm_var *var = vm_var_create(VAR_TYPE_FUNCTION); \
		var->var.function = func; \
		vm_map_define(vm->base, name, var); \
	} while (0)

	// Math
	STD("+", &vm_std_add);
	STD("-", &vm_std_sub);
	STD("*", &vm_std_mul);
	STD("/", &vm_std_div);
	STD("^", &vm_std_pow);

	// Logic
	STD("==", &vm_std_eq);
	STD("!=", &vm_std_neq);
	STD(">", &vm_std_gt);
	STD("<", &vm_std_lt);
	STD(">=", &vm_std_gteq);
	STD("<=", &vm_std_lteq);
	STD("and", &vm_std_and);
	STD("or", &vm_std_or);

	// Util
	STD("tostr", &vm_std_tostr);
	STD("concat", &vm_std_concat);
	STD("..", &vm_std_concat);

#undef STD

	vm->base->immutable = 1;

	return vm;
}

vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs)
{
	vm_var *ret = vm->var_none;
	for (int i = 0; i < exprs->expressionc; ++i)
	{
		ret = vm_exec(vm, vm->root, exprs->expressions[i]);
	}
	return ret;
}

// TODO: better error handling
vm_var *l_vm_error(l_vm *vm, char *msg)
{
	printf("Error: %s\n", msg);
	return vm->var_none;
}
