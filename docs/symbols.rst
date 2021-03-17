Symbols
=======

Spirit's built-in symbol table support can be used to parse options
that contain a list of fixed string values.

For example, an option which can be either "on" or "off":

.. code-block ::

    my-option off;

Could be parsed like this:

.. code-block:: c++

    struct test_config {
        bool my_option;
    };

    x3::symbols<bool> syms;
    syms.add("on", true)("off", false);
    // ...
        cfg::option("my-option", &test_config::my_option, syms);

Symbols can also be used to parse a list of values:

.. code-block:: c++

    struct test_config {
        std::vector<int> my_options;
    };

    x3::symbols<int> syms;
    syms.add("one", 1)("two", 2)("three", 3);
    // ...
        cfg::option("my-option", &test_config::my_options, syms % ',');

.. code-block::

    my-option one, three;  # test_config.my_options == {1, 3}
