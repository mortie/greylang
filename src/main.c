#include "scanner.h"
#include "parser.h"
#include "vm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum opr_type
{
	OPR_EXEC,
	OPR_PRETTY
};

static void usage(char* name)
{
	fprintf(stderr, "Usage: %s [options] <file>\n", name);
}

int main(int argc, char** argv)
{
	enum opr_type opr = OPR_EXEC;
	char* file = NULL;

	int errors = 0;
	for (int i = 1; i < argc; ++i)
	{
		char* arg = argv[i];

		if (strcmp(arg, "-") == 0)
		{
			file = "-";
		}
		else if (arg[0] == '-')
		{
			if (
					strcmp(arg, "-p") ||
					strcmp(arg, "--pretty"))
			{
				opr = OPR_PRETTY;
			}
			else
			{
				fprintf(stderr, "Unrecognized option: '%s'\n", arg);
				errors = 1;
			}
		}
		else
		{
			file = arg;
		}
	}

	if (file == NULL) errors = 1;

	if (errors)
	{
		usage(argv[0]);
		return 1;
	}

	FILE* in;
	if (strcmp(file, "-") == 0)
	{
		in = stdin;
	}
	else
	{
		in = fopen(file, "r");
	}

	if (in == NULL)
	{
		perror("hello");
		exit(1);
	}

	l_scanner* stream = l_scanner_create(in);
	l_p_expr_list* list = l_parse(stream);

	switch (opr)
	{
	case OPR_EXEC:
	{
		l_vm* vm = l_vm_create();
		l_vm_run(vm, list);
		break;
	}
	case OPR_PRETTY:
		l_pretty_expr_list(list, 0, stdout);
		break;
	}
}
