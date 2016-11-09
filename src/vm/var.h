/*
 * Included from vm.h
 */

typedef enum vm_var_type
{
	VAR_TYPE_OBJECT,
	VAR_TYPE_FUNCTION,
	VAR_TYPE_ARRAY,
	VAR_TYPE_NUMBER,
	VAR_TYPE_BOOL,
	VAR_TYPE_NONE
} vm_var_type;

/*
 * Arrays
 */

typedef struct vm_var_array
{
	vm_var **vars;
	int varc;
	int allocd;
} vm_var_array;

void vm_var_array_init(vm_var_array *arr);

/*
 * Functions
 */

typedef struct vm_var_function
{
	vm_var *(*fptr)(l_vm *vm, vm_var *var, int infix);

	l_p_expr **exprs;
	int exprc;

	char **argnames;
	int argnamec;
	vm_var *self;
} vm_var_function;

void vm_var_function_init(vm_var_function *func);

vm_var *vm_var_function_exec(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args,
		int infix);

typedef struct vm_var
{
	union
	{
		vm_var_array array;
		vm_var_function function;
		double number;
		int boolean;
	} var;
	vm_var_type type;
	int refs;
	vm_map *map;
} vm_var;

/*
 * Create a new vm_var, allocated on the heap with malloc.
 */
vm_var *vm_var_create(vm_var_type type);

/*
 * Increase or decrease the ref count.
 * Variable is freed if counter reaches 0.
 */
void vm_var_increfs(vm_var *var);
void vm_var_decrefs(vm_var *var);

/*
 * Utility functions
 */

// Returns 1 if equal, 0 otherwise
int vm_var_equals(vm_var *a, vm_var *b);

// Returns a string, allocated by malloc
char *vm_var_tostring(vm_var *var);
