#include "scanner.h"
#include "parser.h"
#include "vm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE* in = fopen(argv[1], "r");
	if (in == NULL)
	{
		perror("hi");
		exit(1);
	}

	l_scanner* stream = l_scanner_create(in);

	l_p_expr_list* list = l_parse(stream);

	l_vm* vm = l_vm_create();
	l_vm_run(vm, list);
}
