# The Grey Programming Language

This is grey-lang, my project to create a language.

## Syntax

### Example

	a = {(n, str)
		repeat(n, { print(str) });
	};

	a(10, "Hello World");

That example defines a function called `a`, which takes two parameters: n and
str. It then calls the built-in function `repeat`, which runs a function `n`
times. The function to be run `n` times is defined inline, and just prints the
contents of `str` when called.

### Another example

	%of = {(x, y)
		y * (x / 100)
	}

	print(10 %of 250) // Prints 25

This creates a function called %of, which just calculates x percent of y. Then,
we call it inbetween two values. It works exactly the same as `%of(10, 250)`,
but looks a lot better.

### Expressions

Expressions are at the heart of the language. The various types are:

#### Assignment

	foo = [expression]

#### Array Literal

	[ [expression], [expression], [expression] ]

#### String Literal

	"some string"

#### Number Literal

	9.0

or

	9

#### Character Literal

	'c'

#### Function Call

	bar([expression], [expression])

You can also use infix function calls, like so:

	[expression] + [expression]

#### Function Definition

The simplest way to define a function is simply:

	{
		[expression list]
	}

When defined like that, arguments passed to the function are available as $1,
$2, etc.

Another way to define the function is like this:

	{
		([argument list])
		[expression list]
	}

where the argument list looks like this:

	[expression], [expression], [expression]

etc.

### Expression List

The root of all programs and functions is the expression list, which looks like
this:

	[expression];
	[expression];
	[expression];

Optionally, you may omitt the last semicolon.
