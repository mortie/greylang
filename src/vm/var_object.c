#include "../vm.h"

#include <stdlib.h>

l_vm_var_object* l_vm_var_object_create()
{
	l_vm_var_object* obj = malloc(sizeof(l_vm_var_object));

	obj->allocd = 8;
	obj->names = malloc(sizeof(char*) * obj->allocd);
	obj->vars = malloc(sizeof(l_vm_var) * obj->allocd);
	obj->len = 0;

	return obj;
}

void l_vm_var_object_set(
		l_vm_var_object* obj,
		char* name,
		l_vm_var* var)
{
	for (int i = 0; i < obj->len; ++i)
	{
		if (strcmp(obj->names[i], name) == 0)
		{
			obj->vars[i] = var;
			return;
		}
	}

	obj->len += 1;
	if (obj->allocd < obj->len)
	{
		obj->allocd *= 2;
		obj->names = realloc(obj->names, sizeof(char*) * obj->allocd);
		obj->vars = realloc(obj->vars, sizeof(char*) * obj->allocd);
	}

	obj->vars[obj->len - 1] = var;

	int slen = strlen(name);
	char* n = malloc(slen + 1);
	memcpy(n, name, slen + 1);
	obj->names[obj->len - 1] = n;
}

l_vm_var* l_vm_var_object_lookup(l_vm_var_object* obj, char* name)
{
	for (int i = 0; i < obj->len; ++i)
	{
		if (strcmp(obj->names[i], name) == 0)
		{
			return obj->vars[i];
		}
	}

	return l_vm_var_create(VAR_TYPE_NONE);
}
