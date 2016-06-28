#include "scanner.h"
#include "token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* str = argv[1];
	l_scanner* scanner = l_scanner_create(stdin);

	l_token token = l_scanner_next(scanner);
	while (token.type != TOKEN_NONE)
	{
		char* typestring = l_token_type_string(token.type);
		int wslen = 20 - strlen(typestring);
		char* ws = malloc(wslen + 1);
		memset(ws, ' ', wslen);
		ws[wslen] = '\0';

		printf("%s:%s%s\n", typestring, ws, token.content);
		token = l_scanner_next(scanner);
	}
}
