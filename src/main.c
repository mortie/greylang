#include "scanner.h"
#include "parser.h"
#include "vm.h"
#include "platform.h"

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

static int repl()
{
	l_vm* vm = l_vm_create();
	vm_map_increfs(vm->root);
	while (1)
	{
		char* str = l_plat_readline("> ");
		if (str == NULL)
			return 1;

		l_scanner* stream = l_scanner_create_str(str);
		vm_var* v = l_vm_run(vm, l_parse(stream));

		char* s = vm_var_tostring(v);
		if (v->type == VAR_TYPE_ARRAY && v->var.array->type == VAR_TYPE_CHAR)
			printf("\"%s\"\n", s);
		else if (v->type == VAR_TYPE_CHAR)
			printf("'%s'\n", s);
		else
			printf("%s\n", s);

		free(str);

		l_vm_cleanup(vm);
	}
	vm_map_decrefs(vm->root);

	return 0;
}

static int exec(FILE* f)
{
	l_scanner* stream = l_scanner_create(f);
	l_p_expr_list* list = l_parse(stream);

	l_vm* vm = l_vm_create();
	l_vm_run(vm, list);

	return 0;
}

static int pretty(FILE* f)
{
	l_scanner* stream = l_scanner_create(f);
	l_p_expr_list* list = l_parse(stream);

	l_pretty_expr_list(list, 0, stdout);

	return 0;
}

int main(int argc, char** argv)
{
	if (argc == 1)
		return repl();

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

	// Get file
	FILE* f;
	if (strcmp(file, "-") == 0)
	{
		f = stdin;
	}
	else
	{
		f = fopen(file, "r");
	}

	// Error if file is invalid
	if (f == NULL)
	{
		perror("hello");
		exit(1);
	}

	if (opr == OPR_EXEC)
		return exec(f);
	else if (opr == OPR_PRETTY)
		return pretty(f);
}
