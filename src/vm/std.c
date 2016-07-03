#include "../vm.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <readline/readline.h>

static void expecttype(l_vm_var_type expected, l_vm_var* var)
{
	if (var->type != expected)
	{
		l_vm_error_type(expected, var->type);
	}
}

static void expectargc(int expected, l_vm_var_array* args)
{
	if (args->len != expected)
	{
		l_vm_error_argnum(expected, args->len);
	}
}

static char* tostring(l_vm_var* var)
{
	char* str;

	switch (var->type)
	{
	case VAR_TYPE_ARRAY:
		str = "[array]";
		break;
	case VAR_TYPE_FUNCTION:
		str = "[function]";
		break;
	case VAR_TYPE_STRING:
		return var->var.string->chars;
		break;
	case VAR_TYPE_CHAR:
		str = malloc(2);
		str[0] = var->var.character;
		str[1] = '\0';
		break;
	case VAR_TYPE_NUMBER:
	{
		str = malloc(16);
		double num = var->var.number;
		if (num == floor(num))
		{
			snprintf(str, 16, "%d", (int)num);
		}
		else
		{
			snprintf(str, 16, "%f", num);
		}
		break;
	}
	case VAR_TYPE_BOOL:
		if (var->var.boolean)
		{
			str = "[true]";
		}
		else
		{
			str = "[false]";
		}
		break;
	case VAR_TYPE_NONE:
		str = "[none]";
		break;
	}

	return str;
}

l_vm_var* l_vm_std_add(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number + b->var.number;
	return v;
}

l_vm_var* l_vm_std_sub(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number - b->var.number;
	return v;
}

l_vm_var* l_vm_std_mul(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number * b->var.number;
	return v;
}

l_vm_var* l_vm_std_div(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = a->var.number / b->var.number;
	return v;
}

l_vm_var* l_vm_std_pow(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_NUMBER);
	v->var.number = pow(a->var.number, b->var.number);
	return v;
}

l_vm_var* l_vm_std_callfunc(l_vm_var_array* args)
{
	if (args->len < 2)
	{
		l_vm_error_argnum(2, args->len);
	}

	l_vm_var* func = args->vars[0];
	l_vm_var* str = args->vars[1];
	expecttype(VAR_TYPE_FUNCTION, func);
	expecttype(VAR_TYPE_STRING, str);

	l_vm_var_function* f = func->var.function;
	l_vm_var_string* s = str->var.string;

	/*
	 * Find the correct function
	 */
	char* name = malloc(s->len + 2);
	memcpy(name + 1, s->chars, s->len);
	name[s->len + 1] = '\0';
	name[0] = '$';

	l_vm_var* rf = l_vm_scope_lookup(f->scope, name);
	if (rf == NULL)
		rf = l_vm_var_create(VAR_TYPE_NONE);

	free(name);
	expecttype(VAR_TYPE_FUNCTION, rf);

	/*
	 * Call the function
	 */

	l_vm_var_array* fargs = malloc(sizeof(l_vm_var_array));
	int fargc = args->len - 2;

	fargs->len = fargc;
	fargs->vars = (args->vars + 2);

	l_vm_var* res = l_vm_var_function_exec(rf->var.function, fargs);

	free(fargs);

	return res;
}

l_vm_var* l_vm_std_callsetter(l_vm_var_array* args)
{
	if (args->len < 2)
	{
		l_vm_error_argnum(2, args->len);
	}

	expecttype(VAR_TYPE_FUNCTION, args->vars[0]);
	expecttype(VAR_TYPE_STRING, args->vars[1]);

	l_vm_var_string* nwname = malloc(sizeof(l_vm_var_string));

	l_vm_var_string* name = args->vars[1]->var.string;
	nwname->len = name->len + 1;
	nwname->chars = malloc(nwname->len + 2);
	memmove(nwname->chars + 1, name->chars, name->len + 1);
	nwname->chars[0] = '$';

	args->vars[1]->var.string = nwname;

	l_vm_var* res = l_vm_std_callfunc(args);
	free(nwname->chars);
	free(nwname);

	return res;
}

