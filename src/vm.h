#ifndef L_VM_H
#define VM_H

typedef struct l_vm_map
{
	int varc;
	int vars;
	int names;

	struct l_vm_map *parent;
} l_vm_map;

#endif
