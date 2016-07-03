#include "../token.h"

char* l_token_type_string(l_token_type type)
{
	switch (type)
	{
	case TOKEN_NONE:
		return "none";
	case TOKEN_ERROR:
		return "error";
	case TOKEN_STRING_LITERAL:
		return "string literal";
	case TOKEN_CHAR_LITERAL:
		return "char literal";
	case TOKEN_NUM_LITERAL:
		return "num literal";
	case TOKEN_NAME:
		return "name";
	case TOKEN_PERIOD:
		return "period '.'";
	case TOKEN_COMMA:
		return "comma ','";
	case TOKEN_SEMICOLON:
		return "semicolon ';'";
	case TOKEN_EQUALS:
		return "equals '='";
	case TOKEN_OPENPAREN:
		return "open paren '('";
	case TOKEN_CLOSEPAREN:
		return "close paren ')'";
	case TOKEN_OPENBRACKET:
		return "open bracket '['";
	case TOKEN_CLOSEBRACKET:
		return "close bracket ']'";
	case TOKEN_OPENBRACE:
		return "open brace '{'";
	case TOKEN_CLOSEBRACE:
		return "close brace '}'";
	default:
		return "invalid token type";
	}
}
