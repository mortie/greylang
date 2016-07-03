#include "../parser.h"

#include <stdlib.h>

l_p_expr* l_parse_expr(l_scanner* stream)
{
	l_p_expr* expr = malloc(sizeof(l_p_expr));

	l_token t = l_scanner_peek(stream);

	/*
	 * Empty
	 */
	if (t.type == TOKEN_CLOSEPAREN ||
			t.type == TOKEN_CLOSEBRACE ||
			t.type == TOKEN_NONE)
	{
		expr->expression.empty = NULL;
		expr->type = EXPR_EMPTY;
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
				l_parse_expr_assignment(stream);
			expr->type = EXPR_ASSIGNMENT;
		}

		/*
		 * Func Call
		 */
		else if (t.type == TOKEN_NAME && next.type == TOKEN_OPENPAREN)
		{
			expr->expression.func_call =
				l_parse_expr_func_call(stream);
			expr->type = EXPR_FUNC_CALL;
		}

		/*
		 * Function
		 */
		else if (t.type == TOKEN_OPENBRACE)
		{
			expr->expression.function =
				l_parse_expr_function(stream);
			expr->type = EXPR_FUNCTION;
		}

		/*
		 * Array Literal
		 */
		else if (t.type == TOKEN_OPENBRACKET)
		{
			expr->expression.array_literal =
				l_parse_expr_array_literal(stream);
			expr->type = EXPR_ARRAY_LITERAL;
		}

		/*
		 * String Literal
		 */
		else if (t.type == TOKEN_STRING_LITERAL)
		{
			expr->expression.string_literal =
				l_parse_expr_string_literal(stream);
			expr->type = EXPR_STRING_LITERAL;
		}

		/*
		 * Char Literal
		 */
		else if (t.type == TOKEN_CHAR_LITERAL)
		{
			expr->expression.string_literal =
				l_parse_expr_string_literal(stream);
			expr->type = EXPR_STRING_LITERAL;
		}

		/*
		 * Number Literal
		 */
		else if (t.type == TOKEN_NUM_LITERAL)
		{
			expr->expression.num_literal =
				l_parse_expr_num_literal(stream);
			expr->type = EXPR_NUM_LITERAL;
		}

		/*
		 * Variable
		 */
		else if (t.type == TOKEN_NAME)
		{
			expr->expression.variable =
				l_parse_expr_variable(stream);
			expr->type = EXPR_VARIABLE;
		}

		/*
		 * I dunno
		 */
		else
		{
			l_scanner_unexpecteda(NULL, 0, t, "expression");
		}
	}

	return expr;
}

void l_pretty_expr(
		l_p_expr* expr,
		int depth,
		FILE* file)
{
	switch (expr->type)
	{
	case EXPR_EMPTY:
		l_pretty_expr_empty(
			depth, file);
		break;
	case EXPR_ASSIGNMENT:
		l_pretty_expr_assignment(
			expr->expression.assignment,
			depth, file);
		break;
	case EXPR_FUNC_CALL:
		l_pretty_expr_func_call(
			expr->expression.func_call,
			depth, file);
		break;
	case EXPR_FUNCTION:
		l_pretty_expr_function(
			expr->expression.function,
			depth, file);
		break;
	case EXPR_ARRAY_LITERAL:
		l_pretty_expr_array_literal(
			expr->expression.array_literal,
			depth, file);
	case EXPR_NUM_LITERAL:
		l_pretty_expr_num_literal(
			expr->expression.num_literal,
			depth, file);
		break;
	case EXPR_STRING_LITERAL:
		l_pretty_expr_string_literal(
			expr->expression.string_literal,
			depth, file);
		break;
	case EXPR_CHAR_LITERAL:
		l_pretty_expr_char_literal(
			expr->expression.char_literal,
			depth, file);
		break;
	case EXPR_VARIABLE:
		l_pretty_expr_variable(
			expr->expression.variable,
			depth, file);
		break;
	}
}
