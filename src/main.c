#include "scanner.h"
#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* str = argv[1];
	l_scanner* stream = l_scanner_create(stdin);

	l_p_expression_list* list = l_parse(stream);
	l_pretty_print(list, stdout);
}
