#include "../scanner.h"

#include <stdlib.h>
#include <stdio.h>
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

	switch (scanner->type)
	{
	case SCANNER_TYPE_FILE:
		scanner->next = fgetc(scanner->input.f);
		break;
	case SCANNER_TYPE_STR:
		scanner->next = scanner->input.str[scanner->character + 1];
		if (scanner->next == '\0')
			scanner->next = EOF;
		break;
	}

	scanner->character += 1;

	if (c == '\n')
	{
		scanner->line += 1;
		scanner->character = 0;
	}

	return c;
}

static l_token gettoken(l_scanner* scanner)
{
	char c = scanner->curr;
	char next = scanner->next;
	int character = scanner->character;
	int line = scanner->line;

	char* content = "";
	int contentlen = 0;
	int contenta = 0;

#define SETTOKEN(ttype, c) \
	do { \
		token.type = ttype; \
		token.content = c; \
		token.character = character; \
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
				(
					cc == '.' && !period &&
					scanner->next >= '0' && scanner->next <= '9')
				))
		{
			if (cc == '.') period = 1;
			STRAPPEND(content, contenta, contentlen, cc);
			cc = nextchar(scanner);
		}

		SETTOKEN(TOKEN_NUM_LITERAL, content);
	}

	/*
	 * Period
	 */
	else if (c == '.' && next != '.')
	{
		SETTOKEN(TOKEN_PERIOD, ",");
		nextchar(scanner);
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
	 * Colon
	 */
	else if (c == ':')
	{
		SETTOKEN(TOKEN_COLON, ":");
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
	else if (c == '=' && next != '=')
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
	 * Hash Brace
	 */
	else if (c == '#' && next == '{')
	{
		SETTOKEN(TOKEN_HASHBRACE, "#{");
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
		while (cc != EOF && !(prev == '*' && cc == '/'))
		{
			prev = cc;
			cc = nextchar(scanner);
		}
		nextchar(scanner);
		token.type = TOKEN_IGNORED;
	}

	/*
	 * Skip single line comment
	 */
	else if (
		(c == '/' && next == '/') ||
		(c == '#' && character == 1))
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
		fprintf(stderr,
			"line %i:%i: %s\n",
			token.line,
			token.character,
			token.content);
		SETTOKEN(TOKEN_NONE, "");
	}

	return token;
}

static l_token nexttoken(l_scanner* scanner)
{
	l_token token;
	do
	{
		token = gettoken(scanner);
	} while (token.type == TOKEN_IGNORED);
	return token;
}

static l_scanner* create()
{
	l_scanner* scanner = malloc(sizeof(l_scanner));
	scanner->character = -1;
	scanner->line = 1;
	scanner->curr = '\0';
	scanner->next = '\0';

	return scanner;
}

static void init(l_scanner* scanner)
{
	nextchar(scanner);
	nextchar(scanner);

	scanner->nexttoken = nexttoken(scanner);
	scanner->nexttoken2 = nexttoken(scanner);
}

l_scanner* l_scanner_create(FILE* f)
{
	l_scanner* scanner = create();
	scanner->input.f = f;
	scanner->type = SCANNER_TYPE_FILE;
	init(scanner);

	return scanner;
}

l_scanner* l_scanner_create_str(char* str)
{
	l_scanner* scanner = create();
	scanner->input.str = str;
	scanner->type = SCANNER_TYPE_STR;
	init(scanner);

	return scanner;
}

l_token l_scanner_next(l_scanner* scanner)
{
	l_token token = nexttoken(scanner);

	l_token next = scanner->nexttoken;
	scanner->nexttoken = scanner->nexttoken2;
	scanner->nexttoken2 = token;

	return next;
}

l_token l_scanner_peek(l_scanner* scanner)
{
	return scanner->nexttoken;
}

l_token l_scanner_peek2(l_scanner* scanner)
{
	return scanner->nexttoken2;
}

void l_scanner_unexpecteda(
		l_token_type* expected,
		int len,
		l_token token,
		char* section)
{
	if (len == 0)
	{
		fprintf(stderr,
			"line %i:%i (%s): Unexpected token %s\n",
			token.line,
			token.character,
			section,
			l_token_type_string(token.type));
		exit(1);
	}

	int totallen = (len * 4) - 4; // Account for ' or ' between the words
	for (int i = 0; i < len; ++i)
	{
		totallen += strlen(l_token_type_string(expected[i]));
	}

	char* str = malloc(totallen + 1);
	int i = 0;
	for (int j = 0; j < len; ++j)
	{
		char* s = l_token_type_string(expected[j]);
		int sl = strlen(s);
		memcpy(str + i, s, sl);
		i += sl;

		if (j + 1 < len)
		{
			memcpy(str + i, " or ", 4);
			i += 4;
		}
	}

	fprintf(stderr,
		"line %i:%i (%s): Expected %s, got %s\n",
		token.line,
		token.character,
		section,
		str,
		l_token_type_string(token.type));

	free(str);

	exit(1);
}

void l_scanner_unexpected(l_token_type expected, l_token token, char* section)
{
	fprintf(stderr,
		"line %i:%i (%s): Expected %s, got %s\n",
		token.line,
		token.character,
		section,
		l_token_type_string(expected),
		l_token_type_string(token.type));

	exit(1);
}

void l_scanner_skip(l_scanner* scanner, l_token_type type, char* section)
{
	l_token token = l_scanner_next(scanner);
	if (token.type != type)
	{
		l_scanner_unexpected(type, token, section);
	}
}

l_token l_scanner_expect(l_scanner* scanner, l_token_type type, char* section)
{
	l_token token = l_scanner_next(scanner);
	if (token.type != type)
	{
		l_scanner_unexpected(type, token, section);
	}

	return token;
}
