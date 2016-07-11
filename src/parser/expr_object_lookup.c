#include "../parser.h"

#include <stdlib.h>

l_p_expr_object_lookup* l_parse_expr_object_lookup(l_scanner* stream, l_p_expr* obj)
{
	l_p_expr_object_lookup* expr = malloc(sizeof(l_p_expr_object_lookup));
	expr->obj = obj;

	// .
	l_scanner_skip(stream, TOKEN_PERIOD, "object lookup");

	// name
	expr->key = l_scanner_expect(stream, TOKEN_NAME, "object lookup")
		.content;

	return expr;
}

void l_pretty_expr_object_lookup(
		l_p_expr_object_lookup* expr,
		int depth,
		FILE* file)
{
	l_pretty_expr(expr->obj, depth, file);
	fprintf(file, ".%s", expr->key);
}
