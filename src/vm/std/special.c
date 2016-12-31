#include "../../vm.h"
#include "defines.h"

#include <stdlib.h>

vm_var *vm_std_extend(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);

	vm_var *proto = args->vars[0];
	vm_var *obj = args->vars[1];

	obj->map->parent = proto->map;

	return obj;
}
