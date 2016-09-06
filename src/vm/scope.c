#include "../vm.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

l_vm_scope* l_vm_scope_create(l_vm_scope* parent)
{
	l_vm_scope* scope = malloc(sizeof(l_vm_scope));

	scope->vara = 8;
	scope->varc = 0;
	scope->vars = malloc(sizeof(l_vm_var) * scope->vara);
	scope->names = malloc(sizeof(char*) * scope->vara);
	scope->parent = parent;
	scope->immutable = 0;

	return scope;
}

l_vm_var* l_vm_scope_shallow_lookup(l_vm_scope* scope, char* name)
{
	for (int i = 0; i < scope->varc; ++i)
	{
		if (strcmp(scope->names[i], name) == 0)
		{
			return scope->vars[i];
		}
	}

	return NULL;
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
	for (i = 0; i < scope->varc; ++i)
	{
		if (strcmp(scope->names[i], name) == 0)
		{
			exists = 1;
			break;
		}
	}

	if (exists)
	{
		scope->vars[i] = var;
	}
	else
	{
		scope->varc += 1;
		if (scope->vara < scope->varc)
		{
			scope->vara *= 2;
			scope->names = realloc(scope->names, sizeof(char*) * scope->vara);
			scope->vars = realloc(scope->vars, sizeof(l_vm_var) * scope->vara);
		}
		scope->vars[scope->varc - 1] = var;

		int slen = strlen(name);
		char* n = malloc(slen + 1);
		memcpy(n, name, slen + 1);
		scope->names[scope->varc - 1] = n;
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

		l_vm_var_clean(lvar);
		lvar->var = var->var;
		lvar->type = var->type;

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
	free(scope->vars);
	free(scope->names);
	free(scope);
}
