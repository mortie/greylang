#include "../vm.h"

#include "../platform.h"

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

l_vm_var* l_vm_std_add(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number + b->var.number;
	return v;
}

l_vm_var* l_vm_std_sub(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number - b->var.number;
	return v;
}

l_vm_var* l_vm_std_mul(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number * b->var.number;
	return v;
}

l_vm_var* l_vm_std_div(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number / b->var.number;
	return v;
}

l_vm_var* l_vm_std_pow(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, a);
	expecttype(VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = pow(a->var.number, b->var.number);
	return v;
}

l_vm_var* l_vm_std_eq(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 1; i < args->len; ++i)
	{
		if (!l_vm_var_eq(args->vars[i-1], args->vars[i]))
			return v;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_neq(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	l_vm_var* v = l_vm_std_eq(vm, self, args);
	v->var.boolean = !v->var.boolean;
	return v;
}

l_vm_var* l_vm_std_gt(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	expecttype(VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev > d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_lt(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	expecttype(VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev < d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_gteq(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	expecttype(VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev >= d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_lteq(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	expecttype(VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_NUMBER, args->vars[i]);
		double d = args->vars[i]->var.number;
		if (!(prev <= d))
			return v;

		prev = d;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_and(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 0; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_BOOL, args->vars[i]);
		if (!args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_or(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		expectargc(2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 1;

	for (int i = 0; i < args->len; ++i)
	{
		expecttype(VAR_TYPE_BOOL, args->vars[i]);
		if (args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 0;
	return v;
}

l_vm_var* l_vm_std_if(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* cond = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_BOOL, cond);
	expecttype(VAR_TYPE_FUNCTION, func);

	if (cond->var.boolean)
	{
		l_vm_var_function_exec(vm, func->var.function, NULL);
	}

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_repeat(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* num = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_NUMBER, num);
	expecttype(VAR_TYPE_FUNCTION, func);

	l_vm_var* tmp = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	l_vm_var_array* arr = malloc(sizeof(l_vm_var_array));
	arr->vars = malloc(sizeof(l_vm_var));
	arr->len = 1;
	arr->allocd = 1;
	arr->vars[0] = tmp;

	int n = num->var.number;
	for (int i = 0; i < n; ++i)
	{
		tmp->var.number = (double)i;
		l_vm_var_function_exec(vm, func->var.function, arr);
	}

	free(arr->vars);
	free(arr);
	l_vm_var_free(tmp);

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_map(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* arr = args->vars[0];
	l_vm_var* func = args->vars[1];
	expecttype(VAR_TYPE_ARRAY, arr);
	expecttype(VAR_TYPE_FUNCTION, func);

	l_vm_var_array* a = arr->var.array;

	l_vm_var_array* res = malloc(sizeof(l_vm_var_array) * a->len);
	res->vars = malloc(sizeof(l_vm_var*) * a->len);
	res->len = a->len;
	res->allocd = a->len;

	l_vm_var_array* arguments = malloc(sizeof(l_vm_var_array));
	arguments->vars = malloc(sizeof(l_vm_var*));
	arguments->len = 1;
	arguments->allocd = 1;

	for (int i = 0; i < a->len; ++i)
	{
		arguments->vars[0] = a->vars[i];
		res->vars[i] =
			l_vm_var_function_exec(vm, func->var.function, arguments);
	}

	free(arguments->vars);
	free(arguments);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_ARRAY);
	v->var.array = res;
	return v;
}

l_vm_var* l_vm_std_tostring(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* arg = args->vars[0];

	char* str = l_vm_var_tostring(arg);

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	l_vm_var_string* s = malloc(sizeof(l_vm_var_string));
	s->chars = str;
	s->len = strlen(str);
	var->var.string = s;

	return var;
}

l_vm_var* l_vm_std_tonumber(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* arg = args->vars[0];
	expecttype(VAR_TYPE_STRING, arg);

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	var->var.number = atof(arg->var.string->chars);

	return var;
}

l_vm_var* l_vm_std_concat(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	char** strs = malloc(sizeof(char*) * args->len);

	int len = 1;
	for (int i = 0; i < args->len; ++i)
	{
		strs[i] = l_vm_var_tostring(args->vars[i]);
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

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	l_vm_var_string* s = malloc(sizeof(l_vm_var_string));
	s->len = len;
	s->chars = str;
	var->var.string = s;

	free(strs);
	return var;
}

l_vm_var* l_vm_std_type(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(1, args);

	char* str;

	switch (args->vars[0]->type)
	{
	case VAR_TYPE_OBJECT:
		str = "object";
		break;
	case VAR_TYPE_ARRAY:
		str = "array";
		break;
	case VAR_TYPE_FUNCTION:
		str = "function";
		break;
	case VAR_TYPE_STRING:
		str = "string";
		break;
	case VAR_TYPE_CHAR:
		str = "char";
		break;
	case VAR_TYPE_NUMBER:
		str = "number";
		break;
	case VAR_TYPE_BOOL:
		str = "bool";
		break;
	case VAR_TYPE_PTR:
	case VAR_TYPE_NONE:
		str = "none";
	}

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	var->var.string = malloc(sizeof(l_vm_var_string));
	var->var.string->chars = str;
	var->var.string->len = strlen(str);
	return var;
}

static l_vm_var* loadc_run(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(2, args);
	l_vm_var* vsymbol = args->vars[0];
	l_vm_var* vfargs = args->vars[1];
	expecttype(VAR_TYPE_STRING, vsymbol);
	expecttype(VAR_TYPE_ARRAY, vfargs);

	char* symbol = vsymbol->var.string->chars;
	l_vm_var_array* fargs = vfargs->var.array;

	l_vm_var* vdl = l_vm_var_object_lookup(self->var.object, "dl");
	l_plat_dl* dl = (l_plat_dl*)vdl->var.ptr;

	l_vm_var* (*fptr)(l_vm_var_array*) = l_plat_dl_read(dl, symbol);

	return (*fptr)(fargs);
}

l_vm_var* l_vm_std_loadc(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	expectargc(1, args);
	l_vm_var* arg = args->vars[0];
	expecttype(VAR_TYPE_STRING, arg);

	l_plat_dl* dl = l_plat_dl_open(arg->var.string->chars);

	// Object to return
	l_vm_var_object* obj = l_vm_var_object_create();
	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_OBJECT);
	var->var.object = obj;

	// Pointer to dl
	l_vm_var* ptr = l_vm_var_create(vm, VAR_TYPE_PTR);
	ptr->var.ptr = (void*)dl;

	// Run function
	l_vm_var_function* runf = l_vm_var_function_create(NULL);
	runf->fptr = &loadc_run;
	runf->self = l_vm_var_create(vm, VAR_TYPE_OBJECT);
	runf->self->var.object = obj;
	l_vm_var* runv = l_vm_var_create(vm, VAR_TYPE_FUNCTION);
	runv->var.function = runf;

	l_vm_var_object_set(obj, "dl", ptr);
	l_vm_var_object_set(obj, "run", runv);

	return var;
}

l_vm_var* l_vm_std_print(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	for (int i = 0; i < args->len; ++i)
	{
		printf("%s", l_vm_var_tostring(args->vars[i]));
	}
	printf("\n");

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_read(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
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

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

	char* s = readline(prompt);

	str->chars = s;
	str->len = strlen(s);
	var->var.string = str;

	return var;
}
