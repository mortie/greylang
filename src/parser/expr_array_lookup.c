#include "../parser.h"

#include <stdlib.h>

l_p_expr_array_lookup *l_parse_expr_array_lookup(l_scanner *stream, l_p_expr *arr)
{
	l_p_expr_array_lookup *expr = malloc(sizeof(*expr));
	expr->arr = arr;

	// [
	l_scanner_skip(stream, TOKEN_OPENBRACKET, "array lookup");

	// expression
	expr->key = l_parse_expr(stream);

	// ]
	l_scanner_skip(stream, TOKEN_CLOSEBRACKET, "array lookup");

	return expr;
}

void l_pretty_expr_array_lookup(
		l_p_expr_array_lookup *expr,
		int depth,
		FILE *file)
{
	l_pretty_expr(expr->arr, depth, file);
	fprintf(file, "[");
	l_pretty_expr(expr->key, 0, file);
	fprintf(file, "]");
}
