#include "../parser.h"

#include <stdlib.h>

l_p_expression_char_literal* l_parse_expression_char_literal(l_scanner* stream)
{
	l_p_expression_char_literal* expr = malloc(sizeof(l_p_expression_char_literal));

	// char literal
	l_token c = l_scanner_next(stream);
	expr->character = c.content[0];

	return expr;
}

void l_pretty_expression_char_literal(
		l_p_expression_char_literal* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);
	fprintf(file, "%s'%c'", tabs, expr->character);
}
