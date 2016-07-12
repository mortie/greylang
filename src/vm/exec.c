#include "../vm.h"

#include <stdlib.h>

static l_vm_var* exec(l_vm* vm, l_vm_scope* scope, l_p_expr* expr)
{
	vm->currLine = expr->line;

	switch (expr->type)
	{
	case EXPR_EMPTY:
	{
		return l_vm_var_create(vm, VAR_TYPE_NONE);
	}
	case EXPR_GROUP:
	{
		return exec(vm, scope, expr->expression.expr_group->expr);
	}
	case EXPR_FUNC_CALL:
	{
		l_vm_var* func = exec(vm, scope, expr->expression.func_call->func);

		if (func->type != VAR_TYPE_FUNCTION)
		{
			return l_vm_error_type(vm, VAR_TYPE_FUNCTION, func->type);
		}

		l_p_comma_expr_list* exprs = expr->expression.func_call->arg_list;

		l_vm_var** argvars = malloc(sizeof(l_vm_var*) * exprs->expressionc);
		for (int i = 0; i < exprs->expressionc; ++i)
		{
			argvars[i] = exec(vm, scope, exprs->expressions[i]);
		}

		l_vm_var_array* args = malloc(sizeof(l_vm_var_array));
		args->vars = argvars;
		args->len = exprs->expressionc;
		args->allocd = exprs->expressionc;

		l_vm_var* res = l_vm_var_function_exec(vm, func->var.function, args);

		free(args);
		return res;
	}
	case EXPR_OBJECT_LOOKUP:
	{
		l_vm_var* obj = exec(vm, scope, expr->expression.object_lookup->obj);
		char* key = expr->expression.object_lookup->key;

		l_vm_var* v = l_vm_map_lookup(obj->map, key);

		if (v == NULL)
			return l_vm_var_create(vm, VAR_TYPE_NONE);

		if (v->type == VAR_TYPE_FUNCTION)
		{
			l_vm_var_function* func = l_vm_var_function_create(v->var.function->scope);
			func->fptr = v->var.function->fptr;
			func->expressions = v->var.function->expressions;
			func->expressionc = v->var.function->expressionc;
			func->argnames = v->var.function->argnames;
			func->argnamec = v->var.function->argnamec;
			func->self = obj;
			v->var.function = func;
		}

		return v;
	}
	case EXPR_ARRAY_LOOKUP:
	{
		l_vm_var* arr = exec(vm, scope, expr->expression.array_lookup->arr);
		l_vm_var* key = exec(vm, scope, expr->expression.array_lookup->key);

		if (arr->type != VAR_TYPE_ARRAY)
		{
			return l_vm_error_type(vm, VAR_TYPE_ARRAY, arr->type);
		}
		if (key->type != VAR_TYPE_NUMBER)
		{
			return l_vm_error_type(vm, VAR_TYPE_NUMBER, key->type);
		}

		l_vm_var_array* a = arr->var.array;
		int k = (int)key->var.number;

		if (k >= a->len)
			return l_vm_var_create(vm, VAR_TYPE_NONE);

		return a->vars[k];
	}
	case EXPR_ASSIGNMENT:
	{
		l_vm_var* var = exec(vm, scope, expr->expression.assignment->val);

		l_p_expr* key = expr->expression.assignment->key;
		switch (key->type)
		{
		case EXPR_OBJECT_LOOKUP:
		{
			l_vm_var* obj = exec(vm, scope, key->expression.object_lookup->obj);
			char* okey = key->expression.object_lookup->key;
			l_vm_map_set(obj->map, okey, var);

			break;
		}
		case EXPR_ARRAY_LOOKUP:
		{
			l_vm_var* arr = exec(vm, scope, key->expression.array_lookup->arr);
			l_vm_var* akey = exec(vm, scope, key->expression.array_lookup->key);

			if (arr->type != VAR_TYPE_ARRAY)
			{
				return l_vm_error_type(vm, VAR_TYPE_ARRAY, arr->type);
			}
			if (akey->type != VAR_TYPE_NUMBER)
			{
				return l_vm_error_type(vm, VAR_TYPE_NUMBER, akey->type);
			}

			l_vm_var_array* a = arr->var.array;
			int k = (int)akey->var.number;
			l_vm_var_array_resize(vm, a, k + 1);

			a->vars[k] = var;

			break;
		}
		case EXPR_VARIABLE:
		{
			l_vm_scope_set(scope, key->expression.variable->name, var);

			break;
		}
		default:
			return l_vm_error(vm, "Invalid assignment");
		}

		return var;
	}
	case EXPR_FUNCTION:
	{
		l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_FUNCTION);
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
	case EXPR_OBJECT_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_OBJECT);

		for (int i = 0; i < expr->expression.object_literal->exprc; ++i)
		{
			char* name = expr->expression.object_literal->names[i];
			l_vm_var* val = exec(vm, scope,
				expr->expression.object_literal->exprs[i]);

			l_vm_map_set(var->map, name, val);
		}

		return var;
	}
	case EXPR_ARRAY_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_ARRAY);

		l_p_comma_expr_list* exprs = expr->expression.array_literal->expr_list;

		l_vm_var_array* arr = malloc(sizeof(l_vm_var_array));
		arr->vars = malloc(sizeof(l_vm_var*) * exprs->expressionc);
		arr->len = exprs->expressionc;
		arr->allocd = exprs->expressionc;
		for (int i = 0; i < exprs->expressionc; ++i)
		{
			arr->vars[i] = exec(vm, scope, exprs->expressions[i]);
		}

		var->var.array = arr;

		return var;
	}
	case EXPR_STRING_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
		l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

		str->chars = expr->expression.string_literal->string;
		str->len = strlen(str->chars);
		var->var.string = str;

		return var;
	}
	case EXPR_NUM_LITERAL:
	{
		l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_NUMBER);
		var->var.number = expr->expression.num_literal->number;
		return var;
	}
	case EXPR_VARIABLE:
	{
		l_vm_var* var = l_vm_scope_lookup(
			scope, expr->expression.variable->name);
		if (var == NULL)
			return l_vm_var_create(vm, VAR_TYPE_NONE);
		else
			return var;
	}
	default:
		return l_vm_var_create(vm, VAR_TYPE_NONE);
	}
}

l_vm_var* l_vm_exec(l_vm* vm, l_vm_scope* scope, l_p_expr** expressions, int expressionc)
{
	for (int i = 0; i < expressionc; ++i)
	{
		if (i == expressionc - 1)
		{
			return exec(vm, scope, expressions[i]);
		}
		else
		{
			l_vm_var* var = exec(vm, scope, expressions[i]);
			if (var->type == VAR_TYPE_ERROR)
				return var;
		}
	}

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_run(l_vm* vm, l_p_expr_list* list)
{
	l_vm_var* res = l_vm_exec(
		vm,
		vm->global,
		list->expressions,
		list->expressionc);

	if (res->type == VAR_TYPE_ERROR)
		puts(l_vm_var_tostring(res));

	return res;
}
