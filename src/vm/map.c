#include "../vm.h"

#include <stdlib.h>

vm_map *vm_map_create(vm_map *parent)
{
	vm_map *map = malloc(sizeof(*map));
	map->varc = 0;
	map->vars = NULL;
	map->keys = NULL;
	map->allocd = 0;
	map->parent = parent;
	return map;
}
