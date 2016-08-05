#include "../vm.h"

#include <stdlib.h>

l_vm_var_array* l_vm_var_array_create()
{
	l_vm_var_array* arr = malloc(sizeof(l_vm_var_array));
	arr->vars = NULL;
	arr->len = 0;
	arr->allocd = 0;
	arr->gc_marked = 0;
	return arr;
}

void l_vm_var_array_resize(l_vm* vm, l_vm_var_array* arr, int nwsize)
{
	if (arr->len >= nwsize)
		return;

	if (arr->allocd < nwsize)
	{
		if (arr->allocd < 4)
			arr->allocd = 4;

		while (arr->allocd < nwsize)
		{
			arr->allocd *= 2;
		}

		arr->vars = realloc(arr->vars, sizeof(l_vm_var*) * arr->allocd);
		for (int i = arr->len - 1; i < arr->allocd; ++i)
		{
			arr->vars[i] = l_vm_var_create(vm, VAR_TYPE_NONE);
		}
	}

	arr->len = nwsize;
}

void l_vm_var_array_free(l_vm_var_array* arr)
{
	free(arr->vars);
	free(arr);
}
