#include "../parser.h"

#include <stdlib.h>

l_p_expr_array_literal* l_parse_expr_array_literal(l_scanner* stream)
{
	l_p_expr_array_literal* expr = malloc(sizeof(l_p_expr_array_literal));

	// [
	l_scanner_skip(stream, TOKEN_OPENBRACKET, "array literal");

	// expression list
	expr->expr_list = l_parse_comma_expr_list(stream);

	// ]
	l_scanner_skip(stream, TOKEN_CLOSEBRACKET, "array literal");

	return expr;
}

void l_pretty_expr_array_literal(
		l_p_expr_array_literal* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);

	fprintf(file, "%s[\n", tabs);
	l_pretty_comma_expr_list(expr->expr_list, depth + 1, file);
	fprintf(file, "%s]", tabs);
}
