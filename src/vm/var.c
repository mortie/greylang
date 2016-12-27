#include "../vm.h"

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

	return a == b;
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
