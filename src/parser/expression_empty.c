#include "../parser.h"

#include <stdlib.h>
#include <stdio.h>

void l_pretty_expression_empty(int depth, FILE* file)
{
	char* tabs;
	P_TABS(depth, tabs);
	fprintf(file, "%s", tabs);
}
