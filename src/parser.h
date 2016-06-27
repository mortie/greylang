#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"

typedef struct l_parser_stream
{
	l_token* tokens;
	int cur;
} l_parser_stream;

l_parser_stream* l_parser_stream_create(l_token* tokens);
l_token l_parser_stream_next(l_parser_stream* stream);
void l_parser_stream_skip(l_parser_stream* stream, l_token_type type);


typedef struct l_p_expression_list
{
} l_p_expression_list;

l_p_expression_list* l_parse_expression_list(l_parser_stream* stream);

#endif
