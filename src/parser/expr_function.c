#include "../parser.h"

#include <stdlib.h>

l_p_expr_function *l_parse_expr_function(l_scanner *stream)
{
	l_p_expr_function *expr = malloc(sizeof(*expr));

	// {
	l_scanner_skip(stream, TOKEN_OPENBRACE, "function");

	// optional: (argument list);
	l_token t = l_scanner_peek(stream);
	if (t.type == TOKEN_OPENPAREN)
	{
		// (
		l_scanner_skip(stream, TOKEN_OPENPAREN, "function");

		// argument list
		expr->arg_definition = l_parse_arg_definition(stream);

		// )
		l_scanner_skip(stream, TOKEN_CLOSEPAREN, "function");
	}
	else
	{
		expr->arg_definition = NULL;
	}

	// expression list
	expr->expr_list = l_parse_expr_list(stream);

	// }
	l_scanner_skip(stream, TOKEN_CLOSEBRACE, "function");

	return expr;
}

void l_pretty_expr_function(
		l_p_expr_function *expr,
		int depth,
		FILE *file)
{
	char *tabs;
	P_TABS(depth, tabs);

	fprintf(file, "{");

	if (expr->arg_definition != NULL)
	{
		fprintf(file, "(");
		l_pretty_arg_definition(expr->arg_definition, 0, file);
		fprintf(file, ")");
	}
	fprintf(file, "\n");

	l_pretty_expr_list(expr->expr_list, depth + 1, file);

	fprintf(file, "%s}", tabs);
}
