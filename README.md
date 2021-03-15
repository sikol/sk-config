# `sk-config`

`sk-config` is a C++ configuration parser library based on Spirit X3.

Features:

* Parses the widely-used `named`-style configuration file format which is 
  easy for humans to read and write and familiar to users.
* Useful, human-readable error reporting:
```
line 9: expected a string
      group 0 {};
here -------^
```
* Simple API abstracts away the details of Spirit for most parsers, but
  provides full to the underlying Spirit parser if desired.
* Parse collections of data directly into STL containers.
* Dynamically-generated parser can be extended to support any data type,
  including user-defined types, or even embed complete sub-parsers.

**WARNING**: This library is still in development and almost certainly
contains bugs; if you find any, please file an issue or a PR.

## Example

Given a configuration file like this:

```
user "alice" {
	password "letmein";
};

user "bob" {
	password "securepassword";
};

access {
	allow alice, bob;
};
```

A parser can be created like this:

```c++
#include <sk/config/parse.hxx>
#include <boost/spirit/include/support_istream_iterator.hpp>

struct user {
	std::string name;
	std::string password;
};

struct access_config {
	std::vector<std::string> allow_users;
};

struct config {
	std::vector<user> users;
	access_config access;
};

int main(int argc, char **argv) {
	namespace cp = sk::config::parsers;

	auto grammar = cp::config<config>(

		cp::block<user>("user", &user::name, &config::users,
			cp::option("password", &user::password)),

		cp::block<access_config>("access", &config::access,
			cp::option("allow", &access_config::allow_users))
	);

	config loaded_config;
	try {
		sk::config::parse_file(argv[1], grammar, loaded_config);
	} catch (sk::config::parse_error const &e) {
		std::cerr << e;
		return 1;
	}
}
```

## Supported compilers

`sk-config` is tested with MSVC 19.29 (VS 2019 16.10) and clang-cl 11.0.1
using the Microsoft STL.  It should work with any reasonably-compliant
compiler supporting C++20.

## Configuration file format

The general format of the configuration file is:

```
option [value] [{ [sub-options...] }];
```

For example:

```
# A boolean option: if the option is present, its value is true.
my-option;

# An option with a value.
my-option 42;

# An option with no value but with sub-options.
my-option { sub-option 42; };

# An option with a value and with sub-options.
my-option 42 { sub-option 666; };
```

When an option has both a value and sub-options it usually represents an
object, where the value of the option is the name of the object and the
sub-options are properties of the object.  For example:

```
user "scott" {
	password "tiger";
	groups wheel, operator;
};
```

## Parsers

`sk-config` requires a parser for each type used in the configuration file.
Parsers are provided for most common types.  

### Numeric types

Parsers are provided for `short`, `int`, `long`, `long long` and their unsigned
versions, `float`, `double`, and `long double`.  Numbers must be in base 10.
Floating-point types accept either integers or decimal numbers in the form
`.0`, `0.` or `0.0`.

### Strings

`std::string` can be parsed in one of two ways:

* A base string, consisting of an alphabetic character followed by alphanumeric
  characters, `-` or `_`.  For example, `astring`, `a_string` or `a-string`.
* A quoted string, which is a string enclosed in single quotes or double quotes,
  with embedded escape characters `\t` or `\n`.  For example, `'a long string'`,
  `"string\twith\ttabs"`, `"string with an embedded \" quote mark"`.

## std::vector<T>, std::deque<T>, std::list<T>

Single-value container types are parsed as a comma-separated list of `T`, where 
`T` must have a valid parser.  For example `std::vector<int>` could parse 
`1, 42, 666`.  Duplicate values will create duplicate entries in the vector.

## std::set<T>, std::unordered_set<T>

As above, except that specifying duplicate values raises an error.

## API usage

There are three basic types used to create a parser:

* `sk::config::config`, which is used once at the top level and contains
  all the options and blocks in the configuration.
    * `sk::config::config<T>(elem, elem...)`: A configuration of type `T`
	  which contains the given `elem`s (either blocks or options) at the
	  top level.
* `sk::config::block`, which is used to declare an object which has
  sub-options.
    * `sk::config::block<T>("name", &config_type::value, elem, elem...)`:
	  A block of type `T` which contains the given `elem`s as direct 
	  ancestors.  The block will be used for the option `"name"` in the
	  configuration file.
* `sk::config::option`, which is used to declare an object which does not
  have sub-options.

