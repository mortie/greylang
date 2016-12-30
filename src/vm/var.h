/*
 * Included from vm.h
 */

/*
 * Variable type
 */

typedef enum vm_var_type
{
	VAR_TYPE_OBJECT,
	VAR_TYPE_FUNCTION,
	VAR_TYPE_ARRAY,
	VAR_TYPE_NUMBER,
	VAR_TYPE_BOOL,
	VAR_TYPE_CHAR,
	VAR_TYPE_NONE
} vm_var_type;

/*
 * Array
 */

typedef struct vm_var_array
{
	vm_var **vars;
	int varc;
	int allocd;
	vm_var_type type;
} vm_var_array;

// Init the array with a type
void vm_var_array_init(vm_var_array *arr, vm_var_type type);

// Return a string, allocated with malloc
char *vm_var_array_tostring(vm_var_array *arr);

// Set an index to a value, if var is of an appropriate type
// Returns 0 on success, -1 on error
int vm_var_array_set(vm_var_array *arr, int index, vm_var *var);

// Get a value from an index
vm_var *vm_var_array_get(vm_var_array *arr, int index);

// Free the array, and decrement refcounts
void vm_var_array_free(vm_var_array *arr);

/*
 * Function
 */

typedef struct vm_var_function
{
	// If not null, parent will be executed,
	// but with this function's self
	struct vm_var_function *parent;

	// If not null, function pointer will be executed
	vm_var *(*fptr)(l_vm *vm, vm_var *self, vm_var_array *args, int infix);

	// Will be executed if parent and fptr is null
	l_p_expr **exprs;
	int exprc;

	// The names of the arguments, if executed with exprs.
	char **argnames;
	int argnamec;

	vm_map *scope;
	vm_var *self;
	int refs;
} vm_var_function;

// Init function
void vm_var_function_init(vm_var_function *func, vm_map *scope);

// Execute function
vm_var *vm_var_function_exec(
		l_vm *vm,
		vm_var_function *func,
		vm_var_array *args,
		vm_var *self,
		int infix);

// Get a function with the 'self' value set to something else.
// Increases the ref count of parent.
void vm_var_function_with_self(
		vm_var_function *parent,
		vm_var_function *func,
		vm_var *self);

// Free function. Decreases the ref count of parent.
void vm_var_function_free(vm_var_function *func);

// Increase or decrease the ref count.
// Function is free'd if counter reaches 0.
void vm_var_function_increfs(vm_var_function *func);
void vm_var_function_decrefs(vm_var_function *func);

/*
 * Char
 */

typedef uint32_t vm_var_char;

// Get a vm_var_char from a char*
vm_var_char vm_var_char_from_utf8(char *str);

// Convert a utf8 string to an aray of vm_var_char
void vm_var_char_array_from_utf8(char *str, vm_var_array *arr);

// Convert a vm_var_char to utf8.
// buf must be at least 5 bytes.
void vm_var_char_to_utf8(vm_var_char ch, char *buf);

// Convert an array of vm_var_char to a utf8 string.
// Returns a string allocated with malloc.
char *vm_var_char_array_to_utf8(vm_var_array *arr);

/*
 * Variable
 */

typedef struct vm_var
{
	union
	{
		vm_var_array *array;
		vm_var_function *function;
		double number;
		int boolean;
		vm_var_char character;
	} var;
	vm_var_type type;
	int refs;
	vm_map *map;
} vm_var;

// Create a new vm_var, allocated on the heap with malloc.
vm_var *vm_var_create(vm_var_type type);

// Free variable
void vm_var_free(vm_var *var);

// Increase or decrease the ref count.
// Variable is free'd if counter reaches 0.
void vm_var_increfs(vm_var *var);
void vm_var_decrefs(vm_var *var);

/*
 * Utility functions
 */

// Returns 1 if equal, 0 otherwise
int vm_var_equals(vm_var *a, vm_var *b);

// Returns a string, allocated by malloc
char *vm_var_tostring(vm_var *var);
