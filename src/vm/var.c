#include "../vm.h"

#include <stdlib.h>

l_vm_var* l_vm_var_create(l_vm_var_type type)
{
	l_vm_var* var = malloc(sizeof(l_vm_var));
	var->type = type;
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
	case VAR_TYPE_CHAR:
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
