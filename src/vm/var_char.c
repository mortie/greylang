#include "../vm.h"

int vm_var_char_count_bytes(uint8_t first)
{
	if (first >= 0xF0)
		return 4;
	else if (first >= 0xE0)
		return 3;
	else if (first >= 0xC0)
		return 2;
	else
		return 1;
}

vm_var_char vm_var_char_from_utf8(char *str)
{
	uint32_t ch = 0;
	uint8_t first = str[0];

	int nbytes = vm_var_char_count_bytes(first);

	if (nbytes == 1)
	{
		ch |= str[0];
	}
	else if (nbytes == 2)
	{
		ch |= str[1] & 0b00111111;
		ch |= (str[0] & 0b00011111) << 6;
	}
	else if (nbytes == 3)
	{
		ch |= str[2] & 0b00111111;
		ch |= (str[1] & 0b00111111) << 6;
		ch |= (str[0] & 0b00001111) << 12;
	}
	else if (nbytes == 4)
	{
		ch |= str[3] & 0b00111111;
		ch |= (str[2] & 0b00111111) << 6;
		ch |= (str[1] & 0b00111111) << 12;
		ch |= (str[0] & 0b00000111) << 18;
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
		int nbytes = vm_var_char_count_bytes(first);

		uint32_t ch = vm_var_char_from_utf8(str + i);
		vm_var *var = vm_var_create(VAR_TYPE_CHAR);
		var->var.character = ch;
		vm_var_array_set(arr, currChar, var);

		currChar += 1;
		i += nbytes;
		if (str[i] == 0)
			break;
	}
}
