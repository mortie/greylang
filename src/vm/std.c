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

l_vm_var* l_vm_std_lookup(l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* func = args->vars[0];
	l_vm_var* str = args->vars[1];
	expecttype(VAR_TYPE_FUNCTION, func);
	expecttype(VAR_TYPE_STRING, str);

	l_vm_var_function* f = func->var.function;
	l_vm_var_string* s = str->var.string;

	char* name = malloc(s->len + 2);
	memcpy(name, s->chars + 1, s->len);
	name[s->len + 1] = '\0';
	name[0] = '$';

	l_vm_var* v = l_vm_scope_lookup(f->scope, name);

	free(name);
	return v;
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

l_vm_var* l_vm_std_print(l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* str = args->vars[0];
	expecttype(VAR_TYPE_STRING, str);
	printf("%s\n", str->var.string->chars);

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

	l_vm_var* var = l_vm_var_create(VAR_TYPE_STRING);
	l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

	char* s = readline(prompt);

	str->chars = s;
	str->len = strlen(s);
	var->var.string = str;

	return var;
}
