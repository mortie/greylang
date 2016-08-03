#include "../vm.h"

#include <stdlib.h>
#include <math.h>

l_vm_var* l_vm_var_create(l_vm* vm, l_vm_var_type type)
{
	l_vm_var* var = malloc(sizeof(l_vm_var));
	var->type = type;

	switch (type)
	{
	case VAR_TYPE_ARRAY:
		var->map = l_vm_map_create(vm->proto_array);
		break;
	case VAR_TYPE_STRING:
		var->map = l_vm_map_create(vm->proto_string);
		break;
	default:
		var->map = l_vm_map_create(NULL);
	}

	return var;
}

void l_vm_var_clean(l_vm_var* var)
{
	switch (var->type)
	{
	case VAR_TYPE_OBJECT:
		free(var->var.object);
		break;
	case VAR_TYPE_ARRAY:
		free(var->var.array);
		break;
	case VAR_TYPE_FUNCTION:
		l_vm_scope_free(var->var.function->scope);
		free(var->var.function);
		break;
	case VAR_TYPE_STRING:
		free(var->var.string);
		break;
	case VAR_TYPE_ERROR:
		free(var->var.error);
		break;
	case VAR_TYPE_NUMBER:
	case VAR_TYPE_BOOL:
	case VAR_TYPE_PTR:
	case VAR_TYPE_NONE:
		break;
	}
}

void l_vm_var_free(l_vm_var* var)
{
	l_vm_var_clean(var);
	free(var);
}

char* l_vm_var_tostring(l_vm_var* var)
{
	char* str = "";

	switch (var->type)
	{
	case VAR_TYPE_OBJECT:
		str = "[object]";
		break;
	case VAR_TYPE_ARRAY:
		str = "[array]";
		break;
	case VAR_TYPE_FUNCTION:
		str = "[function]";
		break;
	case VAR_TYPE_STRING:
		return var->var.string->chars;
		break;
	case VAR_TYPE_NUMBER:
	{
		str = malloc(16);
		double num = var->var.number;
		if (num == floor(num))
		{
			snprintf(str, 16, "%d", (int)num);
		}
		else
		{
			snprintf(str, 16, "%f", num);
		}
		break;
	}
	case VAR_TYPE_BOOL:
		if (var->var.boolean)
		{
			str = "[true]";
		}
		else
		{
			str = "[false]";
		}
		break;
	case VAR_TYPE_ERROR:
	{
		l_vm_var_error* err = var->var.error;

		char* format = "Error: Line %i: %s";
		char* msg = err->msg;

		// Assuming no more than 99999999 lines
		int len = strlen(format) + strlen(msg) + 8;

		str = malloc(len);
		snprintf(str, len, format, err->line, err->msg);
		break;
	}
	case VAR_TYPE_PTR:
	case VAR_TYPE_NONE:
		str = "[none]";
		break;
	}

	return str;
}

int l_vm_var_eq(l_vm_var* var1, l_vm_var* var2)
{
	if (var1->type != var2->type)
	{
		return 0;
	}

	switch (var1->type)
	{
	case VAR_TYPE_OBJECT:
		return var1->var.object == var2->var.object;
	case VAR_TYPE_ARRAY:
		return var1->var.array == var2->var.array;
	case VAR_TYPE_FUNCTION:
		return var1->var.function == var2->var.function;
	case VAR_TYPE_STRING:
		return strcmp(
			var1->var.string->chars,
			var2->var.string->chars) == 0;
	case VAR_TYPE_NUMBER:
		return var1->var.number == var2->var.number;
	case VAR_TYPE_BOOL:
		return var1->var.boolean == var2->var.boolean;
	case VAR_TYPE_ERROR:
		return var1->var.error == var2->var.error;
	case VAR_TYPE_PTR:
	case VAR_TYPE_NONE:
		return 1;
	}

	return 0;
}
