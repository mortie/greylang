#include "../vm.h"

#include <stdlib.h>
#include <stdio.h>

l_vm *l_vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->base = vm_map_create(NULL);
	vm->base->immutable = 1;
	vm->root = vm_map_create(vm->base);

	vm->var_none = vm_var_create(VAR_TYPE_NONE);
	vm->var_true = vm_var_create(VAR_TYPE_BOOL);
	vm->var_true->var.boolean = 1;
	vm->var_false = vm_var_create(VAR_TYPE_BOOL);
	vm->var_false->var.boolean = 0;

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
