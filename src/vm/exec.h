/*
 * Include from vm.h
 */

vm_var *vm_exec(l_vm *vm, vm_map *scope, l_p_expr *expr);
vm_var *vm_exec_exprs(l_vm *vm, vm_map *scope, l_p_expr **exprs, int exprc);
