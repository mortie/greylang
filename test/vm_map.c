#include "src/test.h"
#include "../src/vm.h"

void test_vm_map()
{
	it("defines and retrieves values") {
		vm_map *map = vm_map_create(NULL);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 10.0;
		vm_map_define(map, "testkey", in);

		double num = vm_map_lookup(map, "testkey")->var.number;
		asserteq(num, 10.0);

		vm_map_free(map);
	}

	it("replaces values correctly") {
		vm_map *map = vm_map_create(NULL);

		vm_var *in1 = vm_var_create(VAR_TYPE_NUMBER);
		in1->var.number = 10.0;
		vm_var *in2 = vm_var_create(VAR_TYPE_NUMBER);
		in2->var.number = 20.0;

		vm_map_define(map, "testkey", in1);
		double num = vm_map_lookup(map, "testkey")->var.number;
		asserteq(num, 10.0);

		vm_map_replace(map, "testkey", in2);
		num = vm_map_lookup(map, "testkey")->var.number;
		asserteq(num, 20.0);

		vm_map_free(map);
	}

	it("doesn't accept replacing an undefined key") {
		vm_map *map = vm_map_create(NULL);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 10.0;
		vm_var_increfs(in);

		int ret = vm_map_replace(map, "testkey", in);
		asserteq(ret, -1);

		vm_map_free(map);
		vm_var_decrefs(in);
	}

	it("returns NULL when looking up undefined keys") {
		vm_map *map = vm_map_create(NULL);

		vm_var *ret = vm_map_lookup(map, "testkey");
		asserteq(ret, NULL);

		vm_map_free(map);
	}

	it("recursively looks up values") {
		vm_map *parent = vm_map_create(NULL);
		vm_map *map = vm_map_create(parent);

		vm_var *in = vm_var_create(VAR_TYPE_NUMBER);
		in->var.number = 10.0;
		vm_map_define(parent, "testkey", in);

		double num = vm_map_lookup_r(map, "testkey")->var.number;
		asserteq(num, 10.0);

		vm_map_free(map);
	}

	it("recursively replaces values") {
		vm_map *parent = vm_map_create(NULL);
		vm_map *map = vm_map_create(parent);

		vm_var *in1 = vm_var_create(VAR_TYPE_NUMBER);
		in1->var.number = 10.0;
		vm_map_define(parent, "testkey", in1);

		vm_var *in2 = vm_var_create(VAR_TYPE_NUMBER);
		in2->var.number = 20.0;
		vm_map_replace_r(map, "testkey", in2);

		double num = vm_map_lookup(parent, "testkey")->var.number;
		asserteq(num, 20.0); // 20.0 means it works, 10.0 means it doesn't

		vm_map_free(map);
	}

	done();
}
