Numeric types
=============

* Include ``<sk/config/parser/numeric.hxx>`` or ``<sk/config.hxx>``.

Integers
--------

Any type that meets the requirements of ``std::integral`` is parsed using
the standard Spirit integer parser. Numbers must be in base 10, and base
prefixes (0x, 0) are not supported.

Examples:

.. code-block::

    opt 1;
    opt -42;

Real numbers
------------

Any type that meets the requirements of ``std::floating_point`` is parsed
using the standard Spirit real number parser. Leading and trailing dots are
both supported, and integers are accepted, so 42 can be parsed as 42.0.

Examples:

.. code-block::

    opt 42.5;
    opt -.333;
    opt 2;
