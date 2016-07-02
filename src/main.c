#include "scanner.h"
#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	l_scanner* stream = l_scanner_create(stdin);

	l_p_expr_list* list = l_parse(stream);
	l_pretty_print(list, stdout);
}
