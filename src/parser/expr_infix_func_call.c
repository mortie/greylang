#include "../parser.h"

#include <stdlib.h>

l_p_expr_func_call* l_parse_expr_infix_func_call(l_scanner* stream, l_p_expr* val1)
{
	l_p_expr_func_call* expr = malloc(sizeof(l_p_expr_func_call));

	expr->arg_list = malloc(sizeof(l_p_comma_expr_list));
	expr->arg_list->expressionc = 2;
	expr->arg_list->expressions = malloc(sizeof(l_p_expr) * 2);

	// expr1
	expr->arg_list->expressions[0] = val1;

	// name
	expr->func = malloc(sizeof(l_p_expr));
	expr->func->type = EXPR_VARIABLE;
	expr->func->expression.variable = l_parse_expr_variable(stream);

	// expr2
	expr->arg_list->expressions[1] = l_parse_expr(stream);

	return expr;
}
