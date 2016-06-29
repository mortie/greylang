#include "../parser.h"

#include <stdlib.h>

l_p_expression_func_call* l_parse_expression_func_call(l_scanner* stream)
{
	l_p_expression_func_call* expr = malloc(sizeof(l_p_expression_func_call));

	// name
	l_token name = l_scanner_next(stream);
	expr->name = name.content;

	// (
	l_scanner_skip(stream, TOKEN_OPENBRACE);

	// expression list
	expr->expr_list = l_parse_expression_list(stream);

	// )
	l_scanner_skip(stream, TOKEN_CLOSEBRACE);

	return expr;
}
