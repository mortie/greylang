#include "../parser.h"

#include <stdlib.h>

l_p_expression_list* l_parse_expression_list(l_scanner* stream)
{
	int alloced = 16;
	l_p_expression_list* list = malloc(sizeof(l_p_expression_list));
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
		 * Continue (semicolon), stop (period), or error
		 */
		l_token next = l_scanner_next(stream);
		if (next.type == TOKEN_SEMICOLON)
		{
			// Continue parsing expressions
		}
		else if (next.type == TOKEN_PERIOD)
		{
			return list;
		}
		else
		{
			l_token_type expected[] = {
				TOKEN_SEMICOLON,
				TOKEN_PERIOD
			};
			l_scanner_unexpecteda(expected, 2, next, "expression list");
		}
	}
}

void l_pretty_expression_list(
		l_p_expression_list* list,
		int depth,
		FILE* file)
{
	for (int i = 0; i < list->expressionc; ++i)
	{
		l_pretty_expression(list->expressions[i], depth, file);
		if (i == list->expressionc - 1)
		{
			fprintf(file, ".\n");
		}
		else
		{
			fprintf(file, ";\n");
		}
	}
}
