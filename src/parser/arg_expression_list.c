#include "../parser.h"

#include <stdlib.h>

l_p_arg_expression_list* l_parse_arg_expression_list(l_scanner* stream)
{
	int alloced = 16;
	l_p_arg_expression_list* list = malloc(sizeof(l_p_arg_expression_list));
	list->expressions = malloc(alloced * sizeof(l_p_expression));
	list->expressionc = 0;

	while (1)
	{
		l_p_expression* expr = l_parse_expression(stream);

		/*
		 * Add expression
		 */
		list->expressionc += 1;
		if (list->expressionc > alloced)
		{
			alloced *= 2;
			list->expressions = realloc(
				list->expressions,
				alloced * sizeof(l_p_expression));
		}
		list->expressions[list->expressionc - 1] = expr;

		/*
		 * Continue (comma), stop (close paren), or error
		 */
		l_token next = l_scanner_peek(stream);
		if (next.type == TOKEN_COMMA)
		{
			l_scanner_skip(stream, TOKEN_COMMA);
		}
		else if (next.type == TOKEN_CLOSEPAREN)
		{
			return list;
		}
		else
		{
			l_token_type expected[] = {
				TOKEN_COMMA,
				TOKEN_CLOSEPAREN
			};
			l_scanner_unexpecteda(expected, 2, next);
		}
	}
}

void l_pretty_arg_expression_list(
		l_p_arg_expression_list* list,
		int depth,
		FILE* file)
{
	for (int i = 0; i < list->expressionc; ++i)
	{
		l_pretty_expression(list->expressions[i], depth, file);
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
