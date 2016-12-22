#include "../vm.h"

#include <stdlib.h>
#include <string.h>

vm_map *vm_map_create(vm_map *parent)
{
	vm_map *map = malloc(sizeof(*map));

	map->varc = 0;
	map->vars = NULL;
	map->keys = NULL;
	map->allocd = 0;
	map->parent = parent;
	map->immutable = 0;
	map->refs = 0;

	if (parent != NULL)
		vm_map_increfs(parent);

	return map;
}

int vm_map_define(vm_map *map, char *key, vm_var *var)
{
	if (map->immutable)
		return -1;

	// Return -1 if key exists
	for (int i = 0; i < map->varc; ++i)
	{
		if (map->keys[i] != NULL && strcmp(map->keys[i], key) == 0)
			return -1;
	}

	map->varc += 1;
	if (map->varc > map->allocd)
	{
		map->allocd = (map->allocd ? map->allocd * 2 : 8);
		map->vars = realloc(map->vars, map->allocd * sizeof(*(map->vars)));
		map->keys = realloc(map->keys, map->allocd * sizeof(*(map->vars)));
	}

	vm_var_increfs(var);
	map->vars[map->varc - 1] = var;
	map->keys[map->varc - 1] = malloc(strlen(key) + 1);
	strcpy(map->keys[map->varc - 1], key);

	return 0;
}

int vm_map_replace(vm_map *map, char *key, vm_var *var)
{
	if (map->immutable)
		return -1;

	for (int i = 0; i < map->varc; ++i)
	{
		if (map->keys[i] != NULL && strcmp(map->keys[i], key) == 0)
		{
			vm_var_increfs(var);
			vm_var_decrefs(map->vars[i]);
			map->vars[i] = var;
			return 0;
		}
	}

	return -1;
}

int vm_map_set(vm_map *map, char *key, vm_var *var)
{
	if (vm_map_replace(map, key, var) < 0)
		return vm_map_define(map, key, var);
	else
		return 0;
}

vm_var *vm_map_lookup(vm_map *map, char *key)
{
	for (int i = 0; i < map->varc; ++i)
	{
		if (map->keys[i] != NULL && strcmp(map->keys[i], key) == 0)
			return map->vars[i];
	}

	return NULL;
}

int vm_map_replace_r(vm_map *map, char *key, vm_var *var)
{
	if (vm_map_replace(map, key, var) == 0)
		return 0;

	if (map->parent == NULL)
		return -1;
	else
		return vm_map_replace_r(map->parent, key, var);
}

int vm_map_set_r(vm_map *map, char *key, vm_var *var)
{
	if (vm_map_set(map, key, var) == 0)
		return 0;

	if (map->parent == NULL)
		return -1;
	else
		return vm_map_set_r(map->parent, key, var);
}

vm_var *vm_map_lookup_r(vm_map *map, char *key)
{
	vm_var *var;
	var = vm_map_lookup(map, key);
	if (var != NULL)
		return var;

	if (map->parent == NULL)
		return NULL;
	else
		return vm_map_lookup_r(map->parent, key);
}

void vm_map_free(vm_map *map)
{
	for (int i = 0; i < map->varc; ++i)
	{
		if (map->vars[i] != NULL)
			vm_var_decrefs(map->vars[i]);
		free(map->keys[i]);

		map->vars[i] = NULL;
		map->keys[i] = NULL;
	}

	if (map->parent != NULL)
		vm_map_decrefs(map->parent);

	free(map->vars);
	free(map->keys);
	free(map);
}

void vm_map_increfs(vm_map *map)
{
	map->refs += 1;
}

void vm_map_decrefs(vm_map *map)
{
	map->refs -= 1;
	if (map->refs <= 0)
		vm_map_free(map);
}
