#include "../vm.h"

#include <stdlib.h>

vm_var *vm_var_create()
{
	vm_var *var = malloc(sizeof(*var));
	var->refs = 0;
	return var;
}

static void freevar(vm_var *var)
{
	free(var);
}

void vm_var_increfs(vm_var *var)
{
	var->refs += 1;
}

void vm_var_decrefs(vm_var *var)
{
	var->refs -= 1;
	if (var->refs <= 0)
		freevar(var);
}
