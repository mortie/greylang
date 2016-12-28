#include "../../vm.h"
#include "defines.h"

#include <stdlib.h>

vm_var *vm_std_tostr(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 1, args);

	char *str = vm_var_tostring(args->vars[0]);
	vm_var_array *arr = malloc(sizeof(*arr));
	vm_var_array_init(arr, VAR_TYPE_CHAR);
	vm_var_char_array_from_utf8(str, arr);
	free(str);

	vm_var *var = vm_var_create(VAR_TYPE_ARRAY);
	var->var.array = arr;
	return var;
}

vm_var *vm_std_concat(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	char *strs[args->varc];

	int len = 1;
	for (int i = 0; i < args->varc; ++i)
	{
		strs[i] = vm_var_tostring(args->vars[i]);
		len += strlen(strs[i]);
	}

	char str[len];
	int total = 0;
	for (int i = 0; i < args->varc; ++i)
	{
		int l = strlen(strs[i]);
		memcpy(str + total, strs[i], l);
		total +=  l;
	}

	str[len - 1] = '\0';

	vm_var* var = vm_var_create(VAR_TYPE_ARRAY);
	vm_var_array *arr = malloc(sizeof(*arr));
	vm_var_array_init(arr, VAR_TYPE_CHAR);
	vm_var_char_array_from_utf8(str, arr);
	var->var.array = arr;

	return var;
}
