``std::map<>``, ``std::unordered_map<>``
========================================

* Include ``<sk/config.hxx>`` or one or more of the following:
    * ``<sk/config/parser/map.hxx>``
    * ``<sk/config/parser/unordered_map.hxx>``

``std::map<T,U>`` and ``std::unordered_map<T,U>`` are supported for named blocks,
where ``T`` is the type of the block's name, and ``U`` is the type of the
block. The values will be inserted into the map using the block name as the key;
duplicate keys will raise an error.

``std::map<T,U>`` and ``std::unordered_map<T,U>`` are also supported for
option values, where ``T`` and ``U`` are supported types.  The option will be
parsed using a block-like syntax, which allows parsing of blocks whose
elements might not be known at compile time.

For example, to parse a ``map<string, int>``:

.. code-block:: c++

    namespace cfg = sk::config;

    struct config {
        std::map<std::string, int> items;
    };

    auto grammar =
        cfg::config<config>(
            cfg::option("items", &config::items));

    config c;

    cfg::parse(R"(
    items {
        one 1;
        three 3;
        forty-two 42;
    };
    )", grammar, c);
