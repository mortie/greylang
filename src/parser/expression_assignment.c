#include "../parser.h"

#include <stdlib.h>

l_p_expression_assignment* l_parse_expression_assignment(l_scanner* stream)
{
	l_p_expression_assignment* expr = malloc(sizeof(l_p_expression_assignment));

	// name
	l_token name = l_scanner_next(stream);
	expr->name = name.content;

	// =
	l_scanner_skip(stream, TOKEN_EQUALS);

	// expression
	expr->expression = l_parse_expression(stream);

	return expr;
}
