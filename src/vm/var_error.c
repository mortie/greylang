#include "../vm.h"

#include <stdlib.h>

static char* typestr(l_vm_var_type type)
{
	switch (type)
	{
	case VAR_TYPE_OBJECT:
		return "object";
	case VAR_TYPE_ARRAY:
		return "array";
	case VAR_TYPE_FUNCTION:
		return "function";
	case VAR_TYPE_STRING:
		return "string";
	case VAR_TYPE_NUMBER:
		return "number";
	case VAR_TYPE_BOOL:
		return "bool";
	case VAR_TYPE_ERROR:
		return "error";
	case VAR_TYPE_RETURN:
		return "return";
	case VAR_TYPE_PTR:
	case VAR_TYPE_NONE:
		return "none";
	}

	return "none";
}

l_vm_var_error* l_vm_var_error_create(l_vm* vm)
{
	l_vm_var_error* err = malloc(sizeof(l_vm_var_error));
	err->msg = NULL;
	err->line = vm->currLine;
	return err;
}

l_vm_var* l_vm_error(l_vm* vm, char* msg)
{
	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_ERROR);
	l_vm_var_error* err = l_vm_var_error_create(vm);
	var->var.error = err;

	int len = strlen(msg);
	err->msg = malloc(len);
	memcpy(err->msg, msg, len);
	err->line = vm->currLine;
	return var;
}

l_vm_var* l_vm_error_type(l_vm* vm, l_vm_var_type expected, l_vm_var_type got)
{
	char* format = "Unexpected type: Expected %s, got %s";
	int len = strlen(format) + (8 * 2); // 8 == strlen("function")

	char msg[len];
	snprintf(msg, len, format, typestr(expected), typestr(got));

	return l_vm_error(vm, msg);
}

l_vm_var* l_vm_error_argc(l_vm* vm, int expected, int got)
{
	char* format = "Unexpected argument count: Expected %i, got %i";
	int len = strlen(format) + (4 * 2); // assuming no more than 9999 args...

	char msg[len];
	snprintf(msg, len, format, expected, got);

	return l_vm_error(vm, msg);
}
