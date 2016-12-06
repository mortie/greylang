/*
 * Included from vm.h
 */

#define STD(name) \
	vm_var *vm_std_(name)(l_vm *vm, vm_var *var, int infix)

STD(tostring);
