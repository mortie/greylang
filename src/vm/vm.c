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
	STD("$", STD_FUNC_CALLFUNC);
	STD("$$", STD_FUNC_CALLSETTER);
	STD("if", STD_FUNC_IF);
	STD("repeat", STD_FUNC_REPEAT);
	STD("map", STD_FUNC_MAP);
	STD("tostring", STD_FUNC_TOSTRING);
	STD("tonumber", STD_FUNC_TONUMBER);
	STD("concat", STD_FUNC_CONCAT);
	STD("print!", STD_FUNC_PRINT);
	STD("read!", STD_FUNC_READ);

#undef STD

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
		l_vm_var* func = l_vm_scope_lookup(
			scope, expr->expression.func_call->name);

		if (func == NULL)
		{
			l_vm_error_undefined(expr->expression.func_call->name);
		}

		if (func->type != VAR_TYPE_FUNCTION)
		{
			l_vm_error_type(VAR_TYPE_FUNCTION, func->type);
		}

		l_p_arg_expr_list* exprs = expr->expression.func_call->arg_list;

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
