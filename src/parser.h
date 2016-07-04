#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"
#include "scanner.h"

#include <stdio.h>
#include <string.h>

#define l_parse l_parse_expr_list
#define l_pretty_print(list, f) l_pretty_expr_list(list, 0, f)

#define P_TABS(n, var) \
	do { \
		var = alloca(n + 1); \
		var[n] = '\0'; \
		memset(var, '\t', n); \
	} while(0)

/*
 * Argument Definition
 */

typedef struct l_p_arg_definition
{
	char** names;
	int namec;
} l_p_arg_definition;

l_p_arg_definition* l_parse_arg_definition(l_scanner* stream);

void l_pretty_arg_definition(
		l_p_arg_definition* arg_definition,
		int depth,
		FILE* file);

/*
 * Expression (temporary, to make types happy)
 */

typedef struct l_p_expr l_p_expr;

/*
 * Expression Types
 */

typedef enum l_p_expr_type
{
	EXPR_EMPTY,
	EXPR_ASSIGNMENT,
	EXPR_FUNC_CALL,
	EXPR_FUNCTION,
	EXPR_ARRAY_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_NUM_LITERAL,
	EXPR_VARIABLE
} l_p_expr_type;

/*
 * Expression List
 */

typedef struct l_p_expr_list
{
	l_p_expr** expressions;
	int expressionc;
} l_p_expr_list;

l_p_expr_list* l_parse_expr_list(l_scanner* stream);

void l_pretty_expr_list(
		l_p_expr_list* list,
		int depth,
		FILE* file);

/*
 * Argument Expression List
 */

typedef struct l_p_comma_expr_list
{
	l_p_expr** expressions;
	int expressionc;
} l_p_comma_expr_list;

l_p_comma_expr_list* l_parse_comma_expr_list(l_scanner* stream);

void l_pretty_comma_expr_list(
		l_p_comma_expr_list* list,
		int depth,
		FILE* file);

/*
 * Empty Expression
 */

typedef struct l_p_expr_empty l_p_expr_empty;

void l_pretty_expr_empty(int depth, FILE* file);

/*
 * Assignment Expression
 */

typedef struct l_p_expr_assignment
{
	char* name;
	l_p_expr* expression;
} l_p_expr_assignment;

l_p_expr_assignment* l_parse_expr_assignment(l_scanner* stream);

void l_pretty_expr_assignment(
		l_p_expr_assignment* expr,
		int depth,
		FILE* file);

/*
 * Function Call Expression
 */

typedef struct l_p_expr_func_call
{
	l_p_expr* func;
	l_p_comma_expr_list* arg_list;
} l_p_expr_func_call;

l_p_expr_func_call* l_parse_expr_func_call(l_scanner* stream, l_p_expr* func);

void l_pretty_expr_func_call(
		l_p_expr_func_call* expr,
		int depth,
		FILE* file);

/*
 * Function Expression
 */

typedef struct l_p_expr_function
{
	l_p_arg_definition* arg_definition;
	l_p_expr_list* expr_list;
} l_p_expr_function;

l_p_expr_function* l_parse_expr_function(l_scanner* stream);

void l_pretty_expr_function(
		l_p_expr_function* expr,
		int depth,
		FILE* file);

/*
 * Array Literal Expression
 */

typedef struct l_p_expr_array_literal
{
	l_p_comma_expr_list* expr_list;
} l_p_expr_array_literal;

l_p_expr_array_literal* l_parse_expr_array_literal(l_scanner* stream);

void l_pretty_expr_array_literal(
		l_p_expr_array_literal* expr,
		int depth,
		FILE* file);

/*
 * String Literal Expression
 */

typedef struct l_p_expr_string_literal
{
	char* string;
} l_p_expr_string_literal;

l_p_expr_string_literal* l_parse_expr_string_literal(l_scanner* stream);

void l_pretty_expr_string_literal(
		l_p_expr_string_literal* expr,
		int depth,
		FILE* file);

/*
 * Char Literal Expression
 */

typedef struct l_p_expr_char_literal
{
	char character;
} l_p_expr_char_literal;

l_p_expr_char_literal* l_parse_expr_char_literal(l_scanner* stream);

void l_pretty_expr_char_literal(
		l_p_expr_char_literal* expr,
		int depth,
		FILE* file);

/*
 * Num Literal Expression
 */

typedef struct l_p_expr_num_literal
{
	double number;
} l_p_expr_num_literal;

l_p_expr_num_literal* l_parse_expr_num_literal(l_scanner* stream);

void l_pretty_expr_num_literal(
		l_p_expr_num_literal* expr,
		int depth,
		FILE* file);

/*
 * Variable Expression
 */

typedef struct l_p_expr_variable
{
	char* name;
} l_p_expr_variable;

l_p_expr_variable* l_parse_expr_variable(l_scanner* stream);

void l_pretty_expr_variable(
		l_p_expr_variable* expr,
		int depth,
		FILE* file);

/*
 * Expression (For real this time)
 */

typedef struct l_p_expr
{
	union expression
	{
		l_p_expr_empty* empty;
		l_p_expr_assignment* assignment;
		l_p_expr_func_call* func_call;
		l_p_expr_function* function;
		l_p_expr_array_literal* array_literal;
		l_p_expr_num_literal* num_literal;
		l_p_expr_string_literal* string_literal;
		l_p_expr_char_literal* char_literal;
		l_p_expr_variable* variable;
	} expression;
	l_p_expr_type type;
} l_p_expr;

l_p_expr* l_parse_expr(l_scanner* stream);

void l_pretty_expr(
		l_p_expr* expr,
		int depth,
		FILE* file);

#endif
