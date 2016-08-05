#include "../vm.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void add_child(l_vm_scope* scope, l_vm_scope* child)
{
	scope->childc += 1;
	if (scope->childa < scope->childc)
	{
		if (scope->childa < 4)
			scope->childa = 4;

		scope->childa *= 2;

		scope->childs = realloc(
			scope->childs, sizeof(l_vm_scope*) * scope->childa);
	}

	scope->childs[scope->childc - 1] = child;
}

l_vm_scope* l_vm_scope_create(l_vm_scope* parent)
{
	l_vm_scope* scope = malloc(sizeof(l_vm_scope));

	scope->map = l_vm_map_create(NULL);
	scope->parent = parent;
	scope->immutable = 0;

	scope->childs = NULL;
	scope->childc = 0;
	scope->childa = 0;

	if (parent != NULL)
		add_child(parent, scope);

	return scope;
}

l_vm_var* l_vm_scope_shallow_lookup(l_vm_scope* scope, char* name)
{
	return l_vm_map_shallow_lookup(scope->map, name);
}

l_vm_var* l_vm_scope_lookup(l_vm_scope* scope, char* name)
{
	while (scope != NULL)
	{
		l_vm_var* var = l_vm_scope_shallow_lookup(scope, name);
		if (var != NULL)
			return var;

		scope = scope->parent;
	}

	return NULL;
}

void l_vm_scope_define(l_vm_scope* scope, char* name, l_vm_var* var)
{
	int exists = 0;
	int i;
	for (i = 0; i < scope->map->len; ++i)
	{
		if (strcmp(scope->map->names[i], name) == 0)
		{
			exists = 1;
			break;
		}
	}

	if (exists)
	{
		scope->map->vars[i] = var;
	}
	else
	{
		l_vm_map_set(scope->map, name, var);
	}
}

static int scope_set(l_vm_scope* scope, char* name, l_vm_var* var)
{
	if (scope == NULL)
		return 0;
	if (scope->immutable)
		return 0;

	int success = scope_set(scope->parent, name, var);
	if (success)
	{
		return 1;
	}
	else
	{
		l_vm_var* lvar = l_vm_scope_shallow_lookup(scope, name);
		if (lvar == NULL)
			return 0;

		(&lvar)[0] = var;

		return 1;
	}
}

void l_vm_scope_set(l_vm_scope* scope, char* name, l_vm_var* var)
{
	int success = scope_set(scope->parent, name, var);

	if (!success)
	{
		l_vm_scope_define(scope, name, var);
	}
}

void l_vm_scope_free(l_vm_scope* scope)
{
	l_vm_map_free(scope->map);
	free(scope);
}
