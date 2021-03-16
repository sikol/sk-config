# `sk-config`

`sk-config` is a C++ configuration parser library based on Spirit X3.

Features:

* Parses the widely-used `named`-style configuration file format which is 
  easy for humans to understand and familiar to users.
* Details of the file format can be customised via parser policies or
  custom parsers.
* Parse configuration data directly into STL containers and user-defined
  structs.
* Useful, human-readable error reporting:
```
in .\sample\sample1.conf, line 10: expected a string
      group 0 {
here -------^
```
* Simple API abstracts away the details of Spirit for most parsers.
* ... but provides full access to the underlying Spirit parser if desired.
* Dynamically-generated parser can be extended to support any data type,
  including user-defined types, or even embed complete Spirit sub-parsers.

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
#include <sk/config.hxx>

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
    namespace cfg = sk::config;

    auto grammar = cfg::config<config>(

        cfg::block<user>("user", &user::name, &config::users,
            cfg::option("password", &user::password)),

        cfg::block<access_config>("access", &config::access,
            cfg::option("allow", &access_config::allow_users))
    );

    config loaded_config;
    try {
        cfg::parse_file(argv[1], grammar, loaded_config);
    } catch (cfg::parse_error const &e) {
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

Single-line comments are introduced with `#`, and multi-line comments are
confixed with `/* ... */`.

## Customising the parser

The parser can be customised by providing a parser policy to `parse()`.  For
example, say we want options to be formatted as `opt: value`, and instead of
terminating options with a semicolon, we want to allow newline as a 
terminator.  We can express this policy like this:

```
    namespace x3 = boost::spirit::x3;
    namespace cfg = sk::config;

    struct my_policy : cfg::parser_policy {
        // Change the parser that will be used to parse end-of-option.
        auto option_terminator() const {
            return x3::eol;
        }

        // Change the parser that will be expected between an option
        // and its value.
        auto option_separator() const {
            return x3::lit(':');
        }
    };
```

Now we can use our custom policy when parsing:

```
    cfg::parse<my_policy>(filename, grammar, ret);
```

This will parse configuration files that look like this:

```
user "fred" {
    uid: 0
    full-name: "Fred Smith"
}
```

## Include files

The easiest way to use the library is to include `<sk/config.hxx>`, which
includes all functionality.  If you prefer to only include required
functionality, you can use the individual include files.

Core functionality:

* `<sk/config/parse.hxx>` - `parse()` function
* `<sk/config/option.hxx>` - `option()` function
* `<sk/config/block.hxx>` - `block()` function
* `<sk/config/config.hxx>` - `config()` function

Basic parsers:

* `<sk/config/parser/numeric.hxx>` - integral and floating point parsers
* `<sk/config/parser/string.hxx>` - `std::basic_string` parser

Standard container parsers:

* `<sk/config/parser/deque.hxx>`
* `<sk/config/parser/list.hxx>`
* `<sk/config/parser/set.hxx>`
* `<sk/config/parser/tuple.hxx>`
* `<sk/config/parser/unordered_set.hxx>`
* `<sk/config/parser/variant.hxx>`
* `<sk/config/parser/vector.hxx>`

## Supported types

`sk-config` requires a parser for each type used in the configuration file.
Parsers are provided for most common built-in types and STL containers.

Due to how Spirit works, parsing into `std::any` or similar type-erasing
containers is _not_ supported; the parser needs to know ahead of time what
type(s) are being parsed, and cannot enumerate all parsable types.  However,
parsing into `tuple`- or `variant`-like types is supported, since all types
are known.

### Integers

Any type that meets the requirements of `std::integral` is parsed using the
standard Spirit integer parser.  Numbers must be in base 10, and base prefixes
(`0x`, `0`) are not supported. 

Examples:
* `opt 1;`
* `opt -42;`

### Real numbers

Any type that meets the requirements of `std::floating_point` is parsed using
the standard Spirit real number parser.  Leading and trailing dots are both
supported, and integers are accepted, so `42` can be parsed as `42.0`.

Examples:
* `opt 42.5;`
* `opt -.333;`
* `opt 2;`

### Booleans

`bool` is parsed differently from other types since it has no value.  Instead,
specifying the option is treated as `true`.

Example:
* `my-option;`

### Strings

`std::basic_string` can be parsed in three ways:

* An unquoted string, which is an alphabetic character followed by alphanumeric
  characters, `-` or `_`.
* A quoted string delimited with either `'` or `"`, with C-style embedded escape
  sequences.  
* A heredoc, starting with `<<<TOKEN` and terminated with `TOKEN`, where `TOKEN`
  can by any valid identifier.

Examples:

* `opt some-string;`
* `opt 'longer string with spaces';`
* `opt "a \"string\" with a newline\n in it\n";`
* ```
opt <<<END
    This is a long string which can contain 
    embedded newlines.
END;
```

### Lists

Most standard container types can be parsed as comma-separated lists:

* `std::vector<T>`
* `std::deque<T>`
* `std::list<T>`
* `std::set<T>`
* `std::unordered_set<T>`

`T` must be a supported type.

For the set types, duplicate values will raise an error.

Examples:

* `opt 1, 42, 666;`
* `opt 42, 'a string';  # List of variant<int,std::string>`
* `opt 1, 1, 2; # Error if parsing into set<> or unordered_set<>`

### std::variant

`std::variant<Ts...>` is parsed as a single value.  All types in `Ts`
must be supported types.

Due to a quirk of the parser, `Ts` should not contain both integral and
floating-point types (for example, `variant<int,float>`) or parsing will
not working correctly.

### std::tuple

`std::tuple<Ts...>` is parsed as a comma-separated list of its contained
types.  All types in `Ts` must be supported types.

### Custom parsers

A custom Spirit parser can be passed to `option()` to override the default
parser for that type.  For example, to parse an hexadecimal integer
prefixed with "0x":

```c++
  cfg::option("my-option", &S::hex_val, x3::lexeme["0x" > x3::hex]);
```

The attribute type of the parser must be assignable to the struct member.

### Symbols

Spirit's built-in symbol table support can be used to parse options that
contain a list of fixed string values.  

For example, an option which can be either "on" or "off":

```
my-option off;
```

Could be parsed like this:

```c++
struct test_config {
    bool my_option;
};

x3::symbols<bool> syms;
syms.add("on", true)("off", false);
// ...
    cfg::option("my-option", &test_config::my_option, syms);
```

Symbols can also be used to parse a list of values:

```c++
struct test_config {
    std::vector<int> my_options;
};

x3::symbols<int> syms;
syms.add("one", 1)("two", 2)("three", 3);
// ...
    cfg::option("my-option", &test_config::my_options, syms % ',');
```

```
my-option one, three;  # test_config.my_options == {1, 3}
```

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

