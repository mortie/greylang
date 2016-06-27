#include "scanner.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

l_token* l_scan(char* input, int len)
{
	char c;
	char prev = '\0';
	int line = 1;

	int tokena = 64;
	int tokenc = 0;
	l_token* tokenv = malloc(sizeof(l_token) * tokena);
	memset(tokenv, 0, sizeof(l_token) * tokena);

#define SETTOKEN(type, c, cl) \
		token.tokentype = type; \
		token.content = c; \
		token.contentlen = cl; \
		token.line = line; \
		tokendefined = 1

	int i = 0;
	while (i < len)
	{
		c = input[i];

		l_token token;
		int tokendefined = 0;

		/*
		 * String Literal
		 */
		if (c == '"')
		{
			int closed = 1;
			int start = i;
			i += 1;
			while (input[i] != '"')
			{
				if (input[i] == '\n') line += 1;
				i += 1;
				if (i < len)
				{
					closed = 0;
					break;
				}
			}
			i += 1;

			if (closed)
			{
				SETTOKEN(TOKEN_STRING_LITERAL, input + start + 1, i - start - 2);
			}
			else
			{
				SETTOKEN(TOKEN_ERROR, "String literal not closed", 0);
			}
		}

		/*
		 * Char Literal
		 */
		else if (c == '\'')
		{
			if (i + 2 < len && input[i + 2] == '\'')
			{
				SETTOKEN(TOKEN_CHAR_LITERAL, input + i + 1, 1);
			}
			else
			{
				SETTOKEN(TOKEN_ERROR, "Char literal not closed", 0);
			}
			i += 3;
		}

		/*
		 * Number Literal
		 */
		else if (c >= '0' && c <= '9')
		{
			int start = i;
			int period = 0;
			char cc = c;
			while (
					i < len && (
					(cc >= '0' && cc <= '9') ||
					(cc == '.' && !period)))
			{
				i += 1;
				if (cc == '.') period = 1;
				cc = input[i];
			}
			prev = cc;

			SETTOKEN(TOKEN_NUM_LITERAL, input + start, i - start);
		}

		/*
		 * Semicolon
		 */
		else if (c == ';')
		{
			SETTOKEN(TOKEN_SEMICOLON, ";", 1);
			i += 1;
		}

		/*
		 * Comma
		 */
		else if (c == ',')
		{
			SETTOKEN(TOKEN_COMMA, ",", 1);
			i += 1;
		}

		/*
		 * Open Paren
		 */
		else if (c == '(')
		{
			SETTOKEN(TOKEN_OPENPAREN, "(", 1);
			i += 1;
		}

		/*
		 * Close Paren
		 */
		else if (c == ')')
		{
			SETTOKEN(TOKEN_CLOSEPAREN, ")", 1);
			i += 1;
		}

		/*
		 * Open Bracket
		 */
		else if (c == '[')
		{
			SETTOKEN(TOKEN_OPENBRACKET, "[", 1);
			i += 1;
		}

		/*
		 * Close Bracket
		 */
		else if (c == ']')
		{
			SETTOKEN(TOKEN_CLOSEBRACKET, "]", 1);
			i += 1;
		}

		/*
		 * Open Brace
		 */
		else if (c == '{')
		{
			SETTOKEN(TOKEN_OPENBRACE, "{", 1);
			i += 1;
		}

		/*
		 * Close Brace
		 */
		else if (c == '}')
		{
			SETTOKEN(TOKEN_CLOSEBRACE, "}", 1);
			i += 1;
		}

		/*
		 * Skip multi line comment
		 */
		else if (prev == '/' && c == '*')
		{
			while (i < len && input[i-1] == '*' && input[i] != '/')
			{
				if (input[i] == '\n') i += 1;
				i += 1;
			}
		}

		/*
		 * Skip single line comment
		 */
		else if (prev == '/' && c == '*')
		{
			while (i < len && input[i] != '\n') i += 1;
			line += 1;
		}

		/*
		 * Skip Whitespace
		 */
		else if (isspace(c))
		{
			while (i < len && isspace(input[i]))
			{
				if (input[i] == '\n') line += 1;
				i += 1;
			}
		}

		/*
		 * Name
		 */
		else
		{
			int start = i;
			char cc = input[i];
			while (i < len && !isspace(cc) &&
					(cc != '(' && cc != ')') &&
					(cc != '[' && cc != ']') &&
					(cc != ';' && cc != ','))
			{
				i += 1;
				cc = input[i];
			}
			prev = cc;

			SETTOKEN(TOKEN_NAME, input + start, i - start);
		}

		prev = input[i];

		/*
		 * Add token if it's defined
		 */
		if (tokendefined)
		{

			/*
			 * Die on error
			 */
			if (token.tokentype == TOKEN_ERROR)
			{
				fprintf(stderr, "%s at line %i\n", token.content, token.line);

				return NULL;
			}

			tokenc += 1;

			if (tokenc > tokena)
			{
				tokena *= 2;
				tokenv = realloc(tokenv, sizeof(l_token) * tokena);
				memset(
					tokenv + tokenc - 2,
					0,
					sizeof(l_token) * (tokena - tokenc - 1));
			}

			memcpy(tokenv + tokenc - 1, &token, sizeof(l_token));
		}
	}

#undef SETTOKEN

	return tokenv;
}
