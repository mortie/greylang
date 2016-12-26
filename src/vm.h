#ifndef VM_H
#define VM_H

#include <stdint.h>

#include "parser.h"

typedef struct l_vm l_vm;
typedef struct vm_var vm_var;
typedef struct vm_map vm_map;

#include "vm/var.h"
#include "vm/map.h"
#include "vm/exec.h"

typedef struct l_vm
{
	vm_map *base;
	vm_map *root;
	vm_var *var_none;
	vm_var *var_true;
	vm_var *var_false;
} l_vm;

l_vm *l_vm_create();
vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs);
vm_var *l_vm_error(l_vm *vm, char *msg);

#endif
