# Create a PHP Extension Cheat Sheet

## C and Zend API

The "native" way is to do it like in the PHP source, in C, with the Zend API to spice things up.

The documentation for the Zend API (all C macros you can use to "simplify" the extension's code), about the Zend Engine internal workings and how to build an extension can be found here :

- [PHP at the Core: A Hacker's Guide](http://php.net/manual/en/internals2.php) (PHP manual)
- http://www.phpinternalsbook.com ([GitHub](https://github.com/phpinternalsbook/PHP-Internals-Book))

Note that both resources are largely WIP in some areas. But if you are serious about building native extensions, you read it all.

Here is a breakdown of links toward the main subjects :

- PHP lifecycle: [Book](http://www.phpinternalsbook.com/php7/extensions_design/php_lifecycle.html)
- Memory Management: [PHP manual](https://secure.php.net/manual/en/internals2.memory.management.php) | Book: [here](http://www.phpinternalsbook.com/zvals/memory_management.html) and [there](http://www.phpinternalsbook.com/php7/memory_management.html)
- Zval and working with variables: [PHP manual](https://secure.php.net/manual/en/internals2.variables.intro.php) | [Book](http://www.phpinternalsbook.com/php7/internal_types/zvals/basic_structure.html)
- Arrays: PHP manual - [Hashtable](https://secure.php.net/manual/en/internals2.variables.tables.php) and [Arrays](https://secure.php.net/manual/en/internals2.variables.arrays.php) | [Book](http://www.phpinternalsbook.com/hashtables.html)
- Strings: [Book](http://www.phpinternalsbook.com/php7/internal_types/strings/zend_strings.html)
- Resources: [Book](http://www.phpinternalsbook.com/php7/internal_types/zend_resources.html)
- Functions: [PHP manual](https://secure.php.net/manual/en/internals2.funcs.php) | [Book](http://www.phpinternalsbook.com/php7/extensions_design/php_functions.html)
- Classes: [Book](http://www.phpinternalsbook.com/classes_objects.html)
- Extension structure: [PHP manual](https://secure.php.net/manual/en/internals2.structure.php) | [Book - Extension skeleton](http://www.phpinternalsbook.com/php7/extensions_design/extension_skeleton.html)

Typically a PHP extension is around 10 times faster than the "same" code in PHP userland.


## C++ and PHP-CPP

PHP-CPP is a C++ library that is a nice wrapper around the Zend API. It can do everything the Zend Engine allows to do. It is maintained by a company that used it for their own products but it's free, open-source and has a full documentation.

- [Documentation](http://www.php-cpp.com/documentation/)
- [GitHub](https://github.com/CopernicaMarketingSoftware/PHP-CPP)

From their documentation, we can read that a simple bubble sort algorithm was 10 times faster when written in a PHP extension rather than in the PHP userland.




## Zephir

Zephir is a high level language that compiles into a PHP extension.

- [Documentation](https://docs.zephir-lang.com/en/latest/index.html)
- [GitHub](https://github.com/phalcon/zephir)

### Zephir syntax cheat sheet

The syntax is very similar to PHP's, so I only mention here what is new/different :

Property declaration shorthand :
```
private myProperty = 10 {
    set, get
};
```
Is the same as declaring the property and both `getMyProperty()` and `setMyProperty(value)`.

Class constants can only be called via the class name: `MyClass::THE_CLASS_CONSTANT`.

Type hint in arguments and returned value : `function theFunc(array! input, const int size = 0)`.  
The `!` after the type forces the argument to be of that type otherwise casting is attempted.  
The `const` keyword can be used inside functions body and arguments

Possibility to return `void`, and/or one or several types: `function theFunc() -> string | bool`.

Variable declaration without strict type uses the `var` keyword: `var a;`.  
These can be assigned a value of any of the PHP's types. Var keyword not necessary in arguments (but possible).

Types can also be specified, like in C.  
For these variables, their types can not change later (value can still change if not const).
```
int theNumber = 0;
array theArray = [];
```
Available types : `bool`, `char`, `string`, `int`, `long`, `float`, `double`, `array`.

The `unsigned` keyword is also available for `char`, `int` and `long`: `unsigned long veryBigNumber = 999999999;`.
Short type  notation is possible: `uchar`, `uint`, `ulong`.

When assigned to a value of a different type, casting is attempted. Throws a compiler exception when types are incompatible.

When assigning value to a statically typed variable from a source that may not be (var variable, PHP functions, array values), it generally needs to be explicitely casted.  
But rules are not super clear, compiler will tell you when he wants that.

For objects, the type can be specified during assignation, not variable creation.
```
var o = new TheClass(); // or even   var o = <TheClass!> new TheClass();
var o2, o3;
o2 = <TheClass> o; // tells the compiler to think that o2 is an instance of TheClass
o3 = <TheClass!> o; // tells the compiler to actually check that o is an instance of TheClass
```
Same notation can be used for arguments and retuned values: `function theFunc(<TheClass> o) -> <TheClass>`.

Parsing of variables inside double quoted strings is not supported, use concatenation instead.

Variable variable name:
```
float price = 0.0;
string name = "price";
let {name} = 10.2;
```
Works the same with properties, functions and classes: `this->{propName}  this->{methodName}()  {funcName}()  new {className}()`;



__MOST IMPORTANTLY, you MUST precede variables with the keyword `let` to be able to change their value__:
```
var theVar = 1; // declaration
let theVar = 2; // assignation of another value
theVar = 3; // throws a parse error
// same with properties:
let this->theProperty = "new value";
```

Parenthesis are optional in control structures.  
No standard `for(;;)`.

```
string aString = "aString";
char aChar;
for aChar in aString { }

int key, value;
var _array = [0, 1, 2, 3, 4];
for key, value in _array { }

// reverse keyword reverse the array
for value in reverse _array { }

// "standard" for
for i in range(0, 9) { }

loop { }
// is the same as
while true { }
```

Syntactic sugar operators:
```
if isset theVar { } // same as isset()
if empty theVar { } // same as empty()
if typeof theVar == "string" { } // same as gettype()

var theVar;
if fetch theVar, theArray[theKey] {
    // if theKey exists in theArray, theVar is populated with the corresponding value
    // and fetch returns true (false otherwise and theVar is left alone)
}
```

Named parameters:
```
function theFunc(array a, int size) { }
theFunc([], 0); // standard way
theFunc(size: 1, a: [1]);
```

Shorthand for closures:
```
needsAClosureAsParam( number => number * number );
// is the same as
needsAClosureAsParam( function(number) { return number * number; } );
```

The `deprecated` method modifier automatically throws a `E_DEPRECATED` error if the method is called.

Require a php script from the userland: `require "the/path.php";`.

PHP global constants are not accessible, but the Surperglobals are: `string queryStr = _SERVER["query_string"];`

You can call any existing functions from PHP core, any extensions of even the userland.

Exceptions at runtime allows you to know in which `.zep` script the error occurred. PHP errors don't.

### Installation

At the time of this writing, Zephir still needs PHP v7.0.x (Zend Module Api No: 20151012) and the extensions created with it will only works with this version of PHP.

So you need to make sure the `php` command point to the correct PHP version (I had to update its symlink like so: `/usr/bin/php > /usr/bin/php7.0`).

First install anywhere the parser: [PHP Zephir parser](https://github.com/phalcon/php-zephir-parser)

```
sudo apt-get install php7.0-dev gcc make re2c autoconf
git clone git://github.com/phalcon/php-zephir-parser.git
cd php-zephir-parser
sudo ./install
```

This creates a PHP extension, so you need to update the __relevant__ `php.ini`. If you want to use it to compile  a Zephir project, you need to use the `php.ini` for PHP CLI.

```
[Zephir Parser]
extension=zephir_parser.so
```

Then install anywhere Zephir itself :

```
git clone git://github.com/phalcon/zephir.git
cd zephir
sudo ./install
```

Anywhere on your disk, initiate a Zephir project : `zephir init extension_name`

Put your `.zep` scripts in the `extension_name/extension_name` folder, following the namespace's hierarchy.

When ready, compile with `zephir build` or `zephir compile`

If all goes well you just need to update the correct `php.ini` (the one used by your site, so not necessarily the same as before). Obviously, you only need to do that the first time.

```
extension=extension_name.so
```

If you got a parser error but no indication of the line where the error occurs, then the parser is missing.

If you got an error about missing functions, check which PHP extension the function is part of and make sure that extension is installed (for some reason my installation was missing the PHP-XML extension, for instance).

If you got an error about incompatible Zend Module Api Number, the `php` command do not point toward PHP 7.0.

### PHP to Zephir

The syntax of PHP and Zephir are close enough that you can automatically go from PHP to Zephir:

- [php-to-zephir](https://github.com/fezfez/php-to-zephir)
- [php2zep](https://github.com/springlchy/php2zep)

For both it is expected to need to pass over the generated code to make sure it is the most optimized.

### Performance

From various personal benchmarks, an extension created with Zephir will be at least 2 times faster than the same code in the PHP userland. Which is great considering the low amount of time it takes to go from PHP to Zephir, but it's still far slower than native extensions.

Two frameworks have been built with Zephir: [Phalcon](https://phalconphp.com) and [Ice](https://www.iceframework.org). From a benchmark of many PHP frameworks ([GitHub](https://github.com/kenjis/php-framework-benchmark) and [including Ice](https://www.iceframework.org/doc/introduction/benchmark)) we can see that they perform much better than Symphony or Laravel but they are still championed by some lightweight plain-PHP frameworks.
