#include "../parser.h"

#include <stdlib.h>

l_p_expression_function* l_parse_expression_function(l_scanner* stream)
{
	l_p_expression_function* expr = malloc(sizeof(l_p_expression_function));

	// {
	l_scanner_skip(stream, TOKEN_OPENBRACE);

	// optional: (argument list)
	l_token t = l_scanner_peek(stream);
	if (t.type == TOKEN_OPENPAREN)
	{
		// (
		l_scanner_skip(stream, TOKEN_OPENPAREN);

		// argument list
		expr->arg_list = l_parse_arg_list(stream);

		// )
		l_scanner_skip(stream, TOKEN_CLOSEPAREN);
	}
	else
	{
		expr->arg_list = NULL;
	}

	// expression list
	expr->expr_list = l_parse_expression_list(stream);

	// }
	l_scanner_skip(stream, TOKEN_CLOSEBRACE);
}
