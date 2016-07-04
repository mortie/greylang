#include "../vm.h"

#include <stdlib.h>

static l_vm_var* exec(l_vm_scope* scope, l_p_expr* expr)
{
	switch (expr->type)
	{
	case EXPR_EMPTY:
	{
		return l_vm_var_create(VAR_TYPE_NONE);
	}
	case EXPR_ASSIGNMENT:
	{
		l_vm_var* var = exec(scope, expr->expression.assignment->expression);
		l_vm_scope_set(scope, expr->expression.assignment->name, var);
		return var;
	}
	case EXPR_FUNC_CALL:
	{
		l_vm_var* func = exec(scope, expr->expression.func_call->func);

		if (func->type != VAR_TYPE_FUNCTION)
		{
			l_vm_error_type(VAR_TYPE_FUNCTION, func->type);
		}

		l_p_comma_expr_list* exprs = expr->expression.func_call->arg_list;

		l_vm_var** argvars = malloc(sizeof(l_vm_var*) * exprs->expressionc);
		for (int i = 0; i < exprs->expressionc; ++i)
		{
			argvars[i] = exec(scope, exprs->expressions[i]);
		}

		l_vm_var_array* args = malloc(sizeof(l_vm_var_array));
		args->vars = argvars;
		args->len = exprs->expressionc;
		args->allocd = exprs->expressionc;

		l_vm_var* res = l_vm_var_function_exec(func->var.function, args);

		free(args);
		return res;
	}
	case EXPR_ARRAY_LOOKUP:
	{
		l_vm_var* arr = exec(scope, expr->expression.array_lookup->arr);
		l_vm_var* key = exec(scope, expr->expression.array_lookup->key);

		if (arr->type != VAR_TYPE_ARRAY)
		{
			l_vm_error_type(VAR_TYPE_ARRAY, arr->type);
		}
		if (key->type != VAR_TYPE_NUMBER)
		{
			l_vm_error_type(VAR_TYPE_NUMBER, key->type);
		}

		l_vm_var_array* a = arr->var.array;
		int k = (int)key->var.number;

		if (k >= a->len)
			return l_vm_var_create(VAR_TYPE_NONE);

		return a->vars[k];
	}
	case EXPR_FUNCTION:
	{
		l_vm_var* var = l_vm_var_create(VAR_TYPE_FUNCTION);
		l_vm_var_function* func = l_vm_var_function_create(scope);

		func->expressions = expr->expression.function->expr_list->expressions;
		func->expressionc = expr->expression.function->expr_list->expressionc;

		l_p_arg_definition* argdef = expr->expression.function->arg_definition;
		if (argdef == NULL)
		{
			func->argnames = NULL;
			func->argnamec = 0;
		}
		else
		{
			func->argnames = argdef->names;
			func->argnamec = argdef->namec;
		}

		var->var.function = func;

		return var;
	}
	case EXPR_ARRAY_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(VAR_TYPE_ARRAY);

		l_p_comma_expr_list* exprs = expr->expression.array_literal->expr_list;

		l_vm_var_array* arr = malloc(sizeof(l_vm_var_array));
		arr->vars = malloc(sizeof(l_vm_var*) * exprs->expressionc);
		arr->len = exprs->expressionc;
		arr->allocd = exprs->expressionc;
		for (int i = 0; i < exprs->expressionc; ++i)
		{
			arr->vars[i] = exec(scope, exprs->expressions[i]);
		}

		var->var.array = arr;

		return var;
	}
	case EXPR_STRING_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(VAR_TYPE_STRING);
		l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

		str->chars = expr->expression.string_literal->string;
		str->len = strlen(str->chars);
		var->var.string = str;

		return var;
	}
	case EXPR_CHAR_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(VAR_TYPE_CHAR);
		var->var.character = expr->expression.char_literal->character;
		return var;
	}
	case EXPR_NUM_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(VAR_TYPE_NUMBER);
		var->var.number = expr->expression.num_literal->number;
		return var;
	}
	case EXPR_VARIABLE:
	{
		l_vm_var* var = l_vm_scope_lookup(
			scope, expr->expression.variable->name);
		if (var == NULL)
			return l_vm_var_create(VAR_TYPE_NONE);
		else
			return var;
	}
	default:
		return l_vm_var_create(VAR_TYPE_NONE);
	}
}

l_vm_var* l_vm_exec(l_vm_scope* scope, l_p_expr** expressions, int expressionc)
{
	for (int i = 0; i < expressionc; ++i)
	{
		if (i == expressionc - 1)
			return exec(scope, expressions[i]);
		else
			exec(scope, expressions[i]);
	}

	return l_vm_var_create(VAR_TYPE_NONE);
}

void l_vm_run(l_vm* vm, l_p_expr_list* list)
{
	l_vm_exec(vm->global, list->expressions, list->expressionc);
}