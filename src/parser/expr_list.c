#include "../parser.h"

#include <stdlib.h>

l_p_expr_list* l_parse_expr_list(l_scanner* stream)
{
	int alloced = 16;
	l_p_expr_list* list = malloc(sizeof(l_p_expr_list));
	list->expressions = malloc(alloced * sizeof(l_p_expr));
	list->expressionc = 0;

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

		// Skip semicolon if it exists
		l_token next = l_scanner_peek(stream);
		l_token n = next;
		if (next.type == TOKEN_SEMICOLON)
		{
			l_scanner_skip(stream, TOKEN_SEMICOLON, "expression list");
			next = l_scanner_peek(stream);
		}

		// Return if there's a none or a close brace
		if (next.type == TOKEN_CLOSEBRACE || next.type == TOKEN_NONE)
			return list;

		// No semicolon and no close brace or none isn't accepted
		if (n.type != TOKEN_SEMICOLON)
		{
			l_token_type expected[] = {
				TOKEN_SEMICOLON,
				TOKEN_CLOSEBRACE,
				TOKEN_NONE
			};
			l_scanner_unexpecteda(expected, 3, next, "expression list");
		}
	}
}

void l_pretty_expr_list(
		l_p_expr_list* list,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);

	for (int i = 0; i < list->expressionc; ++i)
	{
		fprintf(file, "%s", tabs);
		l_pretty_expr(list->expressions[i], depth, file);
		if (i == list->expressionc - 1)
		{
			fprintf(file, "\n");
		}
		else
		{
			fprintf(file, ";\n");
		}
	}
}
