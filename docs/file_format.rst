Configuration file format
=========================

The general format of the configuration file is:

.. code-block::

    option [value] [{ [sub-options...] }];

For example:

.. code-block::

    # A boolean option: if the option is present, its value is true.
    my-option;

    # An option with a value.
    my-option 42;

    # An option with no value but with sub-options.
    my-option { sub-option 42; };

    # An option with a value and with sub-options.
    my-option 42 { sub-option 666; };

When an option has both a value and sub-options it usually represents
an object, where the value of the option is the name of the object
and the sub-options are properties of the object. For example:

.. code-block::

    user "scott" {
        password "tiger";
        groups wheel, operator;
    };

Single-line comments are introduced with ``#``, and multi-line comments
are confixed with ``/* ... */``.

.. code-block::

    # This is a single line comment.
    /* This is a multi-line comment.  It can span multiple
     * lines, but comments cannot be nested. */
