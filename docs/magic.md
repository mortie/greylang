# Magic Names

In grey, methods prefixed with $ are reserved for magic functions
which affect the language in various ways, like python's functions
surrounded by \_\_ (e.g python's \_\_init\_\_ is $init in grey).

In general, you should never call a magic function yourself, instead trigger
them through their intended use.

In this document, `Foo` will refer to a class, and `foo` an instance of `Foo`.

## Special

* `$init`:
	* A class constructor.
	* `foo := Foo()` calls `Foo.$init()` with `self` set to a new object.
* `$iter`:
	* Create an iterator.
	* `it := foo.$iter()` - here, `it` should be a function
	* `print for foo` calls `foo.$iter()` to get a function,
	  then calls that function until it returns `none`
* `$call`:
	* Called when the object is called like a function.
	* `foo()` calls `foo.$call()`
	* If an object contains both `$init` and `$call`, `$call` will
	  be called if it's an instance of a class, and `$init` if it's
	  not an instance.

## Utility

* `$contains?`
	* Whether the object contains a value
	* `10 in? Range(5, 15)` does `Range(5, 15).$contains?(10)`

## Operator Overload

All these functions should return a new instance if possible, instead of
mutating `foo`.

* `$add`: Called with `+`.
	* `foo + bar` does `foo.$add(bar)`
* `$sub`: Called with `-`.
	* `foo - bar` does `foo.$sub(bar)`
* `$mul`: Called with `\*`.
	* `foo \* bar` does `foo.$mul(bar)`
* `$div`: Called with `/`.
	* `foo / bar` does `foo.$div(bar)`
* `$pow`: Called with `^`.
	* `foo ^ bar` does `foo.$pow(bar)`
