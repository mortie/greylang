# The Grey Programming Language

This is grey-lang, my project to create a language.

# Example

	a = {(n, str)
		repeat(n, { print(str) });
	};

	a(10, "Hello World");

That example defines a function called `a`, which takes two parameters: n and
str. It then calls the built-in function `repeat`, which runs a function `n`
times. The function to be run `n` times is defined inline, and just prints the
contents of `str` when called.

# Another example

	%of = {(x, y)
		y * (x / 100)
	};

	print(10 %of 250) // Prints 25

This creates a function called %of, which just calculates x percent of y. Then,
we call it inbetween two values. It works exactly the same as `%of(10, 250)`,
but looks a lot better.

# Docs

* [Standard Library](https://github.com/mortie/greylang/blob/master/stdlib.md)
