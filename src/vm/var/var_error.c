#include "../../vm.h"

#include <stdlib.h>

void vm_var_error_init(l_vm *vm, vm_var_error *err, char *msg)
{
	err->file = malloc(strlen(vm->currfile) + 1);
	strcpy(err->file, vm->currfile);

	err->line = vm->currline;

	err->msg = malloc(strlen(msg) + 1);
	strcpy(err->msg, msg);
}

void vm_var_error_print(vm_var_error *err, FILE *f)
{
	fprintf(f, "Error: %s:%i: %s\n", err->file, err->line, err->msg);
}

void vm_var_error_free(vm_var_error *err)
{
	free(err->file);
	free(err->msg);
}
