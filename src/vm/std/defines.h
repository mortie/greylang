/*
 * This file contains macros to be used in functions
 * which contain the standard library
 */

#define EXPECTTYPE(vm, expected, var) \
	do { \
		if (var == NULL || var->type != expected) \
			return l_vm_error(vm, "Invalid types"); \
	} while(0)

#define EXPECTARGC(vm, expected, args) \
	do { \
		if (args->varc != expected) \
			return l_vm_error(vm, "Invalid number of arguments"); \
	} while(0)

#define SWAP(a, b) \
	do { \
		vm_var *tmp = b; \
		b = a; \
		a = tmp; \
	} while(0)
