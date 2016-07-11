#include "../parser.h"

#include <stdlib.h>

l_p_expr_func_call* l_parse_expr_func_call(l_scanner* stream, l_p_expr* func)
{
	l_p_expr_func_call* expr = malloc(sizeof(l_p_expr_func_call));
	expr->func = func;

	// (
	l_scanner_skip(stream, TOKEN_OPENPAREN, "func call");

	// expression list
	expr->arg_list = l_parse_comma_expr_list(stream);

	// )
	l_scanner_skip(stream, TOKEN_CLOSEPAREN, "func call");

	return expr;
}

void l_pretty_expr_func_call(
		l_p_expr_func_call* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);

	l_pretty_expr(expr->func, depth, file);
	fprintf(file, "(");
	l_pretty_comma_expr_list(expr->arg_list, depth, file);
	fprintf(file, ")");
}
