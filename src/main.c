#include "scanner.h"
#include "token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		puts("Expected exactly one argument.");
		exit(1);
	}

	char* str = argv[1];
	l_token* tokens = l_scan(str, strlen(str));
	if (tokens == NULL)
		return 1;

	int i = 0;
	l_token token = tokens[0];
	while (token.tokentype != TOKEN_NONE)
	{
		printf("%s: ", l_token_type_string(token.tokentype));
		for (int j = 0; j < token.contentlen; ++j)
		{
			putchar(*(token.content + j));
		}
		putchar('\n');
		token = tokens[++i];
	}
}
