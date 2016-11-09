#include "../vm.h"

#include <stdlib.h>

l_vm *vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));
	vm->root = vm_map_create(NULL);
	vm->root->immutable = 1;
}
