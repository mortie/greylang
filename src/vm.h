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
#include "vm/std.h"

typedef struct l_vm
{
	vm_map *base;
	vm_map *root;

	vm_var **cleanups;
	int cleanupc;
	int cleanupa;
	int cleanup_offset;

	vm_var *var_none;
	vm_var *var_true;
	vm_var *var_false;

	vm_var *proto_object;
	vm_var *proto_function;
	vm_var *proto_array;
	vm_var *proto_string;
	vm_var *proto_number;
	vm_var *proto_bool;
	vm_var *proto_char;
	vm_var *proto_error;
	vm_var *proto_none;

	char *currfile;
	int currline;
} l_vm;

l_vm *l_vm_create(char *file);
vm_var *l_vm_run(l_vm *vm, l_p_expr_list *exprs);
vm_var *l_vm_error(l_vm *vm, char *msg);
vm_var *l_vm_error_type(l_vm *vm, vm_var_type expected, vm_var *var);

void l_vm_cleanup_add(l_vm *vm, vm_var *var);
void l_vm_cleanup(l_vm *vm);

#endif
