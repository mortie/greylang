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

vm_var *vm_std_tonum(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 1, args);
	vm_var *arr = args->vars[0];

	if (
			arr->type != VAR_TYPE_ARRAY ||
			arr->var.array->type != VAR_TYPE_CHAR)
	{
		return l_vm_error(vm, "Invalid types");
	}

	vm_var *var = vm_var_create(VAR_TYPE_NUMBER);

	char *str = vm_var_char_array_to_utf8(arr->var.array);
	var->var.number = (double)atof(str);
	free(str);

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
		RETIFERR(args->vars[i]);
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

vm_var *vm_std_which(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 1)
		EXPECTARGC(vm, 1, args);

	for (int i = 0; i < args->varc; ++i)
	{
		RETIFERR(args->vars[i]);
		if (args->vars[i]->type != VAR_TYPE_NONE)
			return args->vars[i];
	}

	return vm->var_none;
}

vm_var *vm_std_in(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var *a = args->vars[0];
	vm_var *b = args->vars[1];
	OVERLOAD(vm, "$contains?", b, a);
}
