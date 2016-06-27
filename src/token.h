#ifndef L_TOKEN_H
#define L_TOKEN_H

typedef enum l_token_type
{
	TOKEN_NONE,
	TOKEN_ERROR,
	TOKEN_STRING_LITERAL,
	TOKEN_CHAR_LITERAL,
	TOKEN_NUM_LITERAL,
	TOKEN_NAME,
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
	TOKEN_OPENPAREN,
	TOKEN_CLOSEPAREN,
	TOKEN_OPENBRACKET,
	TOKEN_CLOSEBRACKET,
	TOKEN_OPENBRACE,
	TOKEN_CLOSEBRACE
} l_token_type;

typedef struct l_token
{
	l_token_type tokentype;
	char* content;
	int contentlen;
	int line;
} l_token;

char* l_token_type_string(l_token_type);

#endif
