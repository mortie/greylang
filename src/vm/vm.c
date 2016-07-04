#include "../vm.h"

#include <stdlib.h>

l_vm* l_vm_create()
{
	l_vm_scope* stdlib = l_vm_scope_create(NULL);

	l_vm* vm = malloc(sizeof(l_vm));
	vm->global = l_vm_scope_create(stdlib);

#define STD(str, std) \
	do { \
		l_vm_var_function* f = l_vm_var_function_create_std( \
			stdlib, std); \
		l_vm_var* v = l_vm_var_create(VAR_TYPE_FUNCTION); \
		v->var.function = f; \
		l_vm_scope_define(stdlib, str, v); \
	} while (0)

	STD("+", STD_FUNC_ADD);
	STD("-", STD_FUNC_SUB);
	STD("*", STD_FUNC_MUL);
	STD("/", STD_FUNC_DIV);
	STD("^", STD_FUNC_POW);
	STD("==", STD_FUNC_EQ);
	STD("!=", STD_FUNC_NEQ);
	STD(">", STD_FUNC_GT);
	STD("<", STD_FUNC_LT);
	STD(">=", STD_FUNC_GTEQ);
	STD("<=", STD_FUNC_LTEQ);
	STD("and", STD_FUNC_AND);
	STD("or", STD_FUNC_OR);
	STD("if", STD_FUNC_IF);
	STD("repeat", STD_FUNC_REPEAT);
	STD("map", STD_FUNC_MAP);
	STD("tostring", STD_FUNC_TOSTRING);
	STD("tonumber", STD_FUNC_TONUMBER);
	STD("concat", STD_FUNC_CONCAT);
	STD("print!", STD_FUNC_PRINT);
	STD("read!", STD_FUNC_READ);

#undef STD

	l_vm_var* var_true = l_vm_var_create(VAR_TYPE_BOOL);
	var_true->var.boolean = 1;
	l_vm_scope_define(stdlib, "true", var_true); \

	l_vm_var* var_false = l_vm_var_create(VAR_TYPE_BOOL);
	var_false->var.boolean = 0;
	l_vm_scope_define(stdlib, "false", var_false); \

	return vm;
}

static char* l_vm_var_type_string(l_vm_var_type type)
{
	switch (type)
	{
	case VAR_TYPE_ARRAY:
		return "array";
	case VAR_TYPE_FUNCTION:
		return "function";
	case VAR_TYPE_STRING:
		return "string";
	case VAR_TYPE_CHAR:
		return "char";
	case VAR_TYPE_NUMBER:
		return "number";
	case VAR_TYPE_BOOL:
		return "bool";
	default:
		return "none";
	}
}

void l_vm_error_type(l_vm_var_type expected, l_vm_var_type got)
{
	fprintf(stderr, "Expected type %s, got %s\n",
		l_vm_var_type_string(expected),
		l_vm_var_type_string(got));
	exit(1);
}

void l_vm_error_argnum(int expected, int got)
{
	fprintf(stderr, "Expected %i arguments, got %i\n",
		expected, got);
	exit(1);
}

void l_vm_error_undefined(char* name)
{
	fprintf(stderr, "Undefined variable: %s\n", name);
	exit(1);
}
