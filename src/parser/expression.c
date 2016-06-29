#include "../parser.h"

#include <stdlib.h>

l_p_expression* l_parse_expression(l_scanner* stream)
{
	l_p_expression* expr = malloc(sizeof(l_p_expression));

	l_token t = l_scanner_peek(stream);

	/*
	 * Empty
	 */
	if (t.type == TOKEN_PERIOD)
	{
		expr->expression.empty = NULL;
		expr->type = EXPRESSION_EMPTY;
	}
	else
	{
		t = l_scanner_peek(stream);
		l_token next = l_scanner_peek2(stream);

		/*
		 * Assignment
		 */
		if (t.type == TOKEN_NAME && next.type == TOKEN_EQUALS)
		{
			expr->expression.assignment = l_parse_expression_assignment(stream);
			expr->type = EXPRESSION_ASSIGNMENT;
		}

		/*
		 * Func Call
		 */
		else if (t.type == TOKEN_NAME && next.type == TOKEN_OPENPAREN)
		{
			expr->expression.func_call = l_parse_expression_func_call(stream);
			expr->type = EXPRESSION_FUNC_CALL;
		}

		/*
		 * Function
		 */
		else if (t.type == TOKEN_OPENBRACE)
		{
			expr->expression.function = l_parse_expression_function(stream);
			expr->type = EXPRESSION_FUNCTION;
		}

		/*
		 * I dunno
		 */
		else
		{
			l_scanner_unexpecteda(NULL, 0, t);
		}
	}
}
