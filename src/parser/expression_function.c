#include "../parser.h"

#include <stdlib.h>

l_p_expression_function* l_parse_expression_function(l_scanner* stream)
{
	l_p_expression_function* expr = malloc(sizeof(l_p_expression_function));

	// {
	l_scanner_skip(stream, TOKEN_OPENBRACE);

	// optional: (argument list);
	l_token t = l_scanner_peek(stream);
	if (t.type == TOKEN_OPENPAREN)
	{
		// (
		l_scanner_skip(stream, TOKEN_OPENPAREN);

		// argument list
		expr->arg_definition = l_parse_arg_definition(stream);

		// );
		l_scanner_skip(stream, TOKEN_CLOSEPAREN);
		l_scanner_skip(stream, TOKEN_SEMICOLON);
	}
	else
	{
		expr->arg_definition = NULL;
	}

	// expression list
	expr->expr_list = l_parse_expression_list(stream);

	// }
	l_scanner_skip(stream, TOKEN_CLOSEBRACE);

	return expr;
}

void l_pretty_expression_function(
		l_p_expression_function* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);

	fprintf(file, "%s{\n", tabs);

	if (expr->arg_definition != NULL)
	{
		fprintf(file, "%s\t(", tabs);
		l_pretty_arg_definition(expr->arg_definition, 0, file);
		fprintf(file, ");\n");
	}

	l_pretty_expression_list(expr->expr_list, depth + 1, file);

	fprintf(file, "%s}", tabs);
}
