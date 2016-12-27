/*
 * Included from vm.h
 */

#define STD(name) \
	vm_var *name(l_vm *vm, vm_var *self, vm_var_array *args, int infix)

STD(vm_std_test);

#undef STD
