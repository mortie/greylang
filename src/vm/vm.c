#include "../vm.h"

#include <stdlib.h>

l_vm* l_vm_create()
{
	l_vm_scope* stdlib = l_vm_scope_create(NULL);

	l_vm* vm = malloc(sizeof(l_vm));
	vm->global = l_vm_scope_create(stdlib);

#define STD(str, ptr) \
	do { \
		l_vm_var_function* f = l_vm_var_function_create(stdlib); \
		f->fptr = ptr; \
		l_vm_var* v = l_vm_var_create(VAR_TYPE_FUNCTION); \
		v->var.function = f; \
		l_vm_scope_define(stdlib, str, v); \
	} while (0)

	STD("+", &l_vm_std_add);
	STD("-", &l_vm_std_sub);
	STD("*", &l_vm_std_mul);
	STD("/", &l_vm_std_div);
	STD("^", &l_vm_std_pow);
	STD("==", &l_vm_std_eq);
	STD("!=", &l_vm_std_neq);
	STD(">", &l_vm_std_gt);
	STD("<", &l_vm_std_lt);
	STD(">=", &l_vm_std_gteq);
	STD("<=", &l_vm_std_lteq);
	STD("and", &l_vm_std_and);
	STD("or", &l_vm_std_or);
	STD("if", &l_vm_std_if);
	STD("repeat", &l_vm_std_repeat);
	STD("map", &l_vm_std_map);
	STD("tostring", &l_vm_std_tostring);
	STD("tonumber", &l_vm_std_tonumber);
	STD("concat", &l_vm_std_concat);
	STD("..", &l_vm_std_concat);

	STD("loadc!", &l_vm_std_loadc);

	STD("print!", &l_vm_std_print);
	STD("read!", &l_vm_std_read);

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
