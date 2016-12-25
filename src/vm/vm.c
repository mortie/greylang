#include "../vm.h"

#include <stdlib.h>

l_vm *l_vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->root = vm_map_create(NULL);
	vm->root->immutable = 1;

	vm->var_none = vm_var_create(VAR_TYPE_NONE);
	vm->var_true = vm_var_create(VAR_TYPE_BOOL);
	vm->var_true->var.boolean = 1;
	vm->var_false = vm_var_create(VAR_TYPE_BOOL);
	vm->var_false->var.boolean = 0;

	return vm;
}

vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs)
{
	return NULL;
}
