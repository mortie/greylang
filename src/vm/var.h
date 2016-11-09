/*
 * Included from vm.h
 */

typedef struct vm_var
{
	int refs;
} vm_var;

/*
 * Create a new vm_var, allocated on the heap with malloc.
 */
vm_var *vm_var_create();

/*
 * Increase or decrease the ref count.
 * Variable is freed if counter reaches 0.
 */
void vm_var_increfs(vm_var *var);
void vm_var_decrefs(vm_var *var);
