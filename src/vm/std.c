#include "../vm.h"

#include "../platform.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define RETIFERR(var) \
	do { \
		if (var->type == VAR_TYPE_ERROR) \
			return var; \
	} while(0);

#define EXPECTTYPE(vm, expected, var) \
	do { \
		RETIFERR(var); \
		if (var->type != expected) \
			return l_vm_error_type(vm, expected, var->type); \
	} while(0);

#define EXPECTARGC(vm, expected, args) \
	do { \
		if (args->len != expected) \
			return l_vm_error_argc(vm, expected, args->len); \
	} while(0);

l_vm_var* l_vm_std_add(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, a);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number + b->var.number;
	return v;
}

l_vm_var* l_vm_std_sub(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, a);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number - b->var.number;
	return v;
}

l_vm_var* l_vm_std_mul(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, a);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number * b->var.number;
	return v;
}

l_vm_var* l_vm_std_div(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, a);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = a->var.number / b->var.number;
	return v;
}

l_vm_var* l_vm_std_pow(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* a = args->vars[0];
	l_vm_var* b = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, a);
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, b);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	v->var.number = pow(a->var.number, b->var.number);
	return v;
}

l_vm_var* l_vm_std_eq(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 1; i < args->len; ++i)
	{
		RETIFERR(args->vars[i-1]);
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
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
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
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
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
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
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
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	double prev = args->vars[0]->var.number;
	for (int i = 1; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[i]);
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
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 0;

	for (int i = 0; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_BOOL, args->vars[i]);
		if (!args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 1;
	return v;
}

l_vm_var* l_vm_std_or(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	if (args->len < 2)
		EXPECTARGC(vm, 2, args);

	l_vm_var* v = l_vm_var_create(vm, VAR_TYPE_BOOL);
	v->var.boolean = 1;

	for (int i = 0; i < args->len; ++i)
	{
		EXPECTTYPE(vm, VAR_TYPE_BOOL, args->vars[i]);
		if (args->vars[i]->var.boolean)
			return v;
	}

	v->var.boolean = 0;
	return v;
}

l_vm_var* l_vm_std_if(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* cond = args->vars[0];
	l_vm_var* func = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_BOOL, cond);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

	if (cond->var.boolean)
	{
		l_vm_var_function_exec(vm, func->var.function, NULL);
	}

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_repeat(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 2, args);
	l_vm_var* num = args->vars[0];
	l_vm_var* func = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_NUMBER, num);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

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
	EXPECTARGC(vm, 2, args);
	l_vm_var* arr = args->vars[0];
	l_vm_var* func = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_ARRAY, arr);
	EXPECTTYPE(vm, VAR_TYPE_FUNCTION, func);

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
	EXPECTARGC(vm, 1, args);
	l_vm_var* arg = args->vars[0];
	RETIFERR(arg);

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
	EXPECTARGC(vm, 1, args);
	l_vm_var* arg = args->vars[0];
	EXPECTTYPE(vm, VAR_TYPE_STRING, arg);

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
		RETIFERR(args->vars[i]);
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

l_vm_var* l_vm_std_error(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 1, args);
	EXPECTTYPE(vm, VAR_TYPE_STRING, args->vars[0]);

	l_vm_var_string* strvar =  args->vars[0]->var.string;
	l_vm_var* var = l_vm_error(vm, strvar->chars);

	return var;
}

l_vm_var* l_vm_std_type(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 1, args);
	RETIFERR(args->vars[0]);

	char* str = "";

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
	case VAR_TYPE_ERROR:
		str = "error";
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
	EXPECTARGC(vm, 2, args);
	l_vm_var* vsymbol = args->vars[0];
	l_vm_var* vfargs = args->vars[1];
	EXPECTTYPE(vm, VAR_TYPE_STRING, vsymbol);
	EXPECTTYPE(vm, VAR_TYPE_ARRAY, vfargs);

	char* symbol = vsymbol->var.string->chars;
	l_vm_var_array* fargs = vfargs->var.array;

	l_vm_var* vdl = l_vm_map_lookup(self->map, "dl");
	l_plat_dl* dl = (l_plat_dl*)vdl->var.ptr;

	l_vm_var* (*fptr)(l_vm*, l_vm_var_array*) =
		l_plat_dl_read(dl, symbol);

	return (*fptr)(vm, fargs);
}

