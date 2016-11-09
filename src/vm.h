#ifndef VM_H
#define VM_H

#include "parser.h"

typedef struct l_vm l_vm;
typedef struct vm_var vm_var;
typedef struct vm_map vm_map;

#include "vm/var.h"
#include "vm/map.h"

typedef struct l_vm
{
	vm_map *root;
} l_vm;

l_vm *l_vm_create();
vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs);

#endif
