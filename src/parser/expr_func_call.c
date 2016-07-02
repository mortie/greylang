#include "../parser.h"

#include <stdlib.h>

l_p_expr_func_call* l_parse_expr_func_call(l_scanner* stream)
{
	l_p_expr_func_call* expr = malloc(sizeof(l_p_expr_func_call));

	// name
	l_token name = l_scanner_expect(stream, TOKEN_NAME, "func call");
	expr->name = name.content;

	// (
	l_scanner_skip(stream, TOKEN_OPENPAREN, "func call");

	// expression list
	expr->arg_list = l_parse_arg_expr_list(stream);

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

	fprintf(file, "%s%s(\n", tabs, expr->name);
	l_pretty_arg_expr_list(expr->arg_list, depth + 1, file);
	fprintf(file, "%s)", tabs);
}
