``std::tuple<>``
================

* Include ``<sk/config/parser/tuple.hxx>`` or ``<sk/config.hxx>``.

``std::tuple<Ts...>`` is parsed as an inline or braced list of values
(see :ref:`lists`).  The number of elements in the list must match the
number of elements in the tuple.  All types in ``Ts`` must be supported
types.


Example:

.. code-block::

    opt 42, 'xyzzy';  # Could be parsed into std::tuple<int,std::string>
