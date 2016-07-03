#include "../vm.h"

#include <stdlib.h>
#include <string.h>

l_vm_scope* l_vm_scope_create(l_vm_scope* parent)
{
	l_vm_scope* scope = malloc(sizeof(l_vm_scope));

	scope->vara = 8;
	scope->varc = 0;
	scope->vars = malloc(sizeof(l_vm_var) * scope->vara);
	scope->names = malloc(sizeof(char*) * scope->vara);
	scope->parent = parent;

	return scope;
}

void l_vm_scope_set(l_vm_scope* scope, char* name, l_vm_var* var)
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

void l_vm_scope_free(l_vm_scope* scope)
{
	for (int i = 0; i < scope->varc; ++i)
	{
		l_vm_var_free(scope->vars[i]);
		free(scope->names[i]);
	}

	free(scope->vars);
	free(scope->names);
	free(scope);
}
