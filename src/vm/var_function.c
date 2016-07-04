#include "../vm.h"

#include <stdlib.h>
#include <stdio.h>

l_vm_var_function* l_vm_var_function_create(
		l_vm_scope* scope)
{
	l_vm_var_function* func = malloc(sizeof(l_vm_var_function));

	func->expressions = NULL;
	func->stdfunc = STD_FUNC_NONE;
	func->scope = scope;
	func->argnames = NULL;
	func->argnamec = 0;

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
	int fakeargs = 0;
	if (args == NULL)
	{
		fakeargs = 1;
		args = malloc(sizeof(l_vm_var_array));
		args->vars = NULL;
		args->len = 0;
		args->allocd = 0;
	}

	l_vm_var* ret;

	switch (func->stdfunc)
	{

	/*
	 * User Defined Function
	 */

	case STD_FUNC_NONE:
	{
		l_vm_scope* scope = l_vm_scope_create(func->scope);
		scope->parent = func->scope;

		if (func->argnames != NULL)
		{
			if (args->len != func->argnamec)
			{
				l_vm_error_argnum(func->argnamec, args->len);
			}

			for (int i = 0; i < args->len; ++i)
			{
				l_vm_scope_define(scope, func->argnames[i], args->vars[i]);
			}
		}

		for (int i = 0; i < args->len; ++i)
		{
			char name[16];
			name[0] = '$';
			snprintf(name + 1, 15, "%d", i + 1);
			l_vm_scope_define(scope, name, args->vars[i]);
		}

		ret = l_vm_exec(scope, func->expressions, func->expressionc);
		break;
	}

	/*
	 * Predefined Functions
	 */

	case STD_FUNC_ADD:
		ret = l_vm_std_add(args);
		break;
	case STD_FUNC_SUB:
		ret = l_vm_std_sub(args);
		break;
	case STD_FUNC_MUL:
		ret = l_vm_std_mul(args);
		break;
	case STD_FUNC_DIV:
		ret = l_vm_std_div(args);
		break;
	case STD_FUNC_POW:
		ret = l_vm_std_pow(args);
		break;
	case STD_FUNC_EQ:
		ret = l_vm_std_eq(args);
		break;
	case STD_FUNC_NEQ:
		ret = l_vm_std_neq(args);
		break;
	case STD_FUNC_GT:
		ret = l_vm_std_gt(args);
		break;
	case STD_FUNC_LT:
		ret = l_vm_std_lt(args);
		break;
	case STD_FUNC_GTEQ:
		ret = l_vm_std_gteq(args);
		break;
	case STD_FUNC_LTEQ:
		ret = l_vm_std_lteq(args);
		break;
	case STD_FUNC_AND:
		ret = l_vm_std_and(args);
		break;
	case STD_FUNC_OR:
		ret = l_vm_std_or(args);
		break;
	case STD_FUNC_IF:
		ret = l_vm_std_if(args);
		break;
	case STD_FUNC_REPEAT:
		ret = l_vm_std_repeat(args);
		break;
	case STD_FUNC_MAP:
		ret = l_vm_std_map(args);
		break;
	case STD_FUNC_TOSTRING:
		ret = l_vm_std_tostring(args);
		break;
	case STD_FUNC_TONUMBER:
		ret = l_vm_std_tonumber(args);
		break;
	case STD_FUNC_CONCAT:
		ret = l_vm_std_concat(args);
		break;
	case STD_FUNC_PRINT:
		ret = l_vm_std_print(args);
		break;
	case STD_FUNC_READ:
		ret = l_vm_std_read(args);
		break;
	default:
		ret = l_vm_var_create(VAR_TYPE_NONE);
		break;
	}

	if (fakeargs)
	{
		free(args);
	}

	return ret;
}
