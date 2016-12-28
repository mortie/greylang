#include "../../vm.h"

#include <stdlib.h>
#include <math.h>

vm_var *vm_var_create(vm_var_type type)
{
	vm_var *var = malloc(sizeof(*var));

	var->type = type;
	var->refs = 0;

	var->map = vm_map_create(NULL);

	return var;
}

static void freevar(vm_var *var)
{
	vm_map_free(var->map);
	free(var);
}

void vm_var_increfs(vm_var *var)
{
	var->refs += 1;
}

void vm_var_decrefs(vm_var *var)
{
	var->refs -= 1;
	if (var->refs <= 0)
		freevar(var);
}

int vm_var_equals(vm_var *a, vm_var *b)
{
	if (a->type != b->type)
		return 0;

	switch (a->type)
	{
	case VAR_TYPE_OBJECT:
		return a == b;

	case VAR_TYPE_ARRAY:
	{
		vm_var_array *arra = a->var.array;
		vm_var_array *arrb = b->var.array;

		if (arra->varc != arrb->varc)
			return 0;

		for (int i = 0; i < arra->varc; ++i)
		{
			if (!vm_var_equals(arra->vars[i], arrb->vars[i]))
				return 0;
		}

		return 1;
	}

	case VAR_TYPE_FUNCTION:
		return a == b;

	case VAR_TYPE_NUMBER:
		return a->var.number == b->var.number;

	case VAR_TYPE_BOOL:
		return !!(a->var.boolean) == !!(b->var.boolean);

	case VAR_TYPE_CHAR:
		return a->var.character == b->var.character;

	case VAR_TYPE_NONE:
		return 1;
	}

	return 0;
}

char *vm_var_tostring(vm_var *var)
{
	char *str = "";

	switch (var->type)
	{
	case VAR_TYPE_OBJECT:
		str = "[object]";
		break;

	case VAR_TYPE_ARRAY:
	{
		return vm_var_array_tostring(var->var.array);
	}

	case VAR_TYPE_FUNCTION:
		str = "[function]";
		break;

	case VAR_TYPE_NUMBER:
	{
		char *ret = malloc(16);
		double num = var->var.number;
		if (num == floor(num))
			snprintf(ret, 15, "%d", (int)num);
		else
			snprintf(ret, 15, "%f", num);

		return ret;
	}

	case VAR_TYPE_BOOL:
		if (var->var.boolean)
			str = "[true]";
		else
			str = "[false]";

		break;

	case VAR_TYPE_CHAR:
	{
		char *ret = malloc(5);
		vm_var_char_to_utf8(var->var.character, ret);
		return ret;
	}

	case VAR_TYPE_NONE:
		str = "[none]";
		break;
	}

	char *ret = malloc(strlen(str) + 1);
	strcpy(ret, str);
	return ret;
}
