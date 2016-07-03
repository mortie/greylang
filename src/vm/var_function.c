#include "../vm.h"

#include <stdlib.h>

l_vm_var_function* l_vm_var_function_create(
		l_vm_scope* scope)
{
	l_vm_var_function* func = malloc(sizeof(l_vm_var_function));

	func->expressions = NULL;
	func->stdfunc = STD_FUNC_NONE;
	func->scope = scope;

	return func;
}

l_vm_var_function* l_vm_var_function_create_std(
		l_vm_scope* scope,
		l_vm_std_func stdfunc)
{
	l_vm_var_function* func = l_vm_var_function_create(scope);
	func->stdfunc = stdfunc;
	return func;
}

l_vm_var* l_vm_var_function_exec(
		l_vm_var_function* func,
		l_vm_var_array* args)
{

	if (func->stdfunc == STD_FUNC_NONE)
	{
		l_vm_scope* scope = l_vm_scope_create(func->scope);

		l_vm_var* var = l_vm_exec(scope, func->expressions, func->expressionc);
		l_vm_scope_free(scope);
		return var;
	}

	switch (func->stdfunc)
	{
	case STD_FUNC_NONE:
	{
		l_vm_scope* scope = l_vm_scope_create(func->scope);
		scope->parent = func->scope;

		l_vm_var* var = l_vm_exec(scope, func->expressions, func->expressionc);
		l_vm_scope_free(scope);
		return var;
	}
	case STD_FUNC_ADD:
		return l_vm_std_add(args);
	case STD_FUNC_SUB:
		return l_vm_std_sub(args);
	case STD_FUNC_MUL:
		return l_vm_std_mul(args);
	case STD_FUNC_DIV:
		return l_vm_std_div(args);
	case STD_FUNC_POW:
		return l_vm_std_pow(args);
	case STD_FUNC_LOOKUP:
		return l_vm_std_lookup(args);
	case STD_FUNC_IF:
		return l_vm_std_if(args);
	case STD_FUNC_REPEAT:
		return l_vm_std_repeat(args);
	case STD_FUNC_MAP:
		return l_vm_std_map(args);
	case STD_FUNC_PRINT:
		return l_vm_std_print(args);
	case STD_FUNC_READ:
		return l_vm_std_read(args);
	}

	return l_vm_var_create(VAR_TYPE_NONE);
}
