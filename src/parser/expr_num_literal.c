#include "../parser.h"

#include <stdlib.h>

l_p_expr_num_literal* l_parse_expr_num_literal(l_scanner* stream)
{
	l_p_expr_num_literal* expr = malloc(sizeof(l_p_expr_num_literal));

	// num literal
	l_token num = l_scanner_expect(stream, TOKEN_NUM_LITERAL, "num literal");
	expr->number = atof(num.content);

	return expr;
}
void l_pretty_expr_num_literal(
		l_p_expr_num_literal* expr,
		int depth,
		FILE* file)
{
	fprintf(file, "%f", expr->number);
}
