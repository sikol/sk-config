Booleans
========

* Include ``<sk/config/parser/numeric.hxx>`` or ``<sk/config.hxx>``.

When used as a bare type, ``bool`` is parsed differently from other types 
since it has no value; instead, specifying the option is treated as true.

Example:

.. code-block::

    my-option;

When ``bool`` is part of a list of values (e.g. ``std::tuple<>``) it must
be written explicitly, in which case the values ``true`` and ``false`` are used.

Example:

.. code-block::

    my-option 42, true; # Valid for std::tuple<int,bool>
