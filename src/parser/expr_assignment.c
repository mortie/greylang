#include "../parser.h"

#include <stdlib.h>

l_p_expr_assignment *l_parse_expr_assignment(l_scanner *stream, l_p_expr *key)
{
	l_p_expr_assignment *expr = malloc(sizeof(*expr));

	// key
	expr->key = key;

	// =
	l_scanner_skip(stream, TOKEN_EQUALS, "assignment");

	// val
	expr->val = l_parse_expr(stream);

	return expr;
}

void l_pretty_expr_assignment(
		l_p_expr_assignment *expr,
		int depth,
		FILE *file)
{
	l_pretty_expr(expr->key, depth, file);
	fprintf(file, " = ");
	l_pretty_expr(expr->val, depth, file);
}
