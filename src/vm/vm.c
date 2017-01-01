#include "../vm.h"
#include "../stdlib.h"

#include <stdlib.h>
#include <stdio.h>

l_vm *l_vm_create()
{
	l_vm *vm = malloc(sizeof(*vm));

	vm->base = vm_map_create(NULL);
	vm->root = vm_map_create(vm->base);

	vm->cleanups = NULL;
	vm->cleanupc = 0;
	vm->cleanupa = 0;
	vm->cleanup_offset = 0;
	vm->currline = 0;

	vm->var_none = vm_var_create(VAR_TYPE_NONE);
	vm->var_true = vm_var_create(VAR_TYPE_BOOL);
	vm->var_true->var.boolean = 1;
	vm->var_false = vm_var_create(VAR_TYPE_BOOL);
	vm->var_false->var.boolean = 0;

	vm_map_define(vm->base, "none", vm->var_none);
	vm_map_define(vm->base, "true", vm->var_true);
	vm_map_define(vm->base, "false", vm->var_false);

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
	STD("?", &vm_std_which);

	// Util
	STD("tostr", &vm_std_tostr);
	STD("tonum", &vm_std_tonum);
	STD("concat", &vm_std_concat);
	STD("..", &vm_std_concat);

	// I/O
	STD("print", &vm_std_print);
	STD("read", &vm_std_read);

#undef STD

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
	return vm_exec_exprs(vm, vm->root, exprs->expressions, exprs->expressionc);
}

// TODO: better error handling
vm_var *l_vm_error(l_vm *vm, char *msg)
{
	printf("Error at line %i: %s\n", vm->currline, msg);
	return vm->var_none;
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
