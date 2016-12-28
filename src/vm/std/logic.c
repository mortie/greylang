#include "../../vm.h"
#include "defines.h"


vm_var *vm_std_eq(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var* v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 1; i < args->varc; ++i)
	{
		if (!vm_var_equals(args->vars[i-1], args->vars[i]))
			return v;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_neq(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	vm_var *v = vm_std_eq(vm, self, args, infix);
	v->var.boolean = !v->var.boolean;
	return v;
}

vm_var *vm_std_gt(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var *v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev > d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_lt(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var *v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev < d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_gteq(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var *v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev >= d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_lteq(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var *v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev <= d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_and(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var* v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 0; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_BOOL, args->vars[i]);
		if (!args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 1;
	return v;
}

vm_var *vm_std_or(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2)
		EXPECTARGC(vm, 2, args);

	vm_var* v = vm_var_create(VAR_TYPE_BOOL);
	v->var.boolean = 1;

	for (int i = 0; i < args->varc; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_BOOL, args->vars[i]);
		if (args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 0;
	return v;
}
