#include "../parser.h"

#include <stdlib.h>

l_p_arg_list* l_parse_arg_list(l_scanner* stream)
{
	int namea = 4;
	l_p_arg_list* list = malloc(sizeof(l_p_arg_list));
	list->names = malloc(namea);
	list->names[0] = '\0';
	list->namec = 0;

	// Empty argument list
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

void l_pretty_arg_list(
		l_p_arg_list* arg_list,
		int depth,
		FILE* file)
{
	for (int i = 0; i < arg_list->namec; ++i)
	{
		fprintf(file, arg_list->names[i]);
		if (i + 1 < arg_list->namec)
		{
			fprintf(file, ", ");
		}
	}
}
