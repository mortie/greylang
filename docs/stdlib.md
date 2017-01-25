# Functions

In greylang, functions server as functions, blocks, and operators from other
languages, so they're even more important than in other languages.

* Functions are defined with just `{` and `}`.
* Functions are just any other data type, and can thus be passed to other
  functions, returned from functions, and assigned to variables.
* Functions can declare arguments, like this:

```
	f := {(str)
		print(str);
	};
	f("Hi World");
```

* Declaring arguments is optional, and all arguments passed to the function is
  available as `$1`, `$2`, etc:

```
	f := { print($1) };
	f("Hi World");
```

* Functions can be called inline, like this:

```
	plus := {(a, b) a + b };
	print(10 plus 20);
```

Note: both objects and functions are defined with `{` and `}`. `{}` is treated
as an empty object, and `{;}` as an empty function. Other than that, it's
interpreted as an object if the first token is a name or string and the second
token is a colon; otherwise it's interpreted as a function.

# Standard Library

## Viariables

* `none`: Variable of type `none`.
* `true`: Boolean with value true.
* `fasle`: Boolean with value false.

## Flow Control

Flow control functions' arguments are reversed if they're called as infix
functions; e.g `if(true, {print("hi")})` becomes `{print("hi")} if true`.

* `if(condition, if-func, else-func)`:
	* Run if-func if condition is true, or else-func otherwise.
	* Returns the return value of the executed function.
	* If values are passed rather than functions, the value is returned.
	* `if(1 == 1, { print("Hello World") });`
	* `print(if(1 == 1, "yes", "no"));`
* `repeat(count, function)`:
	* Run function `count` times.
	* `repeat(10, { print("Hello World") });`
* `while(function, function)`:
	* Run the second function as long as the first function returns true.
	* `i = 0; while({ i < 10 }, { print("Hello World"); i = i + 1 })`
* `for(iterable, function)`:
	* Run the function for each value in the iterable.
	* `for(Range(10, 20), {(val) print(val)});`

## Special

* `extend(object[, object])`
	* Look at `examples/classes.grey`.
* `instanceof?(object, object)`
	* Returns whether  the first object is an instance of the second object.

## Math

All arithmatic is done using functions which are generally called as infix
functions. All are just regular functions, as `+`, `-`, etc. are valid
identifiers.

* `10 + 20`
* `10 - 20`
* `10 * 20`
* `10 / 20`
* `10 ^ 20` (10 to the power of 20)

## Boolean Logic

Like arithmatic, all logic is done using regular functions.

* `10 == 10`
* `10 != 10`
* `20 > 10`
* `10 < 20`
* `20 >= 10`
* `10 <= 20`
* `true and true`
* `false or true`

## I/O

* `print(string[, string, ...])`:
	* Print to console. Puts a space between each string.
* `read(prompt)`:
	* Read from console.

## Util

* `tostr(variable)`:
	* Convert any variable to a string.
* `tonum(string)`:
	* Convert a string to a number.
* `concat(string, string[, string])`:
	* Concatenate strings.
* `..`: Alias of `concat`.
* `foo ? bar`:
	* Returns the leftmost argument that's not none.

## Methods

None of these are implemented in the new vm yet, working on it.

### Array Methods

* `array.len()`:
	* Get the length of the array.
* `array.push(value)`:
	* Add a variable to the end of the array.
* `array.pop()`:
	* Remove and return the last element of the aray.
* `aray.map(function)`:
	* Run function for every variable in the array.

### String Methods

* `string.len()`:
	* Get the length of the string.
* `string.sub(start[, end])`:
	* Get a substring of the string.
