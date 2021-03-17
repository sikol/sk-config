Lists
=====

* Include ``<sk/config.hxx>`` or one or more of the following:
     * ``<sk/config/parser/deque.hxx>``
     * ``<sk/config/parser/list.hxx>``
     * ``<sk/config/parser/set.hxx>``
     * ``<sk/config/parser/unordered_set.hxx>``
     * ``<sk/config/parser/vector.hxx>``

Most standard container types can be parsed as lists. Two list 
styles are supported: inline (comma-separated) and braced.

Supported containers:

* ``std::vector<T>``
* ``std::deque<T>``
* ``std::list<T>``
* ``std::set<T>``
* ``std::unordered_set<T>``

``T`` must be a supported type.

.. note:: For ``std::set`` and ``std::unordered_set``, duplicate values
          will raise an error.

Inline lists are comma-separated values, for example:

.. code-block::

    opt 1, 42, 666;
    opt 42, 'a string'; # List of variant<int,std::string>
    opt 1, 1, 2; # Error if parsing into set<> or unordered_set<>


Braced lists are formatted similar to blocks:

.. code-block::

    opt {
        1; 
        42;
        666;
    };

Unlike inline lists, braced lists can be nested for multi-dimensional containers.

Both list styles can be mixed in a single value:

.. code-block::

    int-value {1; 2; 3;}, 
              {4; 5; 6;}, 
              {7; 8; 9;}; # std::vector<std::vector<int>>

List styles can be configured in the parser policy (see :ref:`parser policy`).

.. code-block:: c++

    struct no_inline_lists : sk::config::parser_policy {
        static constexpr bool allow_inline_lists = false;
        static constexpr bool allow_braced_lists = true;
    };

