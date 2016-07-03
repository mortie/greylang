# Language

This is my project to create a language.

## Syntax

### Example

	a = {
		(n, str);
		repeat(n, { print!(str) })
	};

	a(10, "Hello World");

### Expressions

Expressions are at the heart of the language. The various types are:

#### Assignment

	foo = [expression]

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

#### Function Definition

The simplest way to define a function is simply:

	{
		[expression list]
	}

When defined like that, arguments passed to the function are available as $1,
$2, etc.

Another way to define the function is like this:

	{
		([argument list]);
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
