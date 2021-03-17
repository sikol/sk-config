``std::pair<>``
===============

* Include ``<sk/config/parser/pair.hxx>`` or ``<sk/config.hxx>``.

``std::pair<T1,T2>`` is parsed as an inline or braced list of values
(see :ref:`lists`).  The list must have exactly two values.  Both 
``T1`` and ``T2`` must be supported types.

Example:

.. code-block::

    opt 42, 'xyzzy';  # Could be parsed into std::pair<int,std::string>
