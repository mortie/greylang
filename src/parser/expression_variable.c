#include "../parser.h"

#include <stdlib.h>

l_p_expression_variable* l_parse_expression_variable(l_scanner* stream)
{
	l_p_expression_variable* expr = malloc(sizeof(l_p_expression_variable));

	// variable
	l_token var = l_scanner_expect(stream, TOKEN_NAME, "variable");
	expr->name = var.content;

	return expr;
}
void l_pretty_expression_variable(
		l_p_expression_variable* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);
	fprintf(file, "%s%s", tabs, expr->name);
}
