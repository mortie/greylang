#include "../../vm.h"
#include "defines.h"

#include "../../platform.h"

#include <stdlib.h>

vm_var *vm_std_print(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	for (int i = 0; i < args->varc; ++i)
	{
		char *str = vm_var_tostring(args->vars[i]);
		if (i == 0)
			printf("%s", str);
		else
			printf(" %s", str);
		free(str);
	}
	printf("\n");

	return vm->var_none;
}

vm_var *vm_std_read(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	char* prompt;
	if (args->varc == 0)
	{
		prompt = "";
	}
	else if (args->varc == 1)
	{
		EXPECTTYPE(vm, VAR_TYPE_ARRAY, args->vars[0]);
		if (args->vars[0]->var.array->type != VAR_TYPE_CHAR)
			return l_vm_error(vm, "Array not character array");

		prompt = vm_var_char_array_to_utf8(args->vars[0]->var.array);
	}
	else
	{
		EXPECTARGC(vm, 1, args);
		return NULL;
	}

	vm_var_array *arr = malloc(sizeof(*arr));
	vm_var_array_init(arr, VAR_TYPE_CHAR);

	char* s = l_plat_readline(prompt);
	vm_var_char_array_from_utf8(s, arr);

	free(s);
	if (args->varc == 1)
		free(prompt);

	vm_var *var = vm_var_create(VAR_TYPE_ARRAY);
	var->var.array = arr;

	return var;
}
