#include "../vm.h"

#include <stdlib.h>

l_vm_map* l_vm_map_create(l_vm_map* proto)
{
	l_vm_map* map = malloc(sizeof(l_vm_map));

	map->allocd = 8;
	map->names = malloc(sizeof(char*) * map->allocd);
	map->vars = malloc(sizeof(l_vm_var) * map->allocd);
	map->len = 0;
	map->proto = proto;

	return map;
}

void l_vm_map_set(
		l_vm_map* map,
		char* name,
		l_vm_var* var)
{
	for (int i = 0; i < map->len; ++i)
	{
		if (strcmp(map->names[i], name) == 0)
		{
			map->vars[i] = var;
			return;
		}
	}

	map->len += 1;
	if (map->allocd < map->len)
	{
		map->allocd *= 2;
		map->names = realloc(map->names, sizeof(char*) * map->allocd);
		map->vars = realloc(map->vars, sizeof(char*) * map->allocd);
	}

	map->vars[map->len - 1] = var;

	int slen = strlen(name);
	char* n = malloc(slen + 1);
	memcpy(n, name, slen + 1);
	map->names[map->len - 1] = n;
}

l_vm_var* l_vm_map_shallow_lookup(l_vm_map* map, char* name)
{
	for (int i = 0; i < map->len; ++i)
	{
		if (strcmp(map->names[i], name) == 0)
		{
			return map->vars[i];
		}
	}

	return NULL;
}

l_vm_var* l_vm_map_lookup(l_vm_map* map, char* name)
{
	while (map != NULL)
	{
		l_vm_var* var = l_vm_map_shallow_lookup(map, name);
		if (var != NULL)
			return var;

		map = map->proto;
	}

	return NULL;
}