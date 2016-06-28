#include "scanner.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#define STRAPPEND(name, a, len, c) \
	do { \
		if (len == 0) { name = malloc(32); a = 32; } \
		len += 1; \
		if (len > a) \
		{ \
			a *= 2; \
			name = realloc(name, a); \
		} \
		name[len - 1] = c; \
		name[len] = '\0'; \
	} while(0)

static char nextchar(l_scanner* scanner)
{
	if (scanner->curr == EOF || scanner->next == EOF)
	{
		scanner->curr = scanner->next = EOF;
		return EOF;
	}

	char c = scanner->next;
	scanner->curr = scanner->next;
	scanner->next = fgetc(scanner->f);

	if (c == '\n') scanner->line += 1;

	return c;
}

static l_token gettoken(l_scanner* scanner)
{
	char c = scanner->curr;
	char next = scanner->next;
	int line = scanner->line;
	FILE* f = scanner->f;

	char* content = "";
	int contentlen = 0;
	int contenta = 0;

#define SETTOKEN(ttype, c) \
	do {\
		token.type = ttype; \
		token.content = c; \
		token.line = line; \
	} while(0)

	l_token token;
	SETTOKEN(TOKEN_NONE, "");

	/*
	 * String Literal
	 */
	if (c == '"')
	{
		int closed = 1;
		char cc = nextchar(scanner);
		while (cc != '"')
		{
			if (cc == EOF)
			{
				closed = 0;
				break;
			}
			STRAPPEND(content, contenta, contentlen, cc);
			cc = nextchar(scanner);
		}
		nextchar(scanner);

		if (closed)
		{
			SETTOKEN(TOKEN_STRING_LITERAL, content);
		}
		else
		{
			SETTOKEN(TOKEN_ERROR, "String literal not closed");
		}
	}

	/*
	 * Char Literal
	 */
	else if (c == '\'')
	{
		char* str = malloc(1);
		str[0] = nextchar(scanner);
		char end = nextchar(scanner);
		nextchar(scanner);

		if (str[0] != EOF && end == '\'')
		{
			SETTOKEN(TOKEN_CHAR_LITERAL, str);
		}
		else
		{
			SETTOKEN(TOKEN_ERROR, "Char literal not closed");
		}
	}

	/*
	 * Number Literal
	 */
	else if (c >= '0' && c <= '9')
	{
		int period = 0;
		char cc = c;
		while (
				cc != EOF && (
				(cc >= '0' && cc <= '9') ||
				(cc == '.' && !period)))
		{
			if (cc == '.') period = 1;
			STRAPPEND(content, contenta, contentlen, cc);
			cc = nextchar(scanner);
		}

		SETTOKEN(TOKEN_NUM_LITERAL, content);
	}

	/*
	 * Comma
	 */
	else if (c == ',')
	{
		SETTOKEN(TOKEN_COMMA, ",");
		nextchar(scanner);
	}

	/*
	 * Semicolon
	 */
	else if (c == ';')
	{
		SETTOKEN(TOKEN_SEMICOLON, ";");
		nextchar(scanner);
	}

	/*
	 * Equals
	 */
	else if (c == '=')
	{
		SETTOKEN(TOKEN_EQUALS, "=");
		nextchar(scanner);
	}

	/*
	 * Open Paren
	 */
	else if (c == '(')
	{
		SETTOKEN(TOKEN_OPENPAREN, "(");
		nextchar(scanner);
	}

	/*
	 * Close Paren
	 */
	else if (c == ')')
	{
		SETTOKEN(TOKEN_CLOSEPAREN, ")");
		nextchar(scanner);
	}

	/*
	 * Open Bracket
	 */
	else if (c == '[')
	{
		SETTOKEN(TOKEN_OPENBRACKET, "[");
		nextchar(scanner);
	}

	/*
	 * Close Bracket
	 */
	else if (c == ']')
	{
		SETTOKEN(TOKEN_CLOSEBRACKET, "]");
		nextchar(scanner);
	}

	/*
	 * Open Brace
	 */
	else if (c == '{')
	{
		SETTOKEN(TOKEN_OPENBRACE, "{");
		nextchar(scanner);
	}

	/*
	 * Close Brace
	 */
	else if (c == '}')
	{
		SETTOKEN(TOKEN_CLOSEBRACE, "}");
		nextchar(scanner);
	}

	/*
	 * Skip multi line comment
	 */
	else if (c == '/' && next == '*')
	{
		char prev = c;
		char cc = nextchar(scanner);
		while (cc != EOF && prev != '*' && cc != '/')
		{
			prev = cc;
			cc = nextchar(scanner);
		}
		token.type = TOKEN_IGNORED;
		nextchar(scanner);
	}

	/*
	 * Skip single line comment
	 */
	else if (c == '/' && next == '*')
	{
		char cc = nextchar(scanner);
		while (cc != EOF && cc != '\n') cc = nextchar(scanner);
		token.type = TOKEN_IGNORED;
		nextchar(scanner);
	}

	/*
	 * Skip Whitespace
	 */
	else if (isspace(c))
	{
		char cc = nextchar(scanner);
		while (cc != EOF && isspace(cc))
		{
			cc = nextchar(scanner);
		}
		token.type = TOKEN_IGNORED;
	}

	/*
	 * None
	 */
	else if (c == EOF)
	{
		SETTOKEN(TOKEN_NONE, "");
	}

	/*
	 * Name
	 */
	else
	{
		char cc = c;
		while (cc != EOF && !isspace(cc) &&
				(cc != '(' && cc != ')') &&
				(cc != '[' && cc != ']') &&
				(cc != ';' && cc != ','))
		{
			STRAPPEND(content, contentlen, contenta, cc);
			cc = nextchar(scanner);
		}

		SETTOKEN(TOKEN_NAME, content);
	}

	/*
	 * Die on error
	 */
	if (token.type == TOKEN_ERROR)
	{
		fprintf(stderr, "%s at line %i\n", token.content, token.line);
		SETTOKEN(TOKEN_NONE, "");
	}

	return token;
}

l_scanner* l_scanner_create(FILE* f)
{
	l_scanner* scanner = malloc(sizeof(l_scanner));
	scanner->f = f;
	scanner->line = 1;
	scanner->curr = '\0';
	scanner->next = '\0';
	nextchar(scanner);
	nextchar(scanner);

	return scanner;
}

l_token l_scanner_next(l_scanner* scanner)
{
	l_token token;

	do
	{
		token = gettoken(scanner);
	} while (token.type == TOKEN_IGNORED);

	return token;
}

void l_scanner_skip(l_scanner* scanner, l_token_type type)
{
	l_token token = l_scanner_next(scanner);
	if (token.type != type)
	{
		fprintf(stderr,
			"Expected %s, got %s, on line %i\n",
			l_token_type_string(type),
			l_token_type_string(token.type),
			token.line);
	}
}
