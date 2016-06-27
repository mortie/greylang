#include "parser.h"

#include <stdlib.h>
#include <stdio.h>

l_parser_stream* l_parser_stream_create(l_token* tokens)
{
	l_parser_stream* stream = malloc(sizeof(l_parser_stream));

	stream->tokens = tokens;
	stream->cur = 0;
}

l_token l_parser_stream_next(l_parser_stream* stream)
{
	return stream->tokens[stream->cur++];
}

void l_parser_stream_skip(l_parser_stream* stream, l_token_type type)
{
	l_token token = l_parser_stream_next(stream);

	if (token.tokentype != type)
	{
		char* expected = l_token_type_string(token.tokentype);
		char* got = l_token_type_string(type);
		fprintf(
			stderr,
			"Expected %s, got %s on line %i", expected, got, token.line);
	}
}
