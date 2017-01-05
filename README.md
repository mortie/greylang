# The Grey Programming Language

This is greylang, my project to create a language.

# Example

	a := {(n, str)
		repeat(n, { print(str) });
	};

	a(10, "Hello World");

That example defines a function called `a`, which takes two parameters: n and
str. It then calls the built-in function `repeat`, which runs a function `n`
times. The function to be run `n` times is defined inline, and just prints the
contents of `str` when called.

# Another example

	%of := {(x, y)
		y * (x / 100)
	};

	print(10 %of 250) // Prints 25

This creates a function called %of, which just calculates x percent of y. Then,
we call it inbetween two values. It works exactly the same as `%of(10, 250)`,
but looks a lot better.

# Docs

* [Standard Library](https://github.com/mortie/greylang/blob/master/stdlib.md)

# Features

## No blocks

Most C-style languages use braces to indicate a block. In greylang, braces
define functions. `{10}` is all that's required to create a function which
always returns 10, so `a := {10}; print(a())` will print 10.

## No "operators"

Most languages have a concept of operators, like `+`, `-`, `*`, and `/`. In
grey, those are functions. `+(10, 20)` will return 30.

Grey also has infix function calls, so `10 + 20` will return exactly the same
as `+(10, 20)`.

The downside to this is that there is no operator precedence. All expressions
are evaluated right to left, so `10 * 4 + 3` will be evaluated like
`10 * (4 + 3)`. The solution is simply to be explicit about the order:
`(10 * 4) + 3`.

## Nice variable names

Many languages are annoyingly strict about what characters are valid in
identifiers. Grey has very few limits. This means that you can have emoji
characters from other languages as identifiers, but more importantly, it allows
nicities like ending boolean variables or functions with `?`. It also allows
`+`, `-`, etc. to just be regular identifiers.

Since variables can contain `-`, nice `dash-case` variable names, which I much
prefer to `camelCase` or `snake_case`.

## Decent Unicode support

Strings in grey are arrays of 32 bit characters. That means each Unicode
code point is handled as its own character, while languages like Java and
Javascript only fit 16 bits in their characters and have to deal with cases
where one unicode code point spans two "characters"

In short, `"𨈇"[0]` works as expected in greylang, but not in various other
languages.

This isn't perfect unicode support, as some symbols are made out of multiple
Unicode code points; "é" can for example be written as the two code points "e"
followed by U+0301 (a combining accent character). Grey won't help you there,
but solving that problem would be way too messy.

The source files are assumed to contain only valid utf-8 text.

Note: this is more performant than it sounds, as strings cache their utf-8
representation, and only need to parse the utf-8 text when you index individual
values. In the common case where string literals are defined and then ignored,
no utf-8 parsing or encoding will be performed. 

# To Do

* <del>Change code style of non-VM parts to match the VM</del>
* Better error handling
* Let functions specify their arguments' types
* Char literals
* Array prototype functions
	* sub, reverse, insert, push, pop
* Some way to do the equivalent of +=, -=, etc.
* Give getter and setter functions syntactic sugar to make them look like
  property lookups
* Deal with reference cycles with functions
* Free memory allocated while parsing
* "Placeholder" function to indicate abstract methods
* Make module system
* Make various modules
	* File system I/O
	* JSON parsing
	* HTTP (_long_ term)
