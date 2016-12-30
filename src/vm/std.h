/*
 * Included from vm.h
 */

#define STD(name) \
	vm_var *name(l_vm *vm, vm_var *self, vm_var_array *args, int infix)

// Flow Control
STD(vm_std_if);
STD(vm_std_repeat);
STD(vm_std_while);
STD(vm_std_for);

// Special
STD(vm_std_new);

// Math
STD(vm_std_add);
STD(vm_std_sub);
STD(vm_std_mul);
STD(vm_std_div);
STD(vm_std_pow);

// Logic
STD(vm_std_eq);
STD(vm_std_neq);
STD(vm_std_gt);
STD(vm_std_lt);
STD(vm_std_gteq);
STD(vm_std_lteq);
STD(vm_std_and);
STD(vm_std_or);

// Util
STD(vm_std_tostr);
STD(vm_std_concat);

// I/O
STD(vm_std_print);
STD(vm_std_read);

#undef STD
