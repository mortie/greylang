#include "../../vm.h"
#include "defines.h"

#include <stdlib.h>

vm_var *vm_std_if(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	if (args->varc < 2 || args->varc > 3)
		EXPECTARGC(vm, 2, args);

	vm_var* cond = args->vars[0];
	vm_var* ifFn = args->vars[1];
	vm_var *elseFn = NULL;
	if (args->varc == 3)
		elseFn = args->vars[2];

	RETIFERR(cond);
	RETIFERR(ifFn);
	RETIFERR(elseFn);

	if (infix) SWAP(cond, ifFn);

	if (cond->var.boolean && ifFn->type == VAR_TYPE_FUNCTION)
		return vm_var_function_call(vm, ifFn->var.function, NULL);
	else if (cond->var.boolean)
		return ifFn;
	else if (elseFn == NULL)
		return vm->var_none;
	else if (elseFn->type == VAR_TYPE_FUNCTION)
		return vm_var_function_call(vm, elseFn->var.function, NULL);
	else
		return elseFn;
}

vm_var *vm_std_repeat(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var *num = args->vars[0];
	vm_var *func = args->vars[1];
	if (infix) SWAP(num, func);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, num);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

	vm_var *counter = vm_var_create(VAR_TYPE_NUMBER);
	vm_var_array *arr = malloc(sizeof(*arr));
	vm_var_array_init(arr, VAR_TYPE_NONE);
	vm_var_array_set(arr, 0, counter);

	for (int i = 0; i < num->var.number; ++i)
	{
		counter->var.number = (double)i;
		vm_var *ret = vm_var_function_call(vm, func->var.function, arr);
		RETIFERR(ret);
	}

	vm_var_array_free(arr);
	free(arr);

	return vm->var_none;
}


vm_var *vm_std_while(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var *cond = args->vars[0];
	vm_var *func = args->vars[1];
	if (infix) SWAP(cond, func);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, cond);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

	while (1)
	{
		vm_var *res = vm_var_function_call(vm, cond->var.function, NULL);
		RETIFERR(res);
		if (res->type != VAR_TYPE_BOOL || !res->var.boolean)
			break;

		res = vm_var_function_call(vm, func->var.function, NULL);
		RETIFERR(res);
	}

	return vm->var_none;
}

vm_var *vm_std_for(l_vm *vm, vm_var *self, vm_var_array *args, int infix)
{
	EXPECTARGC(vm, 2, args);
	vm_var *iterable = args->vars[0];
	vm_var *func = args->vars[1];
	if (infix) SWAP(iterable, func);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

	// Call $iter, put result in 'iter'
	vm_var *iterv = vm_map_lookup_r(iterable->map, "$iter");
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, iterv);
	vm_var *iter = vm_var_function_exec(
		vm, iterv->var.function, NULL, iterable, 0);
	EXPECTTYPE(vm, VAR_TYPE_OBJECT, iter);
	vm_var_increfs(iter);

	// Get iter.$next, put resulting vm_var_function in 'nextf'
	vm_var *nextv = vm_map_lookup_r(iter->map, "$next");
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, nextv);
	vm_var_function *nextf = nextv->var.function;

	// Arr for the function args
	vm_var_array arr;
	vm_var_array_init(&arr, VAR_TYPE_NONE);

	// Create counter and set it as the second argument
	vm_var *counter = vm_var_create(VAR_TYPE_NUMBER);
	counter->var.number = 0;
	vm_var_array_set(&arr, 1, counter);

	while (1)
	{
		vm_var *val = vm_var_function_call(vm, nextf, NULL);

		if (val->type == VAR_TYPE_NONE)
			break;


		// Update first argument
		vm_var_array_set(&arr, 0, val);

		vm_var_function_call(vm, func->var.function, &arr);

		counter->var.number += 1;
	}

	vm_var_array_free(&arr);
	vm_var_decrefs(iter);
	return vm->var_none;
}
