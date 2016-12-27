#include "../vm.h"

#include <stdlib.h>

vm_var *vm_exec(l_vm *vm, vm_map *scope, l_p_expr *expr)
{
	switch (expr->type)
	{
	case EXPR_EMPTY:
		return vm->var_none;

	case EXPR_GROUP:
		return vm_exec(vm, scope, expr->expression.expr_group->expr);

	case EXPR_FUNC_CALL:
	{
		l_p_expr_func_call *e = expr->expression.func_call;
		vm_var *func = vm_exec(vm, scope, e->func);
		l_p_comma_expr_list *exprs = e->arg_list;

		if (func->type != VAR_TYPE_FUNCTION)
			return l_vm_error(vm, "Expected function");

		vm_var_array *args = malloc(sizeof(*args));
		vm_var_array_init(args, VAR_TYPE_NONE);

		for (int i = 0; i < exprs->expressionc; ++i)
		{
			vm_var *val = vm_exec(vm, scope, exprs->expressions[i]);
			vm_var_array_set(args, i, val);
		}

		vm_var *ret = vm_var_function_exec(
			vm, func->var.function,
			args, e->infix);

		vm_var_array_free(args);
		free(args);

		return ret;
	}

	case EXPR_OBJECT_LOOKUP:
	{
		l_p_expr_object_lookup *e = expr->expression.object_lookup;
		vm_var *obj = vm_exec(vm, scope, e->obj);
		vm_var *var = vm_map_lookup_r(obj->map, e->key);
		if (var == NULL)
			return vm->var_none;
		else
			return var;
	}

	case EXPR_ARRAY_LOOKUP:
	{
		l_p_expr_array_lookup *e = expr->expression.array_lookup;
		vm_var *obj = vm_exec(vm, scope, e->arr);
		vm_var *key = vm_exec(vm, scope, e->key);

		if (obj->type == VAR_TYPE_ARRAY)
		{
			vm_var_array *arr = obj->var.array;
			if (key->type != VAR_TYPE_NUMBER)
				return l_vm_error(vm, "Expected number in array lookup");

			int num = (int)key->var.number;
			if (num >= arr->varc)
				return l_vm_error(vm, "Array index out of bounds");

			vm_var *var = arr->vars[num];
			if (var == NULL)
				return vm->var_none;
			else
				return var;
		}

		// TODO: object lookup
		else if (
				key->type == VAR_TYPE_ARRAY &&
				key->var.array->type == VAR_TYPE_CHAR)
		{
			return l_vm_error(vm, "Object lookup with [] is not implemented");
		}
		else
		{
			return l_vm_error(vm, "Confusing array/object lookup");
		}
	}

	case EXPR_ASSIGNMENT:
	{
		l_p_expr_assignment *e = expr->expression.assignment;

		vm_var *val = vm_exec(vm, scope, e->val);
		l_p_expr *ekey = e->key;

		if (ekey->type == EXPR_OBJECT_LOOKUP)
		{
			l_p_expr_object_lookup *ol = ekey->expression.object_lookup;
			vm_var *obj = vm_exec(vm, scope, ol->obj);
			if (vm_map_set(obj->map, ol->key, val) == -1)
				return l_vm_error(vm, "Scope is immutable");
		}
		else if (ekey->type == EXPR_ARRAY_LOOKUP)
		{
			l_p_expr_array_lookup *al = ekey->expression.array_lookup;
			vm_var *arr = vm_exec(vm, scope, al->arr);
			vm_var *key = vm_exec(vm, scope, al->key);

			if (arr->type == VAR_TYPE_ARRAY && key->type == VAR_TYPE_NUMBER)
			{
				int ret = vm_var_array_set(
					arr->var.array, (int)(key->var.number), val);
				if (ret == -1)
					return l_vm_error(vm, "Can't insert value into array");
			}
			else if (
					key->type == VAR_TYPE_ARRAY &&
					key->var.array->type == VAR_TYPE_CHAR)
			{
				char *k = ""; // TODO: char array to string conversion
				if (vm_map_set(arr->map, k, val))
					return l_vm_error(vm, "Scope is immutable");
			}
			else
			{
				return l_vm_error(vm, "Confusing array/object assignment");
			}
		}
		else if (ekey->type == EXPR_VARIABLE)
		{
			if (vm_map_replace(scope, ekey->expression.variable->name, val) == -1)
				return l_vm_error(vm, "Variable isn't defined");
		}
		else
		{
			return l_vm_error(vm, "Invalid assignment");
		}

		return val;
	}

	case EXPR_DECLARATION:
	{
		l_p_expr_assignment *e = expr->expression.assignment;

		vm_var *val = vm_exec(vm, scope, e->val);
		l_p_expr *ekey = e->key;

		if (ekey->type != EXPR_VARIABLE)
			return l_vm_error(vm, "Invalid declaration");

		char *key = ekey->expression.variable->name;
		if (vm_map_define(scope, key, val) == -1)
			return l_vm_error(vm, "Variable already exists");

		return vm->var_none;
	}

	case EXPR_FUNCTION:
	{
		l_p_expr_function *e = expr->expression.function;

		vm_var_function *func = malloc(sizeof(*func));
		vm_var_function_init(func, scope);

		if (e->arg_definition != NULL)
		{
			func->argnames = e->arg_definition->names;
			func->argnamec = e->arg_definition->namec;
		}
		func->exprs = e->expr_list->expressions;
		func->exprc = e->expr_list->expressionc;

		vm_var *var = vm_var_create(VAR_TYPE_FUNCTION);
		var->var.function = func;
		return var;
	}

	case EXPR_OBJECT_LITERAL:
	{
		l_p_expr_object_literal *e = expr->expression.object_literal;

		vm_var *var = vm_var_create(VAR_TYPE_OBJECT);

		for (int i = 0; i < e->exprc; ++i)
		{
			vm_var *val = vm_exec(vm, scope, e->exprs[i]);
			if (vm_map_set(var->map, e->names[i], val) == -1)
				return l_vm_error(vm, "Scope is immutable");
		}

		return var;
	}

	case EXPR_ARRAY_LITERAL:
	{
		l_p_expr_array_literal *al = expr->expression.array_literal;
		l_p_comma_expr_list *e = al->expr_list;

		vm_var_array *arr = malloc(sizeof(*arr));
		vm_var_array_init(arr, VAR_TYPE_NONE);

		for (int i = 0; i < e->expressionc; ++i)
		{
			vm_var *val = vm_exec(vm, scope, e->expressions[i]);
			vm_var_array_set(arr, i, val);
		}

		vm_var *var = vm_var_create(VAR_TYPE_ARRAY);
		var->var.array = arr;
		return var;
	}

	case EXPR_STRING_LITERAL:
	{
		vm_var_array *arr = malloc(sizeof(*arr));
		vm_var_array_init(arr, VAR_TYPE_CHAR);

		// TODO: make strings

		vm_var *var = vm_var_create(VAR_TYPE_ARRAY);
		var->var.array = arr;
		return var;
	}

	case EXPR_NUM_LITERAL:
	{
		vm_var *var = vm_var_create(VAR_TYPE_NUMBER);
		var->var.number = expr->expression.num_literal->number;
		return var;
	}

	case EXPR_VARIABLE:
	{
		char *key = expr->expression.variable->name;
		vm_var *var = vm_map_lookup_r(scope, key);
		if (var == NULL)
			return vm->var_none;
		else
			return var;
	}
	}

	return vm->var_none;
}
