#include "../vm.h"

#include "../stdlib.h"

#include <stdlib.h>

l_vm* l_vm_create()
{
	l_vm_scope* stdlib = l_vm_scope_create(NULL);

	l_vm* vm = malloc(sizeof(l_vm));
	vm->currLine = 0;
	vm->global = l_vm_scope_create(stdlib);

#define STD(str, ptr) \
	do { \
		l_vm_var_function* f = l_vm_var_function_create(stdlib); \
		f->fptr = ptr; \
		l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_FUNCTION); \
		v->var.function = f; \
		l_vm_scope_define(stdlib, str, v); \
	} while (0)

	/*
	 * Global Functions
	 */

	STD("new", &l_vm_std_new);

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
	STD("&&", &l_vm_std_and);
	STD("||", &l_vm_std_or);

	STD("if", &l_vm_std_if);
	STD("repeat", &l_vm_std_repeat);
	STD("while", &l_vm_std_while);
	STD("for", &l_vm_std_for);

	STD("tostring", &l_vm_std_tostring);
	STD("tonumber", &l_vm_std_tonumber);
	STD("concat", &l_vm_std_concat);
	STD("..", &l_vm_std_concat);

	STD("error", &l_vm_std_error);
	STD("type", &l_vm_std_type);

	STD("loadc", &l_vm_std_loadc);

	STD("print", &l_vm_std_print);
	STD("read", &l_vm_std_read);

#undef STD
#define PROTO(obj, str, ptr) \
	do { \
		l_vm_var_function* f = l_vm_var_function_create(NULL); \
		f->fptr = ptr; \
		l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_FUNCTION); \
		v->var.function = f; \
		l_vm_map_set(obj, str, v); \
	} while (0)

	l_vm_map* proto_array = l_vm_map_create(NULL);
	vm->proto_array = proto_array;

	PROTO(proto_array, "len", &l_vm_std_array_len);
	PROTO(proto_array, "push", &l_vm_std_array_push);
	PROTO(proto_array, "pop", &l_vm_std_array_pop);
	PROTO(proto_array, "map", &l_vm_std_array_map);
	PROTO(proto_array, "iter", &l_vm_std_array_iter);

	l_vm_map* proto_string = l_vm_map_create(NULL);
	vm->proto_string = proto_string;

	PROTO(proto_string, "len", &l_vm_std_string_len);
	PROTO(proto_string, "sub", &l_vm_std_string_sub);
	PROTO(proto_string, "iter", &l_vm_std_string_iter);

#undef PROTO

	/*
	 * Global Variables
	 */

	l_vm_var* var_none = l_vm_var_create(vm, VAR_TYPE_NONE);
	l_vm_scope_define(stdlib, "none", var_none);

	l_vm_var* var_true = l_vm_var_create(vm, VAR_TYPE_BOOL);
	var_true->var.boolean = 1;
	l_vm_scope_define(stdlib, "true", var_true);

	l_vm_var* var_false = l_vm_var_create(vm, VAR_TYPE_BOOL);
	var_false->var.boolean = 0;
	l_vm_scope_define(stdlib, "false", var_false);

	/*
	 * Load Standard Library
	 */

	l_scanner* stream = l_scanner_create_str(l_stdlib_str);
	l_vm_run(vm, l_parse(stream));

	return vm;
}
