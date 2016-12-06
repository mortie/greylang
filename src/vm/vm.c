#include "../vm.h"

#include <stdlib.h>

l_vm *l_vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->root = vm_map_create(NULL);
	vm->root->immutable = 1;

	return vm;
}

vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs)
{
	return NULL;
}
