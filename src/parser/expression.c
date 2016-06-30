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
			expr->expression.assignment =
				l_parse_expression_assignment(stream);
			expr->type = EXPRESSION_ASSIGNMENT;
		}

		/*
		 * Func Call
		 */
		else if (t.type == TOKEN_NAME && next.type == TOKEN_OPENPAREN)
		{
			expr->expression.func_call =
				l_parse_expression_func_call(stream);
			expr->type = EXPRESSION_FUNC_CALL;
		}

		/*
		 * Function
		 */
		else if (t.type == TOKEN_OPENBRACE)
		{
			expr->expression.function =
				l_parse_expression_function(stream);
			expr->type = EXPRESSION_FUNCTION;
		}

		/*
		 * String Literal
		 */
		else if (t.type == TOKEN_STRING_LITERAL)
		{
			expr->expression.string_literal =
				l_parse_expression_string_literal(stream);
			expr->type = EXPRESSION_STRING_LITERAL;
		}

		/*
		 * Char Literal
		 */
		else if (t.type == TOKEN_CHAR_LITERAL)
		{
			expr->expression.string_literal =
				l_parse_expression_string_literal(stream);
			expr->type = EXPRESSION_STRING_LITERAL;
		}

		/*
		 * Number Literal
		 */
		else if (t.type == TOKEN_NUM_LITERAL)
		{
			expr->expression.num_literal =
				l_parse_expression_num_literal(stream);
			expr->type = EXPRESSION_NUM_LITERAL;
		}

		/*
		 * Variable
		 */
		else if (t.type == TOKEN_NAME)
		{
			expr->expression.variable =
				l_parse_expression_variable(stream);
			expr->type = EXPRESSION_VARIABLE;
		}

		/*
		 * I dunno
		 */
		else
		{
			l_scanner_unexpecteda(NULL, 0, t);
		}
	}

	return expr;
}

void l_pretty_expression(
		l_p_expression* expr,
		int depth,
		FILE* file)
{
	switch (expr->type)
	{
	case EXPRESSION_EMPTY:
		l_pretty_expression_empty(
			depth, file);
		break;
	case EXPRESSION_ASSIGNMENT:
		l_pretty_expression_assignment(
			expr->expression.assignment,
			depth, file);
		break;
	case EXPRESSION_FUNC_CALL:
		l_pretty_expression_func_call(
			expr->expression.func_call,
			depth, file);
		break;
	case EXPRESSION_FUNCTION:
		l_pretty_expression_function(
			expr->expression.function,
			depth, file);
		break;
	case EXPRESSION_NUM_LITERAL:
		l_pretty_expression_num_literal(
			expr->expression.num_literal,
			depth, file);
		break;
	case EXPRESSION_STRING_LITERAL:
		l_pretty_expression_string_literal(
			expr->expression.string_literal,
			depth, file);
		break;
	case EXPRESSION_CHAR_LITERAL:
		l_pretty_expression_char_literal(
			expr->expression.char_literal,
			depth, file);
		break;
	case EXPRESSION_VARIABLE:
		l_pretty_expression_variable(
			expr->expression.variable,
			depth, file);
		break;
	}
}
