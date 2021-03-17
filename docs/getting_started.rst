Getting started
===============

Before you can parse a configuration file, you need to define the
C++ types that will store the loaded configuration.  These are
normal structs (or classes) with a member field for each configuration
option.  Most common types are supported (see `:ref:types`) and
if needed, support for additional types can be added.

Our sample configuration file will look like this:

.. code-block::

    user "root" {
      uid 0;
      gid 0;
    };

    group "wheel" {
      gid 0;
      member root;
    };

We have two "objects" in this configuration file, ``user`` and
``group``, so we'll define structs for both of them:

.. code-block:: c++

    struct user {
        int uid, gid;
        std::string username;
    };

    struct group {
        std::string name;
        int gid;
        std::set<std::string> members;
    };

We also need to define our top-level configuration object.  Since
our ``user`` and ``group`` blocks are both "named" blocks, meaning
they appear as ``user "name" { ... }`` rather than just ``user { ... }``,
we'll use maps for the top-level configuration object so we can
look up objects by name.

.. code-block:: c++

    struct config {
        std::map<std::string, user> users;
        std::map<std::string, group> groups;
    };

Now we need to create the *grammar* for the configuration, which
tells the parser how to turn the configuration file into objects.
We do this using the three basic primitives ``config()``, ``block()``
and ``option()``:

.. code-block:: c++

    namespace cfg = sk::config;

    // The type of the grammar is typically an extremely long 
    // template type, so we use ``auto`` to avoid having to write 
    // it out by hand.
    auto grammar = 
        // Start by introducing our top-level configuration object
        // using config().  This should be used exactly once at the
        // top level of the grammar.
        cfg::config<config>(

            // Define our "user" block using block().  For a named
            // block, block() takes three arguments:
            //
            //  - The label that will represent this object in the
            //    configuration file, which for our configuration file
            //    is "user".  In nearly all cases this will be a string,
            //    but it can be any type.
            //
            //  - A pointer to the member that will be used to store
            //    the name of the block; in our case, that's 
            //    user::username.
            //
            //  - A pointer to the member of the parent block that this
            //    block will be stored in.  Our parent block is the
            //    top-level "config" struct, so we want the users to be
            //    stored in config::users.  sk-config will automatically
            //    detect that config::users is a map<string, user> and
            //    place objects in the map using the username as the key.

            cfg::block<user>("user", &user::username, &config::users,

            // Define our two options, which are "uid" and "gid", by calling
            // cfg::option().  We provide the name of the option, and a
            // pointer to the member the option's value should be stored in.

                             cfg::option("uid", &user::uid),
                             cfg::option("gid", &user::gid)),

            // Now do the same for the "group" block.

            cfg::block<group>("group", &group::name, &config::groups,
                              cfg::option("gid", &group::gid),

                              // group::members is a set<string>; so sk-config
                              // will parse this option as a list of strings.
                              cfg::option("member", &group::members)));

Now we have our grammar, all we need to do is call ``parse_file()``:

.. code-block:: c++

    config loaded_config;

    try {
        cfg::parse_file("my_app.conf", grammar, loaded_config);
    } catch (cfg::parse_error const &e) {
        std::cerr << e;
        std::exit(1);
    }

``parse_file()`` will load the provided filename and parse it into the
``loaded_config`` object.  If the configuration file contains an error,
we can print the ``parse_error`` object to display a nicely-formatted
error message.

That's it!  For a compilable example, see the
`full version <https://github.com/sikol/sk-config/blob/master/sample/main.cxx>`_
of this sample parser.
