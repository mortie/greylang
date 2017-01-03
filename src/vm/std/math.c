#include "../../vm.h"
#include "defines.h"

#include <math.h>

vm_var *vm_std_add(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var *a = args->vars[0];
	vm_var *b = args->vars[1];
	OVERLOAD_OR_TYPECHECK(
		vm, "$add",
		a, VAR_TYPE_NUMBER,
		b, VAR_TYPE_NUMBER);

	vm_var *v = vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number + b->var.number;
	return v;
}

vm_var* vm_std_sub(l_vm* vm, vm_var* self, vm_var_array* args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var* a = args->vars[0];
	vm_var* b = args->vars[1];
	OVERLOAD_OR_TYPECHECK(
		vm, "$sub",
		a, VAR_TYPE_NUMBER,
		b, VAR_TYPE_NUMBER);

	vm_var* v = vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number - b->var.number;
	return v;
}

vm_var* vm_std_mul(l_vm* vm, vm_var* self, vm_var_array* args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var* a = args->vars[0];
	vm_var* b = args->vars[1];
	OVERLOAD_OR_TYPECHECK(
		vm, "$mul",
		a, VAR_TYPE_NUMBER,
		b, VAR_TYPE_NUMBER);

	vm_var* v = vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number * b->var.number;
	return v;
}

vm_var* vm_std_div(l_vm* vm, vm_var* self, vm_var_array* args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var* a = args->vars[0];
	vm_var* b = args->vars[1];
	OVERLOAD_OR_TYPECHECK(
		vm, "$div",
		a, VAR_TYPE_NUMBER,
		b, VAR_TYPE_NUMBER);

	vm_var* v = vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number / b->var.number;
	return v;
}

vm_var* vm_std_pow(l_vm* vm, vm_var* self, vm_var_array* args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var* a = args->vars[0];
	vm_var* b = args->vars[1];
	OVERLOAD_OR_TYPECHECK(
		vm, "$pow",
		a, VAR_TYPE_NUMBER,
		b, VAR_TYPE_NUMBER);

	vm_var* v = vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = pow(a->var.number, b->var.number);
	return v;
}
