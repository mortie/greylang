#include "../../vm.h"
#include "defines.h"

#include <stdlib.h>

vm_var *vm_std_new(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 1)
		EXPECTARGC(vm, 1, args);

	vm_var *proto = args->vars[0];
	vm_var *obj;

	if (args->varc >= 2)
		obj = args->vars[1];
	else
		obj = vm_var_create(VAR_TYPE_OBJECT);

	obj->map->parent = proto->map;

	// Call init if it exist
	vm_var *init = vm_map_lookup_r(obj->map, "$init");
	if (init != NULL)
	{
		EXPECTTYPE(vm, VAR_TYPE_FUNCTION, init);

		vm_var_function initFn;
		vm_var_function_init_self(&initFn, init->var.function, obj);

		vm_var_function_exec(vm, &initFn, NULL, NULL, 0);

		vm_var_function_free(&initFn);
	}

	return obj;
}
