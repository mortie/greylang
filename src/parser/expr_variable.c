#include "../parser.h"

#include <stdlib.h>

l_p_expr_variable* l_parse_expr_variable(l_scanner* stream)
{
	l_p_expr_variable* expr = malloc(sizeof(l_p_expr_variable));

	// variable
	l_token var = l_scanner_expect(stream, TOKEN_NAME, "variable");
	expr->name = var.content;

	return expr;
}
void l_pretty_expr_variable(
		l_p_expr_variable* expr,
		int depth,
		FILE* file)
{
	fprintf(file, "%s", expr->name);
}
