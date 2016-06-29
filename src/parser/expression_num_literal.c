#include "../parser.h"

#include <stdlib.h>

l_p_expression_num_literal* l_parse_expression_num_literal(l_scanner* stream)
{
	l_p_expression_num_literal* expr = malloc(sizeof(l_p_expression_num_literal));

	// num literal
	l_token num = l_scanner_next(stream);
	expr->number = atof(num.content);

	return expr;
}
void l_pretty_expression_num_literal(
		l_p_expression_num_literal* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);
	fprintf(file, "%s%f", tabs, expr->number);
}
