#include "../parser.h"

#include <stdlib.h>

l_p_expr_object_literal* l_parse_expr_object_literal(l_scanner* stream)
{
	l_p_expr_object_literal* expr = malloc(sizeof(l_p_expr_object_literal));

	int allocd = 8;
	expr->names = malloc(sizeof(char*) * allocd);
	expr->exprs = malloc(sizeof(l_p_expr*) * allocd);
	expr->exprc = 0;

	// #{
	l_scanner_skip(stream, TOKEN_HASHBRACE, "object literal");

	l_token t = l_scanner_peek(stream);
	while (t.type != TOKEN_CLOSEBRACE)
	{
		// name
		l_token name = l_scanner_expect(stream, TOKEN_NAME, "object literal");

		// :
		l_scanner_skip(stream, TOKEN_COLON, "object literal");

		// value
		l_p_expr* val = l_parse_expr(stream);

		// ,
		l_token comma = l_scanner_peek(stream);
		if (comma.type == TOKEN_COMMA)
			l_scanner_skip(stream, TOKEN_COMMA, "object literal");

		// add expression
		expr->exprc += 1;
		if (allocd < expr->exprc)
		{
			allocd *= 2;
			expr->names = realloc(expr->names, sizeof(char*) * allocd);
			expr->exprs = realloc(expr->names, sizeof(l_p_expr*) * allocd);
		}
		expr->names[expr->exprc - 1] = name.content;
		expr->exprs[expr->exprc - 1] = val;

		t = l_scanner_peek(stream);
	}

	// }
	l_scanner_skip(stream, TOKEN_CLOSEBRACE, "object literal");

	return expr;
}

void l_pretty_expr_object_literal(
		l_p_expr_object_literal* expr,
		int depth,
		FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);

	fprintf(file, "#{\n");
	for (int i = 0; i < expr->exprc; ++i)
	{
		fprintf(file, "%s\t%s: ", tabs, expr->names[i]);
		l_pretty_expr(expr->exprs[i], depth + 1, file);
		if (i != expr->exprc - 1)
			fprintf(file, ",");
		fprintf(file, "\n");
	}
	fprintf(file, "%s}", tabs);
}
