#include "../parser.h"

#include <stdlib.h>

l_p_arg_definition* l_parse_arg_definition(l_scanner* stream)
{
	int namea = 4;
	l_p_arg_definition* list = malloc(sizeof(l_p_arg_definition));
	list->names = malloc(namea);
	list->names[0] = '\0';
	list->namec = 0;

	// Empty argument definition
	l_token t = l_scanner_peek(stream);
	if (t.type == TOKEN_CLOSEPAREN)
		return list;

	while (1)
	{
		// name
		l_token name = l_scanner_next(stream);
		if (name.type != TOKEN_NAME)
			l_scanner_unexpected(TOKEN_NAME, name);
		list->namec += 1;
		if (namea < list->namec)
		{
			namea *= 2;
			list->names = realloc(list->names, namea);
		}
		list->names[list->namec - 1] = name.content;

		// , or )
		l_token next = l_scanner_peek(stream);
		if (next.type == TOKEN_COMMA)
		{
			l_scanner_skip(stream, TOKEN_COMMA);
		}
		else if (next.type == TOKEN_CLOSEPAREN)
		{
			break;
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

	return list;
}

void l_pretty_arg_definition(
		l_p_arg_definition* arg_definition,
		int depth,
		FILE* file)
{
	for (int i = 0; i < arg_definition->namec; ++i)
	{
		fprintf(file, arg_definition->names[i]);
		if (i + 1 < arg_definition->namec)
		{
			fprintf(file, ", ");
		}
	}
}
