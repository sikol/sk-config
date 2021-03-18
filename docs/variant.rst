``std::variant<>``
==================

* Include ``<sk/config/parser/variant.hxx>`` or ``<sk/config.hxx>``.

``std::variant<Ts...>`` is parsed as a single value. All types in
``Ts`` must be supported types.  If one of ``Ts`` is `bool`, the
boolean value must be explicitly specified.

.. note:: Due to a quirk of the parser, ``Ts`` should not contain
          both integral and floating-point types (for example, 
          ``variant<int,float>``) or parsing will not work correctly.

Example:

.. code-block::

    opt 42;  # Could be parsed into std::variant<int,std::string>
    opt 'forty-two'; # Could also be parsed into std::variant<int,std::string>
    opt; # Not valid for std::variant<int,bool>
    opt true;  # OK for std::variant<int,bool>