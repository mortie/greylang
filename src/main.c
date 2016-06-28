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
		printf("%s: %s\n", l_token_type_string(token.type), token.content);
		token = l_scanner_next(scanner);
	}
}
