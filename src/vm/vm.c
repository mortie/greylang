#include "../vm.h"
#include "../stdlib.h"

#include <stdlib.h>
#include <stdio.h>

l_vm *l_vm_create(char *currfile)
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->base = vm_map_create(NULL);
	vm->root = vm_map_create(vm->base);

	vm->cleanups = NULL;
	vm->cleanupc = 0;
	vm->cleanupa = 0;
	vm->cleanup_offset = 0;
	vm->currfile = currfile;
	vm->currline = 0;

	vm->var_none = vm_var_create(VAR_TYPE_NONE);
	vm->var_true = vm_var_create(VAR_TYPE_BOOL);
	vm->var_true->var.boolean = 1;
	vm->var_false = vm_var_create(VAR_TYPE_BOOL);
	vm->var_false->var.boolean = 0;

	vm_map_define(vm->base, "none", vm->var_none);
	vm_map_define(vm->base, "true", vm->var_true);
	vm_map_define(vm->base, "false", vm->var_false);

	/*
	 * Global Functions
	 */

#define STD(name, fn) \
	do { \
		vm_var_function *func = malloc(sizeof(*func)); \
		vm_var_function_init_fptr(func, fn); \
		vm_var *var = vm_var_create(VAR_TYPE_FUNCTION); \
		var->var.function = func; \
		vm_map_define(vm->base, name, var); \
	} while (0)

	// Flow Control
	STD("if", &vm_std_if);
	STD("repeat", &vm_std_repeat);
	STD("while", &vm_std_while);
	STD("for", &vm_std_for);

	// Special
	STD("extend", &vm_std_extend);
	STD("instanceof?", &vm_std_instanceof);
	STD("error", &vm_std_error);

	// Math
	STD("+", &vm_std_add);
	STD("-", &vm_std_sub);
	STD("*", &vm_std_mul);
	STD("/", &vm_std_div);
	STD("^", &vm_std_pow);

	// Logic
	STD("==", &vm_std_eq);
	STD("!=", &vm_std_neq);
	STD(">", &vm_std_gt);
	STD("<", &vm_std_lt);
	STD(">=", &vm_std_gteq);
	STD("<=", &vm_std_lteq);
	STD("and", &vm_std_and);
	STD("or", &vm_std_or);

	// Util
	STD("tostr", &vm_std_tostr);
	STD("tonum", &vm_std_tonum);
	STD("concat", &vm_std_concat);
	STD("..", &vm_std_concat);
	STD("?", &vm_std_which);
	STD("in?", &vm_std_in);

	// I/O
	STD("print", &vm_std_print);
	STD("read", &vm_std_read);

#undef STD

	/*
	 * Prototypes
	 */

#define PROTO(name, vname, p) \
	do { \
		vm_var *proto = vm_var_create(VAR_TYPE_OBJECT); \
		proto->map->parent = p; \
		vm->vname = proto; \
		vm_map_define(vm->base, name, proto); \
	} while (0);

#define PROTOFN(name, vname, fn) \
	do { \
		vm_var_function *func = malloc(sizeof(*func)); \
		vm_var_function_init_fptr(func, fn); \
		vm_var *v = vm_var_create(VAR_TYPE_FUNCTION); \
		var->var.function = func; \
		vm_map_define(var->map, name, v); \
	} while (0);

	PROTO("Object", proto_object, NULL);

	PROTO("Function", proto_function, vm->proto_object->map);

	PROTO("Array", proto_array, vm->proto_object->map);

	PROTO("String", proto_string, vm->proto_array->map);

	PROTO("Number", proto_number, vm->proto_object->map);

	PROTO("Bool", proto_bool, vm->proto_object->map);

	PROTO("Char", proto_char, vm->proto_object->map);

	PROTO("Error", proto_error, vm->proto_object->map);

	// none instanceof? Object should be false
	PROTO("None", proto_none, NULL);

#undef PROTO
#undef PROTOFN

	/*
	 * Load standard library
	 */

	{
		l_scanner *stream = l_scanner_create_str(l_stdlib_str);
		l_p_expr_list *exprs = l_parse(stream);
		vm_exec_exprs(vm, vm->base, exprs->expressions, exprs->expressionc);
	}

	vm->base->immutable = 1;

	return vm;
}

vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs)
{
	vm_var *var = vm_exec_exprs(
		vm, vm->root, exprs->expressions, exprs->expressionc);

	if (var->type == VAR_TYPE_ERROR)
	{
		vm_var_error_print(var->var.error, stderr);
		return vm->var_none;
	}

	return var;
}

vm_var *l_vm_error(l_vm *vm, char *msg)
{
	vm_var *var = vm_var_create(VAR_TYPE_ERROR);

	vm_var_error *err = malloc(sizeof(*err));
	vm_var_error_init(vm, err, msg);
	var->var.error = err;

	return var;
}

vm_var *l_vm_error_type(l_vm *vm, vm_var_type expected, vm_var *var)
{
	if (var == NULL)
		var = vm->var_none;

	char *expected_str = vm_var_type_string(expected);
	char *got_str = vm_var_type_string(var->type);
	const char *fmt = "Expected type %s, got type %s\n";

	int len = strlen(expected_str) + strlen(got_str) + strlen(fmt);

	char *msg = malloc(len + 1);
	sprintf(msg, fmt, expected_str, got_str);

	vm_var *err = l_vm_error(vm, msg);
	free(msg);
	return err;
}

void l_vm_cleanup_add(l_vm *vm, vm_var *var)
{
	vm->cleanupc += 1;
	if (vm->cleanupc >= vm->cleanupa)
	{
		if (vm->cleanupa == 0)
			vm->cleanupa = 4;
		else
			vm->cleanupa *= 2;

		vm->cleanups = realloc(
			vm->cleanups,
			sizeof(*(vm->cleanups)) * vm->cleanupa);
	}

	vm->cleanups[vm->cleanupc - 1] = var;
	vm_var_increfs(var);
}

void l_vm_cleanup(l_vm *vm)
{
	for (int i = vm->cleanup_offset; i < vm->cleanupc; ++i)
	{
		vm_var_decrefs(vm->cleanups[i]);
	}
	vm->cleanupc = vm->cleanup_offset;
}
