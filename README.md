# `sk-config`

`sk-config` is a declarative parser for `named`-style configuration files
based on Spirit X3.  While there are many different types of configuration
file format (XML, JSON, YAML, TOML, Ini, ...), `named`-style configuration
files have the advantage of being terse, easy to read and write, and very
common among Unix applications.

**WARNING**: This library is still in development and is not completely
functional yet.

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
	std::ifstream config_file(argv[1]);
	boost::spirit::istream_iterator begin(config_file), end;

	namespace cp = sk::config::parsers;

	auto grammar = cp::config<config>(

		cr::block<user>("user", &user::name, &config::users,
			cr::option("password", &user::password)),

		cr::block<access_config>("access", &config::access,
			cr::option("allow", &access_config::allow_users))
	);

	config loaded_config;
	try {
		sk::config::parse(begin, end, grammar);
	} catch (sk::config::parse_error const &e) {
		std::cerr << e;
		return 1;
	}
}
```

## Configuration file format

The general format of the configuration file is:

```
option [value] [{ [option; option; ...] }];
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

When an option has both a value and sub-options it usually defines an
object, where the value of the option is the name of the object and the
sub-options are properties of the object.  For example:

```
user "scott" {
	password "tiger";
	groups wheel, operator;
};
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

