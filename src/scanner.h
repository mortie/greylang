#ifndef L_SCANNER_H
#define L_SCANNER_H

#include "token.h"

#include <stdio.h>

typedef struct l_scanner
{
	FILE* f;
	int character;
	int line;
	char curr;
	char next;
	l_token nexttoken;
	l_token nexttoken2;
} l_scanner;

l_scanner* l_scanner_create(FILE* f);
l_token l_scanner_next(l_scanner* scanner);
l_token l_scanner_peek(l_scanner* scanner);
l_token l_scanner_peek2(l_scanner* scanner);
void l_scanner_unexpecteda(l_token_type* expected, int len, l_token token);
void l_scanner_unexpected(l_token_type expected, l_token token);
void l_scanner_skip(l_scanner* scanner, l_token_type type);

#endif
