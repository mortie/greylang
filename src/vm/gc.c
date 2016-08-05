#include "../vm.h"

#include <stdlib.h>

static void markv(l_vm_var* var, int val)
{
	var->gc_marked = val;

	switch(var->type)
	{
	case VAR_TYPE_ARRAY:
		var->var.array->gc_marked = val;
		break;
	case VAR_TYPE_FUNCTION:
		var->var.function->gc_marked = val;
		break;
	case VAR_TYPE_ERROR:
		var->var.error->gc_marked = val;
		break;
	default:
		break;
	}
}

static void markm(l_vm_map* map)
{
	for (int i = 0; i < map->len; ++i)
	{
		markv(map->vars[i], 1);
	}

	if (map->internal != NULL)
		markm(map->internal);
}

static void mark(l_vm_scope* scope)
{
	if (!scope->immutable)
		markm(scope->map);

	for (int i = 0; i < scope->childc; ++i)
	{
		mark(scope->childs[i]);
	}
}

static void freev(l_vm_var* var)
{
	if (!var->map->gc_marked)
		l_vm_map_free(var->map);

	switch (var->type)
	{
	case VAR_TYPE_ARRAY:
		if (!var->var.array->gc_marked)
			l_vm_var_array_free(var->var.array);
		break;
	case VAR_TYPE_FUNCTION:
		if (!var->var.function->gc_marked)
			l_vm_var_function_free(var->var.function);
	case VAR_TYPE_STRING:
		// No check for gc_marked because immutable
		l_vm_var_string_free(var->var.string);
		break;
	case VAR_TYPE_ERROR:
		if (!var->var.error->gc_marked)
			l_vm_var_error_free(var->var.error);
		break;
	default:
		break;
	}

	free(var);
}

static void sweep(l_vm* vm)
{
	for (int i = 0; i < vm->varc; ++i)
	{
		l_vm_var* var = vm->vars[i];
		if (var == NULL || var->gc_marked)
			continue;

		freev(var);
	}
}

void l_vm_gc(l_vm* vm)
{
	mark(vm->global);
	sweep(vm);
}
