#include "src/test.h"
#include "../src/vm.h"

#include <stdlib.h>

void test_vm_char()
{
	/*
	 * These tests uses these unicode code points for testing:
	 *     1 byte: 0x41 ('A')
	 *     2 byte: 0xC6 ('Æ')
	 *     3 byte: 0x2603 (Snowman)
	 *     4 byte: 0x28207 (A Han character)
	 */

	it("decodes 1 byte utf-8 characters") {
		asserteq(vm_var_char_from_utf8("A"), 0x41);
	}

	it("decodes 2 byte utf-8 characters") {
		asserteq(vm_var_char_from_utf8("\u00C6"), 0xC6);
	}

	it("decodes 3 byte utf-8 characters") {
		asserteq(vm_var_char_from_utf8("\u2603"), 0x2603);
	}

	it("decodes 4 byte utf-8 characters") {
		asserteq(vm_var_char_from_utf8("\U00028207"), 0x28207);
	}

	it("converts plain ascii text to a vm_var_char array") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_CHAR);
		vm_var_char_array_from_utf8("Hello", &arr);

		asserteq(arr.varc, 5);
		asserteq(arr.vars[0]->var.character, 'H');
		asserteq(arr.vars[1]->var.character, 'e');
		asserteq(arr.vars[2]->var.character, 'l');
		asserteq(arr.vars[3]->var.character, 'l');
		asserteq(arr.vars[4]->var.character, 'o');

		vm_var_array_free(&arr);
	}

	it("converts utf8 text to a vm_var_char array") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_CHAR);
		vm_var_char_array_from_utf8("A\u00C6\u2603\U00028207B", &arr);

		asserteq(arr.varc, 5);
		asserteq(arr.vars[0]->var.character, 'A');
		asserteq(arr.vars[1]->var.character, 0xC6);
		asserteq(arr.vars[2]->var.character, 0x2603);
		asserteq(arr.vars[3]->var.character, 0x28207);
		asserteq(arr.vars[4]->var.character, 'B');

		vm_var_array_free(&arr);
	}

	it("converts vm_var_chars to 1 byte utf8 characters") {
		char buf[5];
		vm_var_char_to_utf8('A', buf);
		asserteq_str(buf, "A");
	}

	it("converts vm_var_chars to 2 byte utf8 characters") {
		char buf[5];
		vm_var_char_to_utf8(0x00C6, buf);
		asserteq_str(buf, "\u00C6");
	}

	it("converts vm_var_chars to 3 byte utf8 characters") {
		char buf[5];
		vm_var_char_to_utf8(0x2603, buf);
		asserteq_str(buf, "\u2603");
	}

	it("converts vm_var_chars to 4 byte utf8 characters") {
		char buf[5];
		vm_var_char_to_utf8(0x28207, buf);
		asserteq_str(buf, "\U00028207");
	}

	it("converts vm_var_char arrays to utf8 text") {
		vm_var_array arr;
		vm_var_array_init(&arr, VAR_TYPE_CHAR);
		vm_var_char_array_from_utf8("A\u00C6\u2603\U00028207B", &arr);

		char *utf8text = vm_var_char_array_to_utf8(&arr);
		asserteq_str(utf8text, "A\u00C6\u2603\U00028207B");

		vm_var_array_free(&arr);
		free(utf8text);
	}

	done();
}
