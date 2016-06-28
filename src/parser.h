#ifndef L_PARSER_H
#define L_PARSER_H

#include "token.h"
#include "scanner.h"

typedef struct l_p_expression_list
{
} l_p_expression_list;

l_p_expression_list* l_parse_expression_list(l_scanner* stream);

#endif
