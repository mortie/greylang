#include "src/test.h"
#include "../src/vm.h"

void vm_array()
{
	it("sets and retrieves values") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_NUMBER);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 10.0;
		vm_var_array_set(&arr, 0, in);

		double num = vm_var_array_get(&arr, 0)->var.number;
		asserteq(num, 10.0);

		vm_var_array_free(&arr);
	};

	it("correctly resizes with consecutive inserts") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_NUMBER);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 20.5;

		vm_var_array_set(&arr, 0, in);
		vm_var_array_set(&arr, 1, in);
		vm_var_array_set(&arr, 2, in);
		vm_var_array_set(&arr, 3, in);
		vm_var_array_set(&arr, 4, in);

		double num;
		num = vm_var_array_get(&arr, 0)->var.number; asserteq(num, 20.5);
		num = vm_var_array_get(&arr, 1)->var.number; asserteq(num, 20.5);
		num = vm_var_array_get(&arr, 2)->var.number; asserteq(num, 20.5);
		num = vm_var_array_get(&arr, 3)->var.number; asserteq(num, 20.5);
		num = vm_var_array_get(&arr, 4)->var.number; asserteq(num, 20.5);

		vm_var_array_free(&arr);
	}

	it("handles arbitrary insertions") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_NUMBER);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 11.0;

		vm_var_array_set(&arr, 17, in);
		double num = vm_var_array_get(&arr, 17)->var.number;
		asserteq(num, 11.0);

		vm_var_array_free(&arr);
	}

	done();
}
