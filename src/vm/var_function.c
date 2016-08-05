#include "../vm.h"

#include <stdlib.h>
#include <stdio.h>

l_vm_var_function* l_vm_var_function_create(
		l_vm_scope* scope)
{
	l_vm_var_function* func = malloc(sizeof(l_vm_var_function));

	func->fptr = NULL;
	func->expressions = NULL;
	func->scope = scope;
	func->argnames = NULL;
	func->argnamec = 0;
	func->self = NULL;

	return func;
}

l_vm_var* l_vm_var_function_exec(
		l_vm* vm,
		l_vm_var_function* func,
		l_vm_var_array* args,
		int infix)
{
	int fakeargs = 0;
	if (args == NULL)
	{
		fakeargs = 1;
		args = l_vm_var_array_create();
	}

	l_vm_var* ret;

	// Function from function pointer
	if (func->fptr != NULL)
	{
		ret = (*func->fptr)(vm, func->self, args, infix);
	}

	// Function defined in language
	else
	{
		l_vm_scope* scope = l_vm_scope_create(func->scope);
		scope->parent = func->scope;

		// Define variables
		if (func->argnames != NULL)
		{
			if (args->len != func->argnamec)
			{
				return l_vm_error_argc(vm, func->argnamec, args->len);
			}

			for (int i = 0; i < args->len; ++i)
			{
				l_vm_scope_define(scope, func->argnames[i], args->vars[i]);
			}
		}

		// Define $n variables
		for (int i = 0; i < args->len; ++i)
		{
			char name[16];
			name[0] = '$';
			snprintf(name + 1, 15, "%d", i + 1);
			l_vm_scope_define(scope, name, args->vars[i]);
		}

		// Define self
		if (func->self != NULL)
		{
			l_vm_scope_define(scope, "self", func->self);
		}

		// Define infix?
		{
			l_vm_var* isInfix = l_vm_var_create(vm, VAR_TYPE_BOOL);
			if (infix)
				isInfix->var.boolean = 1;
			else
				isInfix->var.boolean = 0;

			l_vm_scope_define(scope, "infix?", isInfix);
		}

		ret = l_vm_exec(vm, scope, func->expressions, func->expressionc);
	}

	if (fakeargs)
	{
		free(args);
	}

	return ret;
}

l_vm_var* l_vm_var_function_set_self(
		l_vm* vm,
		l_vm_var_function* func,
		l_vm_var* obj)
{
	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_FUNCTION);
	l_vm_var_function* varf = l_vm_var_function_create(func->scope);
	varf->fptr = func->fptr;
	varf->expressions = func->expressions;
	varf->expressionc = func->expressionc;
	varf->argnames = func->argnames;
	varf->argnamec = func->argnamec;
	varf->self = obj;
	var->var.function = varf;

	return var;
}

void l_vm_var_function_free(l_vm_var_function* func)
{
	free(func->argnames);
	free(func);
}
