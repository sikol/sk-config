# `sk-config`

`sk-config` is a C++ configuration parser library based on Spirit X3.

* [Documentation](https://sk-config.readthedocs.io/en/latest/)

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
    std::map<std::string, user> users;
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

`sk-config` is tested with:

* MSVC 19.28 (VS 16.9) on Windows
* Clang 11.0.1 on Windows
* Clang 10.0.0 on Linux
* GCC 10.2.0 on Linux

It should work with any reasonably-compliant compiler supporting C++20.

## Documentation

* [Here](https://sk-config.readthedocs.io/en/latest/)