l_vm_var* l_vm_std_if(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* cond = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_BOOL, cond);
	expecttype(VAR_TYPE_FUNCTION, func);

	if (cond->var.boolean)
	{
		l_vm_var_function_exec(func->var.function, NULL);
	}

	return l_vm_var_create(VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_repeat(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* num = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, num);
	expecttype(VAR_TYPE_FUNCTION, func);

	l_vm_var* tmp = l_vm_var_create(VAR_TYPE_NUMBER);
	l_vm_var_array* arr = malloc(sizeof(l_vm_var_array));
	arr->vars = malloc(sizeof(l_vm_var));
	arr->len = 1;
	arr->allocd = 1;
	arr->vars[0] = tmp;

	int n = num->var.number;
	for (int i = 0; i < n; ++i)
	{
		tmp->var.number = (double)i;
		l_vm_var_function_exec(func->var.function, arr);
	}

	free(arr->vars);
	free(arr);
	l_vm_var_free(tmp);

	return l_vm_var_create(VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_map(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* arr = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_ARRAY, arr);
	expecttype(VAR_TYPE_FUNCTION, func);

	l_vm_var_array* a = arr->var.array;

	l_vm_var_array* res = malloc(sizeof(l_vm_var_array) * a->len);
	res->len = a->len;
	res->allocd = a->len;

	l_vm_var_array* arguments = malloc(sizeof(l_vm_var_array));
	arguments->len = 1;
	arguments->allocd = 1;

	for (int i = 0; i < a->len; ++i)
	{
		arguments->vars[0] = a->vars[i];
		res->vars[i] =
			l_vm_var_function_exec(func->var.function, arguments);
	}

	free(arguments);

	l_vm_var* v = l_vm_var_create(VAR_TYPE_ARRAY);
	v->var.array = res;
	return v;
}

l_vm_var* l_vm_std_tostring(l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* arg = args->vars[0];

	char* str = tostring(arg);

	l_vm_var* var = l_vm_var_create(VAR_TYPE_STRING);
	l_vm_var_string* s = malloc(sizeof(l_vm_var_string));
	s->chars = str;
	s->len = strlen(str);
	var->var.string = s;

	return var;
}

l_vm_var* l_vm_std_tonumber(l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* arg = args->vars[0];
	expecttype(VAR_TYPE_STRING, arg);

	l_vm_var* var = l_vm_var_create(VAR_TYPE_NUMBER);
	var->var.number = atof(arg->var.string->chars);

	return var;
}

l_vm_var* l_vm_std_concat(l_vm_var_array* args)
{
	char** strs = malloc(sizeof(char*) * args->len);

	int len = 1;
	for (int i = 0; i < args->len; ++i)
	{
		strs[i] = tostring(args->vars[i]);
		len += strlen(strs[i]);
	}

	char* str = malloc(len);
	int total = 0;
	for (int i = 0; i < args->len; ++i)
	{
		int l = strlen(strs[i]);
		memcpy(str + total, strs[i], l);
		total +=  l;
	}

	str[len - 1] = '\0';

	l_vm_var* var = l_vm_var_create(VAR_TYPE_STRING);
	l_vm_var_string* s = malloc(sizeof(l_vm_var_string));
	s->len = len;
	s->chars = str;
	var->var.string = s;

	free(strs);
	return var;
}

l_vm_var* l_vm_std_print(l_vm_var_array* args)
{
	for (int i = 0; i < args->len; ++i)
	{
		printf("%s", tostring(args->vars[i]));
	}
	printf("\n");

	return l_vm_var_create(VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_read(l_vm_var_array* args)
{
	char* prompt;
	if (args->len == 0)
	{
		prompt = "";
	}
	else if (args->len == 1)
	{
		expecttype(VAR_TYPE_STRING, args->vars[0]);
		prompt = args->vars[0]->var.string->chars;
	}
	else
	{
		l_vm_error_argnum(1, args->len);
		return NULL;
	}

	l_vm_var* var = l_vm_var_create(VAR_TYPE_STRING);
	l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

	char* s = readline(prompt);

	str->chars = s;
	str->len = strlen(s);
	var->var.string = str;

	return var;
}
