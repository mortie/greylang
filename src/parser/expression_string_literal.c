#include "../parser.h"

#include <stdlib.h>

l_p_expression_string_literal* l_parse_expression_string_literal(l_scanner* stream)
{
	l_p_expression_string_literal* expr = malloc(sizeof(l_p_expression_string_literal));

	// string literal
	l_token str = l_scanner_next(stream);
	expr->string = str.content;

	return expr;
}

void l_pretty_expression_string_literal(
		l_p_expression_string_literal* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);
	fprintf(file, "%s\"%s\"", tabs, expr->string);
}
