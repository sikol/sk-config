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

	auto loaded_config = sk::config::parse(begin, end, grammar);
}
```