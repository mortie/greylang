#include "../vm.h"

#include <string.h>
#include <stdlib.h>

// The minimum value of the first `char` to indicate n bytes
#define C4bytes 0xF0
#define C3bytes 0xE0
#define C2bytes 0xC0

// The minimum value for the vm_var_char for the utf8 equivalent to be n bytes
#define U4bytes 0x10000
#define U3bytes 0x0800
#define U2bytes 0x0080

vm_var_char vm_var_char_from_utf8(char *str)
{
	vm_var_char ch = 0;
	uint8_t first = str[0];

	if (first >= C4bytes)
	{
		ch |= str[3] & 0b00111111;
		ch |= (str[2] & 0b00111111) << 6;
		ch |= (str[1] & 0b00111111) << 12;
		ch |= (str[0] & 0b00000111) << 18;
	}
	else if (first >= C3bytes)
	{
		ch |= str[2] & 0b00111111;
		ch |= (str[1] & 0b00111111) << 6;
		ch |= (str[0] & 0b00001111) << 12;
	}
	else if (first >= C2bytes)
	{
		ch |= str[1] & 0b00111111;
		ch |= (str[0] & 0b00011111) << 6;
	}
	else
	{
		ch |= str[0];
	}

	return ch;
}

void vm_var_char_array_from_utf8(char *str, vm_var_array *arr)
{
	arr->type = VAR_TYPE_CHAR;

	int i = 0;
	int currChar = 0;
	while (1)
	{
		uint8_t first = str[i];
		int nbytes;
		if (first >= C4bytes) nbytes = 4;
		else if (first >= C3bytes) nbytes = 3;
		else if (first >= C2bytes) nbytes = 2;
		else nbytes = 1;

		vm_var_char ch = vm_var_char_from_utf8(str + i);
		vm_var *var = vm_var_create(VAR_TYPE_CHAR);
		var->var.character = ch;
		vm_var_array_set(arr, currChar, var);

		currChar += 1;
		i += nbytes;
		if (str[i] == 0)
			break;
	}
}

void vm_var_char_to_utf8(vm_var_char ch, char *buf)
{
	memset(buf, 0, 5);
	if (ch >= U4bytes)
	{
		buf[0] = 0b11110000 | ((ch >> 18) & 0b00000111);
		buf[1] = 0b10000000 | ((ch >> 12) & 0b00111111);
		buf[2] = 0b10000000 | ((ch >> 6) & 0b00111111);
		buf[3] = 0b10000000 | (ch & 0b00111111);
	}
	else if (ch >= U3bytes)
	{
		buf[0] = 0b11100000 | ((ch >> 12) & 0b00001111);
		buf[1] = 0b10000000 | ((ch >> 6) & 0b00111111);
		buf[2] = 0b10000000 | (ch & 0b00111111);
	}
	else if (ch >= U2bytes)
	{
		buf[0] = 0b11000000 | ((ch >> 6) & 0b00011111);
		buf[1] = 0b10000000 | (ch & 0b00111111);
	}
	else
	{
		buf[0] = ch;
	}
}

char *vm_var_char_array_to_utf8(vm_var_array *arr)
{
	char buf[5];
	char *str = malloc((arr->varc * 4) + 1);
	int len = 0;
	str[0] = 0;

	for (int i = 0; i < arr->varc; ++i)
	{
		vm_var_char ch = arr->vars[i]->var.character;
		vm_var_char_to_utf8(ch, buf);

		int nbytes = strlen(buf);
		memcpy(str + len, buf, nbytes);
		len += nbytes;
		str[len] = 0;
	}

	str = realloc(str, len + 1);

	return str;
}
