#include "../vm.h"

#include <stdlib.h>

void vm_var_array_init(vm_var_array *arr, vm_var_type type)
{
	arr->vars = NULL;
	arr->varc = 0;
	arr->allocd = 0;
	arr->type = type;
}

char *vm_var_array_tostring(vm_var_array *arr)
{
	char *str = malloc(strlen("[array]") + 1);
	strcpy(str, "[array]");
	return str;
}

void vm_var_array_set(vm_var_array *arr, int index, vm_var *var)
{
	if (index >= arr->allocd)
	{
		int oldallocd = arr->allocd;
		if (arr->allocd == 0)
		{
			arr->allocd = 4;
		}
		else
		{
			while (index >= arr->allocd)
				arr->allocd *= 2;
		}

		arr->vars = realloc(arr->vars, sizeof(*(arr->vars)) * arr->allocd);
		memset(arr->vars + oldallocd + 1, 0, arr->allocd - oldallocd);
	}
}