l_vm_var* l_vm_std_loadc(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTARGC(vm, 1, args);
	l_vm_var* arg = args->vars[0];
	EXPECTTYPE(vm, VAR_TYPE_STRING, arg);

	// dlopen, and return if error
	l_plat_dl* dl = l_plat_dl_open(arg->var.string->chars);
	if (!dl->success)
		return l_vm_error(vm, "Couldn't load file");

	// Object to return
	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_OBJECT);

	// Pointer to dl
	l_vm_var* ptr = l_vm_var_create(vm, VAR_TYPE_PTR);
	ptr->var.ptr = (void*)dl;

	// Run function
	l_vm_var_function* runf = l_vm_var_function_create(NULL);
	runf->fptr = &loadc_run;
	runf->self = l_vm_var_create(vm, VAR_TYPE_OBJECT);
	runf->self = var;
	l_vm_var* runv = l_vm_var_create(vm, VAR_TYPE_FUNCTION);
	runv->var.function = runf;

	l_vm_map_set(var->map, "dl", ptr);
	l_vm_map_set(var->map, "run", runv);

	return var;
}

l_vm_var* l_vm_std_print(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	for (int i = 0; i < args->len; ++i)
	{
		RETIFERR(args->vars[i]);
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
		EXPECTTYPE(vm, VAR_TYPE_STRING, args->vars[0]);
		prompt = args->vars[0]->var.string->chars;
	}
	else
	{
		EXPECTARGC(vm, 1, args);
		return NULL;
	}

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	l_vm_var_string* str = malloc(sizeof(l_vm_var_string));

	char* s = l_plat_readline(prompt);

	str->chars = s;
	str->len = strlen(s);
	var->var.string = str;

	return var;
}

l_vm_var* l_vm_std_array_len(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTTYPE(vm, VAR_TYPE_ARRAY, self);
	EXPECTARGC(vm, 0, args);

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	var->var.number = (double)self->var.array->len;
	return var;
}

l_vm_var* l_vm_std_array_push(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTTYPE(vm, VAR_TYPE_ARRAY, self);
	EXPECTARGC(vm, 1, args);

	l_vm_var_array* arr = self->var.array;
	if (arr->allocd == 0)
	{
		arr->allocd = 8;
		arr->vars = malloc(sizeof(l_vm_var*) * arr->allocd);
	}
	arr->len += 1;
	if (arr->allocd < arr->len)
	{
		arr->allocd *= 2;
		arr->vars = realloc(arr->vars, sizeof(l_vm_var) * arr->allocd);
	}
	arr->vars[arr->len - 1] = args->vars[0];

	return l_vm_var_create(vm, VAR_TYPE_NONE);
}

l_vm_var* l_vm_std_array_pop(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTTYPE(vm, VAR_TYPE_ARRAY, self);
	EXPECTARGC(vm, 0, args);

	l_vm_var_array* arr = self->var.array;

	if (arr->len == 0)
		return l_vm_var_create(vm, VAR_TYPE_NONE);

	l_vm_var* var = arr->vars[0];
	arr->len -= 1;

	return var;
}

l_vm_var* l_vm_std_string_len(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTTYPE(vm, VAR_TYPE_STRING, self);
	EXPECTARGC(vm, 0, args);

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_NUMBER);
	var->var.number = (double)self->var.string->len;
	return var;
}

l_vm_var* l_vm_std_string_sub(l_vm* vm, l_vm_var* self, l_vm_var_array* args)
{
	EXPECTTYPE(vm, VAR_TYPE_STRING, self);
	if (args->len < 1 || args->len < 2)
	{
		EXPECTARGC(vm, 1, args);
	}

	int start;
	int end;
	int slen = self->var.string->len;

	EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[0]);
	start = (int)args->vars[0]->var.number;

	if (args->len == 2)
	{
		EXPECTTYPE(vm, VAR_TYPE_NUMBER, args->vars[1]);
		end = (int)args->vars[1]->var.number;
		if (end < 0)
			end = slen + end - 1;
		if (end < start)
			end = start;
		if (end >= slen)
			end = slen;
	}
	else
	{
		end = slen - 1;
	}

	int len = end - start + 1;
	printf("%i %i %i\n", start, end, len);
	char* chars = malloc(len + 1);
	memcpy(chars, self->var.string->chars + start, len);
	chars[len + 1] = '\0';

	l_vm_var* var = l_vm_var_create(vm, VAR_TYPE_STRING);
	var->var.string = malloc(sizeof(l_vm_var_string));
	var->var.string->chars = chars;
	var->var.string->len = len;

	return var;
}
