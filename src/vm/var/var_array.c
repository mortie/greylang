#include "../../vm.h"

#include <stdlib.h>

void vm_var_array_init(vm_var_array *arr, vm_var_type type)
{
	arr->vars = NULL;
	arr->varc = 0;
	arr->allocd = 0;
	arr->type = type;

	arr->ready = 1;
	arr->strcache = NULL;
}

char *vm_var_array_tostring(vm_var_array *arr)
{
	if (arr->type == VAR_TYPE_CHAR)
	{
		return vm_var_char_array_to_utf8(arr);
	}
	else
	{
		char *str = malloc(strlen("[array]") + 1);
		strcpy(str, "[array]");
		return str;
	}
}

void vm_var_array_prepare(vm_var_array *arr)
{
	if (!arr->ready && arr->type == VAR_TYPE_CHAR)
	{
		vm_var_char_array_from_utf8(arr->strcache, arr);
		arr->ready = 1;
	}
}

int vm_var_array_set(vm_var_array *arr, int index, vm_var *var)
{
	if (
			arr->type != VAR_TYPE_NONE &&
			var->type != arr->type)
	{
		return -1;
	}

	if (arr->strcache != NULL && arr->ready)
	{
		free(arr->strcache);
		arr->strcache = NULL;
	}

	if (index >= arr->allocd)
	{
		int oldallocd = arr->allocd;

		if (arr->allocd == 0)
			arr->allocd = 4;

		while (index >= arr->allocd)
			arr->allocd *= 2;

		arr->vars = realloc(arr->vars, sizeof(*(arr->vars)) * arr->allocd);
		memset(arr->vars + oldallocd, 0,
			(arr->allocd - oldallocd) * sizeof(*(arr->vars)));
	}

	if (arr->allocd > arr->varc && index >= arr->varc)
		arr->varc = index + 1;

	if (arr->vars[index] != NULL)
		vm_var_decrefs(arr->vars[index]);

	vm_var_increfs(var);
	arr->vars[index] = var;

	return 0;
}

vm_var *vm_var_array_get(vm_var_array *arr, int index)
{
	if (index >= arr->varc)
		return NULL;

	return arr->vars[index];
}

void vm_var_array_free(vm_var_array *arr)
{
	for (int i = 0; i < arr->varc; ++i)
	{
		if (arr->vars[i] != NULL)
			vm_var_decrefs(arr->vars[i]);
	}

	free(arr->vars);
	free(arr->strcache);
}
