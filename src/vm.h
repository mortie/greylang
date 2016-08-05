#ifndef L_VM_H
#define L_VM_H

#include "parser.h"

/*
 * Vm, Scope and Var (temporary, to make types happy)
 */

typedef struct l_vm l_vm;
typedef struct l_vm_scope l_vm_scope;
typedef struct l_vm_var l_vm_var;

/*
 * Map
 */

typedef struct l_vm_map
{
	struct l_vm_map* internal;
	char** names;
	l_vm_var** vars;
	int len;
	int allocd;
	struct l_vm_map* proto;
	int gc_marked;
} l_vm_map;

l_vm_map* l_vm_map_create(l_vm_map* proto);

void l_vm_map_set(
		l_vm_map* map,
		char* name,
		l_vm_var* var);

void l_vm_map_set_internal(
		l_vm_map* map,
		char* name,
		l_vm_var* var);

l_vm_var* l_vm_map_shallow_lookup(l_vm_map* map, char* name);
l_vm_var* l_vm_map_shallow_lookup_internal(l_vm_map* map, char* name);
l_vm_var* l_vm_map_lookup(l_vm_map* map, char* name);
void l_vm_map_free(l_vm_map* map);

/*
 * Variables
 */

typedef enum l_vm_var_type
{
	VAR_TYPE_OBJECT,
	VAR_TYPE_ARRAY,
	VAR_TYPE_FUNCTION,
	VAR_TYPE_STRING,
	VAR_TYPE_NUMBER,
	VAR_TYPE_BOOL,
	VAR_TYPE_ERROR,
	VAR_TYPE_PTR,
	VAR_TYPE_NONE
} l_vm_var_type;

// All variables are maps; the object type just doesn't have anything else
typedef struct l_vm_var_object l_vm_var_object;

typedef struct l_vm_var_array
{
	l_vm_var** vars;
	int len;
	int allocd;
	int gc_marked;
} l_vm_var_array;

l_vm_var_array* l_vm_var_array_create();
void l_vm_var_array_resize(l_vm* vm, l_vm_var_array* arr, int nwsize);
void l_vm_var_array_free(l_vm_var_array* arr);

typedef struct l_vm_var_function
{
	l_vm_var* (*fptr)(l_vm* vm, l_vm_var*, l_vm_var_array*, int infix);

	l_p_expr** expressions;
	int expressionc;

	l_vm_scope* scope;
	char** argnames;
	int argnamec;
	l_vm_var* self;
	int gc_marked;
} l_vm_var_function;

l_vm_var_function* l_vm_var_function_create(l_vm_scope* scope);
l_vm_var* l_vm_var_function_exec(
		l_vm* vm,
		l_vm_var_function* func,
		l_vm_var_array* args,
		int infix);

l_vm_var* l_vm_var_function_set_self(
		l_vm* vm,
		l_vm_var_function* func,
		l_vm_var* obj);

void l_vm_var_function_free(l_vm_var_function* func);

typedef struct l_vm_var_string
{
	char* chars;
	int len;
	// No gc_marked because immutable
} l_vm_var_string;

void l_vm_var_string_free(l_vm_var_string* str);

typedef struct l_vm_var_error
{
	char* msg;
	int line;
	int gc_marked;
} l_vm_var_error;

l_vm_var* l_vm_error(l_vm* vm, char* str);
l_vm_var* l_vm_error_type(l_vm* vm, l_vm_var_type expected, l_vm_var_type got);
l_vm_var* l_vm_error_argc(l_vm* vm, int expected, int got);
void l_vm_var_error_free(l_vm_var_error* err);

typedef struct l_vm_var_none l_vm_var_none;

typedef struct l_vm_var
{
	union
	{
		l_vm_var_object* object;
		l_vm_var_array* array;
		l_vm_var_function* function;
		l_vm_var_string* string;
		double number;
		int boolean;
		l_vm_var_error* error;
		void* ptr;
		l_vm_var_none* none;
	} var;
	l_vm_map* map;
	l_vm_var_type type;
	int gc_marked;
} l_vm_var;

l_vm_var* l_vm_var_create(l_vm* vm, l_vm_var_type type);
l_vm_var* l_vm_var_copy(l_vm* vm, l_vm_var* var);

char* l_vm_var_tostring(l_vm_var* var);
int l_vm_var_eq(l_vm_var* var1, l_vm_var* var2);

/*
 * Standard Library
 */

// +
l_vm_var* l_vm_std_add(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// -
l_vm_var* l_vm_std_sub(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// *
l_vm_var* l_vm_std_mul(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// /
l_vm_var* l_vm_std_div(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// ^
l_vm_var* l_vm_std_pow(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// ==
l_vm_var* l_vm_std_eq(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// !=
l_vm_var* l_vm_std_neq(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// >
l_vm_var* l_vm_std_gt(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// <
l_vm_var* l_vm_std_lt(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// >=
l_vm_var* l_vm_std_gteq(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// <=
l_vm_var* l_vm_std_lteq(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// and
l_vm_var* l_vm_std_and(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// or
l_vm_var* l_vm_std_or(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// if
l_vm_var* l_vm_std_if(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// repeat
l_vm_var* l_vm_std_repeat(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// while
l_vm_var* l_vm_std_while(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// for
l_vm_var* l_vm_std_for(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// tostring
l_vm_var* l_vm_std_tostring(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// tonumber
l_vm_var* l_vm_std_tonumber(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// concat ..
l_vm_var* l_vm_std_concat(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// error
l_vm_var* l_vm_std_error(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// type
l_vm_var* l_vm_std_type(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// loadc
l_vm_var* l_vm_std_loadc(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// print
l_vm_var* l_vm_std_print(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// read
l_vm_var* l_vm_std_read(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// array.len
l_vm_var* l_vm_std_array_len(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// array.push
l_vm_var* l_vm_std_array_push(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// array.pop
l_vm_var* l_vm_std_array_pop(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// array.map
l_vm_var* l_vm_std_array_map(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// array.iter
l_vm_var* l_vm_std_array_iter(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

// string.len
l_vm_var* l_vm_std_string_len(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// string.sub
l_vm_var* l_vm_std_string_sub(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);
// string.iter
l_vm_var* l_vm_std_string_iter(l_vm* vm, l_vm_var* self, l_vm_var_array* args, int infix);

/*
 * Scope
 */

typedef struct l_vm_scope
{
	l_vm_map* map;
	struct l_vm_scope* parent;
	int immutable;
	l_vm_scope** childs;
	int childc;
	int childa;
} l_vm_scope;

l_vm_scope* l_vm_scope_create(l_vm_scope* parent);
l_vm_var* l_vm_scope_shallow_lookup(l_vm_scope* scope, char* name);
l_vm_var* l_vm_scope_lookup(l_vm_scope* scope, char* name);
void l_vm_scope_define(l_vm_scope* scope, char* name, l_vm_var* var);
void l_vm_scope_set(l_vm_scope* scope, char* name, l_vm_var* var);
void l_vm_scope_free(l_vm_scope* scope);

/*
 * GC
 */

void l_vm_gc(l_vm* vm);

/*
 * VM
 */

typedef struct l_vm
{
	l_vm_scope* global;

	l_vm_map* proto_array;
	l_vm_map* proto_string;

	l_vm_var** vars;
	int varc;
	int vara;

	int currLine;
} l_vm;

l_vm* l_vm_create();
l_vm_var* l_vm_exec(l_vm* vm, l_vm_scope* scope, l_p_expr** expressions, int expressionc);
l_vm_var* l_vm_run(l_vm* vm, l_p_expr_list* list);

#endif
