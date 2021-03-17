Custom parsers
==============

A custom Spirit parser can be passed to ``option()`` to override the 
default parser for that type. For example, to parse an hexadecimal
integer prefixed with "0x":

.. code-block:: c++

    cfg::option("my-option", &S::hex_val, x3::lexeme["0x" > x3::hex]);

Every Spirit parser has an automatically-determined attribute type,
such as ``int`` or ``std::vector<string>``, which is determined by
Spirit's `compound attribute rules <https://www.boost.org/doc/libs/1_75_0/libs/spirit/doc/x3/html/spirit_x3/quick_reference/compound_attribute_rules.html>`_.

The attribute type of the custom parser must be assignable to the
struct member type.  Supported assignments are:

* From ``std::vector<T>``:
    * To ``std::vector<T>``
    * To ``std::deque<T>``
    * To ``std::list<T>``
    * To ``std::set<T>``
    * To ``std::unordered_set<T>``
* From ``std::variant<Ts>`` to ``std::variant<Ts>``
* From ``std::tuple<Ts>`` to ``std::tuple<Ts>``
* From ``std::pair<T,U>`` to ``std::pair<T,U>``
* From ``std::tuple<T,U>`` to ``std::pair<T,U>``
* From ``std::basic_string<C>`` to ``std::basic_string<C>``
* From any built-in type to any other built-in type, according to
  C++ assignment rules.
