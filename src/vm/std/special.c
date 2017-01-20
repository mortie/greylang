#include "../../vm.h"
#include "defines.h"

#include <stdlib.h>

vm_var *vm_std_extend(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);

	vm_var *proto = args->vars[0];
	vm_var *obj = args->vars[1];
	RETIFERR(proto);
	RETIFERR(obj);

	obj->map->parent = proto->map;

	return obj;
}

vm_var *vm_std_instanceof(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);

	RETIFERR(args->vars[0]);
	RETIFERR(args->vars[1]);
	vm_map *a = args->vars[0]->map;
	vm_map *b = args->vars[1]->map;

	while (a != NULL)
	{
		if (a == b)
			return vm->var_true;
		else
			a = a->parent;
	}

	return vm->var_false;
}

vm_var *vm_std_error(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 1, args);
	vm_var *msg = args->vars[0];
	RETIFERR(msg);

	char *str = vm_var_tostring(msg);
	vm_var *err = l_vm_error(vm, str);
	free(str);
	return err;
}
