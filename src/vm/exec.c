#include "../vm.h"

vm_var *vm_exec(l_p_expr *expr)
{
	switch (expr->type)
	{
	case EXPR_EMPTY:
		return NULL;

	case EXPR_GROUP:
		return vm_exec(expr->expression.expr_group->expr);

	case EXPR_FUNC_CALL:
		// TODO: fix
		break;

	case EXPR_OBJECT_LOOKUP:
	{
		l_p_expr_object_lookup *e = expr->expression.object_lookup;
		vm_var *obj = vm_exec(e->obj);
		return vm_map_lookup_r(obj->map, e->key);
	}

	case EXPR_ARRAY_LOOKUP:
	{
		l_p_expr_array_lookup *e = expr->expression.array_lookup;
		vm_var *obj = vm_exec(e->arr);
		vm_var *key = vm_exec(e->key);

		if (obj->type == VAR_TYPE_ARRAY)
		{
			vm_var_array *arr = obj->var.array;
			if (key->type != VAR_TYPE_NUMBER)
				return NULL; // TODO: errors

			int num = (int)key->var.number;
			if (num >= arr->varc)
				return NULL; // TODO: errors

			return arr->vars[num];
		}
		else
		{
			if (
					key->type != VAR_TYPE_ARRAY ||
					key->var.array->type != VAR_TYPE_CHAR)
				return NULL; // TODO: errors
		}
	}
	}
}
