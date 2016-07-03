#include "../parser.h"

#include <stdlib.h>

l_p_comma_expr_list* l_parse_comma_expr_list(l_scanner* stream)
{
	int alloced = 16;
	l_p_comma_expr_list* list = malloc(sizeof(l_p_comma_expr_list));
	list->expressions = malloc(alloced * sizeof(l_p_expr));
	list->expressionc = 0;

	if (l_scanner_peek(stream).type == TOKEN_CLOSEPAREN)
		return list;

	while (1)
	{
		l_p_expr* expr = l_parse_expr(stream);

		/*
		 * Add expression
		 */
		list->expressionc += 1;
		if (list->expressionc > alloced)
		{
			alloced *= 2;
			list->expressions = realloc(
				list->expressions,
				alloced * sizeof(l_p_expr));
		}
		list->expressions[list->expressionc - 1] = expr;

		/*
		 * Continue (comma), stop (close paren or close bracket), or error
		 */
		l_token next = l_scanner_peek(stream);
		if (next.type == TOKEN_COMMA)
		{
			l_scanner_skip(stream, TOKEN_COMMA, "arg expression list");
		}
		else if (next.type == TOKEN_CLOSEPAREN || next.type == TOKEN_CLOSEBRACKET)
		{
			return list;
		}
		else
		{
			l_token_type expected[] = {
				TOKEN_COMMA,
				TOKEN_CLOSEPAREN,
				TOKEN_CLOSEBRACKET
			};
			l_scanner_unexpecteda(expected, 2, next, "arg expression list");
		}
	}
}

void l_pretty_comma_expr_list(
		l_p_comma_expr_list* list,
		int depth,
		FILE* file)
{
	for (int i = 0; i < list->expressionc; ++i)
	{
		l_pretty_expr(list->expressions[i], depth, file);
		if (i == list->expressionc - 1)
		{
			fprintf(file, "\n");
		}
		else
		{
			fprintf(file, ",\n");
		}
	}
}
