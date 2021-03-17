sk-config: extensible configuration file parser for C++
=======================================================

.. toctree::
   :hidden: 
   :maxdepth: 2
   :caption: Contents:

   file_format.rst
   includes.rst
   types.rst
   api.rst
   custom_parser.rst
   parser_policy.rst

sk-config is a configuration file parser for C++.  It parses *named*-style
configuration files, which are easy for humans to read and write, and are
familiar to users.

sk-config is based on the Spirit X3 parser framework.  For most use-cases,
it completely hides the details of Spirit and provides a simple API that
allows parsers to be created quickly and easily.  For more advanced use-cases,
it allows the user to provide custom Spirit parsers where needed, and it
can even be embedded into a larger Spirit parser.

Example
-------

Suppose we have a configuration file which looks like this:

.. code-block::

    user "alice" {
        password "letmein";
    };

    user "bob" {
        password "securepassword";
    };

    access {
        allow alice, bob;
    };

We can create a parser like this:

.. code-block:: c++

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

Supported compilers
-------------------

sk-config is tested with:

* MSVC 19.28 (VS 16.9) on Windows
* Clang 11.0.1 on Windows
* Clang 10.0.0 on Linux
* GCC 10.2.0 on Linux

Indices and tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
