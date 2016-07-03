#ifndef L_VM_H
#define L_VM_H

#include "parser.h"

/*
 * Scope and Var (temporary, to make types happy)
 */

typedef struct l_vm_scope l_vm_scope;
typedef struct l_vm_var l_vm_var;

/*
 * Variables
 */

typedef enum l_vm_var_type
{
	VAR_TYPE_ARRAY,
	VAR_TYPE_FUNCTION,
	VAR_TYPE_STRING,
	VAR_TYPE_CHAR,
	VAR_TYPE_NUMBER,
	VAR_TYPE_BOOL,
	VAR_TYPE_NONE
} l_vm_var_type;

typedef struct l_vm_var_array
{
	l_vm_var** vars;
	int len;
	int allocd;
} l_vm_var_array;

typedef enum l_vm_std_func
{
	STD_FUNC_NONE,
	STD_FUNC_ADD,
	STD_FUNC_SUB,
	STD_FUNC_MUL,
	STD_FUNC_DIV,
	STD_FUNC_POW,
	STD_FUNC_LOOKUP,
	STD_FUNC_IF,
	STD_FUNC_REPEAT,
	STD_FUNC_MAP,
	STD_FUNC_PRINT
} l_vm_std_func;

typedef struct l_vm_var_function
{
	l_vm_std_func stdfunc;
	l_p_expr** expressions;
	int expressionc;
	l_vm_scope* scope;
} l_vm_var_function;

l_vm_var_function* l_vm_var_function_create_std(
		l_vm_scope* scope,
		l_vm_std_func stdfunc);
l_vm_var_function* l_vm_var_function_create(
		l_vm_scope* scope);
l_vm_var* l_vm_var_function_exec(
		l_vm_var_function* func,
		l_vm_var_array* args);

typedef struct l_vm_var_string
{
	char* chars;
	int len;
} l_vm_var_string;

typedef struct l_vm_var_none l_vm_var_none;

typedef struct l_vm_var
{
	union var
	{
		l_vm_var_array* array;
		l_vm_var_function* function;
		l_vm_var_string* string;
		char character;
		double number;
		int boolean;
		l_vm_var_none* none;
	} var;
	l_vm_var_type type;
} l_vm_var;

l_vm_var* l_vm_var_create(l_vm_var_type type);
void l_vm_var_free(l_vm_var* var);

/*
 * Errors
 */

void l_vm_error_type(l_vm_var_type expected, l_vm_var_type got);
void l_vm_error_argnum(int expected, int got);
void l_vm_error_undefined(char* name);

/*
 * Standard Library
 */

l_vm_var* l_vm_std_add(l_vm_var_array* args);    // +
l_vm_var* l_vm_std_sub(l_vm_var_array* args);    // -
l_vm_var* l_vm_std_mul(l_vm_var_array* args);    // *
l_vm_var* l_vm_std_div(l_vm_var_array* args);    // /
l_vm_var* l_vm_std_pow(l_vm_var_array* args);    // ^
l_vm_var* l_vm_std_lookup(l_vm_var_array* args); // $
l_vm_var* l_vm_std_if(l_vm_var_array* args);     // if
l_vm_var* l_vm_std_repeat(l_vm_var_array* args); // repeat
l_vm_var* l_vm_std_map(l_vm_var_array* args);    // map
l_vm_var* l_vm_std_print(l_vm_var_array* args);  // print!

/*
 * Scope
 */

typedef struct l_vm_scope
{
	char** names;
	l_vm_var** vars;
	int varc;
	int vara;
	struct l_vm_scope* parent;
} l_vm_scope;

l_vm_scope* l_vm_scope_create(l_vm_scope* parent);
void l_vm_scope_set(l_vm_scope* scope, char* name, l_vm_var* var);
l_vm_var* l_vm_scope_shallow_lookup(l_vm_scope* scope, char* name);
l_vm_var* l_vm_scope_lookup(l_vm_scope* scope, char* name);
void l_vm_scope_free(l_vm_scope* scope);

/*
 * VM
 */

typedef struct l_vm
{
	l_vm_scope* global;
} l_vm;

l_vm* l_vm_create();
l_vm_var* l_vm_exec(l_vm_scope* scope, l_p_expr** expressions, int expressionc);
void l_vm_run(l_vm* vm, l_p_expr_list* list);

#endif
