``std::map<>``
==============

* Include ``<sk/config.hxx>`` or one or more of the following:
    * ``<sk/config/parser/map.hxx>``
    * ``<sk/config/parser/unordered_map.hxx>``

``std::map<T,U>`` and ``std::unordered_map<T,U>`` are supported for named blocks
(only), where ``T`` is the type of the block's name, and ``U`` is the type of the
block. The values will be inserted into the map using the block name as the key;
duplicate keys will raise an error.
