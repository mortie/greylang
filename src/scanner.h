#ifndef L_SCANNER_H
#define L_SCANNER_H

#include "token.h"

#include <stdio.h>

typedef struct l_scanner
{
	FILE* f;
	int line;
	char curr;
	char next;
} l_scanner;

l_scanner* l_scanner_create(FILE* f);
l_token l_scanner_next(l_scanner* scanner);

#endif
