#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"
#include "scanner.h"

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
	EXPRESSION_FUNC_CALL
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

/*
 * Function Call Expression
 */
typedef struct l_p_expression_func_call
{
	char* name;
	l_p_expression_list* arglist;
} l_p_expression_func_call;
l_p_expression_func_call* l_parse_expression_func_call(l_scanner* stream);

/*
 * Expression (For real this time)
 */
typedef struct l_p_expression
{
	union expression
	{
		l_p_expression_assignment* assignment;
		l_p_expression_func_call* func_call;
		l_p_expression_empty* empty;
	} expression;
	l_p_expression_type type;
} l_p_expression;
l_p_expression* l_parse_expression(l_scanner* stream);

#endif
