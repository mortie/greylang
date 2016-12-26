#ifndef L_TOKEN_H
#define L_TOKEN_H

typedef enum l_token_type
{
	TOKEN_NONE,
	TOKEN_ERROR,
	TOKEN_IGNORED,
	TOKEN_STRING_LITERAL,
	TOKEN_NUM_LITERAL,
	TOKEN_NAME,
	TOKEN_PERIOD,
	TOKEN_COMMA,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	TOKEN_EQUALS,
	TOKEN_COLONEQUALS,
	TOKEN_OPENPAREN,
	TOKEN_CLOSEPAREN,
	TOKEN_OPENBRACKET,
	TOKEN_CLOSEBRACKET,
	TOKEN_OPENBRACE,
	TOKEN_CLOSEBRACE
} l_token_type;

typedef struct l_token
{
	l_token_type type;
	char* content;
	int contentlen;
	int line;
	int linechar;
} l_token;

char* l_token_type_string(l_token_type);

#endif
