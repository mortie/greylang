#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"
#include "scanner.h"

/*
 * Argument List
 */
typedef struct l_p_arg_list
{
	char** names;
	int namec;
} l_p_arg_list;

l_p_arg_list* l_parse_arg_list(l_scanner* stream);

void l_pretty_arg_list(
		l_p_arg_list* arg_list,
		int depth,
		FILE* file);

/*
 * Expression (temporary, to make types happy)
 */
typedef struct l_p_expression l_p_expression;

/*
 * Expression Types
 */
typedef enum l_p_expression_type
{
	EXPRESSION_EMPTY,
	EXPRESSION_ASSIGNMENT,
	EXPRESSION_FUNC_CALL,
	EXPRESSION_FUNCTION
} l_p_expression_type;

/*
 * Expression List
 */
typedef struct l_p_expression_list
{
	l_p_expression** expressions;
	int expressionc;
} l_p_expression_list;

l_p_expression_list* l_parse_expression_list(l_scanner* stream);

void l_pretty_expression_list(
		l_p_expression_list* list,
		int depth,
		FILE* file);

/*
 * Empty Expression
 */
typedef struct l_p_expression_empty l_p_expression_empty;

/*
 * Assignment Expression
 */
typedef struct l_p_expression_assignment
{
	char* name;
	l_p_expression* expression;
} l_p_expression_assignment;

l_p_expression_assignment* l_parse_expression_assignment(l_scanner* stream);

void l_pretty_expression_assignment(
		l_p_expression_assignment* expr,
		int depth,
		FILE* file);

/*
 * Function Call Expression
 */
typedef struct l_p_expression_func_call
{
	char* name;
	l_p_expression_list* expr_list;
} l_p_expression_func_call;

l_p_expression_func_call* l_parse_expression_func_call(l_scanner* stream);

void l_pretty_expression_func_call(
		l_p_expression_func_call* expr,
		int depth,
		FILE* file);

/*
 * Function Expression
 */
typedef struct l_p_expression_function
{
	l_p_arg_list* arg_list;
	l_p_expression_list* expr_list;
} l_p_expression_function;

l_p_expression_function* l_parse_expression_function(l_scanner* stream);

void l_pretty_expression_function(
		l_p_expression_function* expr,
		int depth,
		FILE* file);

/*
 * Expression (For real this time)
 */
typedef struct l_p_expression
{
	union expression
	{
		l_p_expression_empty* empty;
		l_p_expression_assignment* assignment;
		l_p_expression_func_call* func_call;
		l_p_expression_function* function;
	} expression;
	l_p_expression_type type;
} l_p_expression;

l_p_expression* l_parse_expression(l_scanner* stream);

void l_pretty_expression(
		l_p_expression* expr,
		int depth,
		FILE* file);

#endif
