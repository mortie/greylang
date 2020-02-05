/*
 * This file contains macros to be used in functions
 * which contain the standard library
 */

#define RETIFERR(var) \
	do { \
		if ((var) != NULL && (var)->type == VAR_TYPE_ERROR) \
			return var; \
	} while(0)

#define EXPECTTYPE(vm, expected, var) \
	do { \
		RETIFERR(var); \
		if (var == NULL || (var)->type != expected) \
			return l_vm_error_type(vm, expected, var); \
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

#define OVERLOAD(vm, name, a, b) \
	do { \
		if (a == NULL) \
			a = vm->var_none; \
		if (b == NULL) \
			b = vm->var_none; \
		vm_var *fn = vm_map_lookup_r(a->map, name); \
		EXPECTTYPE(vm, VAR_TYPE_FUNCTION,  fn); \
		vm_var_array args; \
		vm_var_array_init(&args, VAR_TYPE_NONE); \
		vm_var_array_set(&args, 0, b); \
		vm_var *ret = vm_var_function_exec( \
			vm, fn->var.function, &args, a, 0); \
		vm_var_array_free(&args); \
		return ret; \
	} while(0)

#define OVERLOAD_OR_TYPECHECK(vm, name, a, atype, b, btype) \
	do { \
		if (a == NULL || b == NULL) \
			return l_vm_error(vm, "Invalid types"); \
		if (a->type == atype && b->type == btype) \
			break; \
		else \
			OVERLOAD(vm, name, a, b); \
	} while(0)
