/*
 * Included from vm.h
 */

typedef struct vm_map
{
	int varc;
	vm_var *vars;
	char **keys;
	int allocd;

	struct vm_map *parent;
} vm_map;

/*
 * Create a new vm_map, allocated on the heap with malloc.
 */
vm_map *vm_map_create(vm_map *parent);

/*
 * Define a new key with a value.
 *
 * Returns:
 *     0 on success, -1 if the key already exists
 */
int vm_map_define(vm_map *map, char *key, vm_var *var);

/*
 * Set an existing key to a new value.
 *
 * Returns:
 *     0 on success, -1 if the key doesn't exist
 */
int vm_map_replace(vm_map *map, char *key, vm_var *var);

/*
 * Set a key to a value. Replaces the value with the same key if it exist,
 * defines a new key if it doesn't.
 */
int vm_map_set(vm_map *map, char *key, vm_var *var);

/*
 * Lookup the value assigned to a key.
 */
vm_var *vm_map_lookup(vm_map *map, char *key);

/*
 * Recursively set an existing key to a new value.
 * 
 * Returns:
 *     0 once it found a key to replace, -1 if no keys could be replaced.
 */
int vm_map_replace_r(vm_map *map, char *key, vm_var *var);

/*
 * Like vm_map_replace_r, but defines a new key on the map
 * if no key could be replaced.
 */
void vm_map_set_r(vm_map *map, char *key, vm_var *var);

/*
 * Lookup recursively.
 */
int vm_map_lookup_r(vm_map *map, char *key);
