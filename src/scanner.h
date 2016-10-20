#ifndef L_SCANNER_H
#define L_SCANNER_H

#include "token.h"

#include <stdio.h>

typedef enum l_scanner_type
{
	SCANNER_TYPE_FILE,
	SCANNER_TYPE_STR
} l_scanner_type;

typedef struct l_scanner
{
	union
	{
		FILE* f;
		char* str;
	} input;

	int character;
	int line;
	int linechar;
	char curr;
	char next;
	l_token nexttoken;
	l_token nexttoken2;
	l_token nexttoken3;

	l_scanner_type type;
} l_scanner;

l_scanner* l_scanner_create(FILE* f);
l_scanner* l_scanner_create_str(char* str);
l_token l_scanner_next(l_scanner* scanner);
l_token l_scanner_peek(l_scanner* scanner);
l_token l_scanner_peek2(l_scanner* scanner);
l_token l_scanner_peek3(l_scanner* scanner);
void l_scanner_unexpecteda(l_token_type* expected, int len, l_token token, char* section);
void l_scanner_unexpected(l_token_type expected, l_token token, char* section);
void l_scanner_skip(l_scanner* scanner, l_token_type type, char* section);
l_token l_scanner_expect(l_scanner* scanner, l_token_type type, char* section);

#endif
