#include "src/test.h"

void test_foo()
{
	it("does things") {
		assert(1 == 2);
	}

	it("does other things") {
		asserteq(2, 3);
	}

	it("does other thing") {
		assert(1);
	}

	done();
}

void test_bar()
{
	it("doesn't actually do much") {
		int foo = 1;
		int bar = 2;
		asserteq(foo, bar);
	}

	it("is pretty") {
		if (1 != 1)
			fail("Logic isn't working anymore");
	}

	done();
}
