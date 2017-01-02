#include "../parser.h"

#include <stdlib.h>

l_p_expr_group *l_parse_expr_group(l_scanner *stream)
{
	l_p_expr_group *expr = malloc(sizeof(*expr));

	// (
	l_scanner_skip(stream, TOKEN_OPENPAREN, "expression group");

	// expression
	expr->expr = l_parse_expr(stream);

	// )
	l_scanner_skip(stream, TOKEN_CLOSEPAREN, "expression group");

	return expr;
}
void l_pretty_expr_group(
		l_p_expr_group *expr,
		int depth,
		FILE *file)
{
	fprintf(file, "(");
	l_pretty_expr(expr->expr, depth, file);
	fprintf(file, ")");
}
