API reference
=============

The basic sk-config interface is provided by three functions: ``config()``,
``block()`` and ``option()``.  All of these are in the namespace ``sk::config``.

.. note:: The function prototypes contain a large number of template arguments,
          because they are used with pointers-to-member.  In practice, most of
          these template arguments can be inferred and do not need to be
          specified.

``config<T>()``
---------------

* **Defined in**: ``<sk/config/config.hxx>`` or ``<sk/config.hxx>``.

**Prototype**:

.. code-block:: c++

    template<typename Options...>
    auto config(Options&&... options);

**Description**:

``config<T>()`` creates a new configuration parser for the type ``T``, which
should be the root object of the configuration.  Its arguments should be a
list of blocks or options created by the ``block()`` or ``option()`` functions.

**Example**:

.. code-block:: c++

    namespace cfg = sk::config;

    struct my_config { ... };
    auto parser = cfg::config<my_config>(...);

``block<T>()``
--------------

* **Defined in**: ``<sk/config/block.hxx>`` or ``<sk/config.hxx>``.

**Prototype**:

.. code-block:: c++

    template<typename BlockType, 
             typename ParentType, 
             typename ParentValueType, 
             typename... Options>
    auto block(auto label,
               ParentValueType ParentType::*member,
               Options &&... options);

    template<typename BlockType,
             typename NameType,
             typename ParentType
             typename ParentValueType,
             typename... Options>
    auto block(auto label,
               NameType BlockType::*name,
               ParentValueType ParentType::*member,
               Options &&... options);

**Arguments**:

* ``label``: The label that will be used to introduce this block type in the
  configuration file.  This is usually an ``std::string``, although it
  can be any type for which a parser is available. If the label is 
  ``"myblock"``, then the configuration file should contain ``myblock { ... };``.
* ``member``: The member field of the *parent* type which this block will
  be stored in.  This can be ``BlockType`` (for a singleton block),
  ``std::vector<BlockType>`` (for a block that can be specified multiple
  times), etc.  If the block has a name field, this can also be an
  ``std::map<NameType, BlockType>``.
* ``name``: For named blocks, this is the member field of the *block* type
  that the block's name will be stored in.  If the name is ``&T::name``,
  then ``myblock "name" { ... };`` will store ``"name"`` in ``T::name``.
* ``options``: The options or nested blocks that can be contained in
  this block.

**Description**:

``block()`` creates a parser for a configuration block called ``label`` 
which is parsed into the type ``B``.  The parsed block will be stored in the
member variable ``member``, which must be a member of the parent block or
(if this is a top-level block) the top-level configuration object.

The first form is used for anonymous blocks, while the second form is used for 
named blocks.  For named blocks, the block name will be stored in the member
``name``.

**Example**:

Create an anonymous block:

.. code-block:: c++

    namespace cfg = sk::config;

    struct my_block { ... };
    struct my_config {
        my_block block;
    };

    auto parse = 
        cfg::config<my_config>(
            cfg::block<my_block>("my-block", &my_config::block,
            ...));

Create a named block:

.. code-block:: c++

    namespace cfg = sk::config;

    struct my_block { 
        std::string block_name;
    };

    struct my_config {
        my_block block;
    };

    auto parse = 
        cfg::config<my_config>(
            cfg::block<my_block>("my-block", 
                                 &my_block::block_name,
                                 &my_config::block,
            ...));


``option<T>()``
---------------

* **Defined in**: ``<sk/config/option.hxx>`` or ``<sk/config.hxx>``.

**Prototype**:

.. code-block:: c++

    template<typename ParentType, 
             typename ValueType>
    auto option(auto label, 
                ValueType ParentType::*member);

    template<typename ParentType, 
             typename ValueType,
             typename Parser>
    auto option(auto label, 
                ValueType ParentType::*member,
                Parser p);

**Arguments**:

* ``label``: The label that will be used to introduce this option in the
  configuration file.  This is usually an ``std::string``, although it
  can be any type for which a parser is available. If the label is 
  ``"my-option"``, then the configuration file should contain ``my-option ...;``.
* ``member``: The member field of the parent type which this block will
  be stored in.  This can be ``ValueType`` (for a singleton option),
  ``std::vector<ValueType>`` (for an option that can be specified multiple
  times), etc.  
* ``p``: The parser that will be used to parse the option's value.

``parse()``
-----------

* **Defined in**: ``<sk/config/parse.hxx>`` or ``<sk/config.hxx>``.

**Prototype**:

.. code-block:: c++

    template <typename Policy = parser_policy, typename Iterator>
    bool parse(Iterator first, 
               Iterator last,
               auto const &grammar, 
               auto &ret,
               std::string const &filename = "");

    template <typename Policy = parser_policy>
    bool parse(std::ranges::range auto const &range, 
               auto const &grammar, 
               auto &ret,
               std::string const &filename = "");

    template <typename Policy = parser_policy>
    bool parse(char const *string, 
               auto const &grammar, 
               auto &ret,
               std::string const &filename = "");

**Description**

Parse a configuration string and return the loaded configuration.

**Arguments**

* ``first``, ``last``: An iterator pair containing the configuration
  string; the iterator's value should be a char-like type.
* ``range``: An `std::ranges::range`` containing the configuration
  string; the range's value should be a char-like type.
* ``string``: A nul-terminated C string containing the configuration.
* ``grammar``: The grammar that will be used to parse the configuration.
* ``ret``: Reference to the top-level configuration object which will
  be populated with the configuration data.
* ``filename``: The name of the file which the configuration was
  loaded from; this is used in error messages.

**Return value**

``parse()`` always returns ``true``.

``parse_file()``
----------------

* **Defined in**: ``<sk/config/parse.hxx>`` or ``<sk/config.hxx>``.

**Prototype**:

.. code-block:: c++
    template <typename Policy = parser_policy>
    bool parse_file(std::filesystem::path filename,
                    auto const &grammar,
                    auto &ret);

**Description**

Parse a configuration file and return the loaded configuration.

**Arguments**

* ``filename``: Name of a configuration file that will be parsed.
* ``grammar``: The grammar that will be used to parse the configuration.
* ``ret``: Reference to the top-level configuration object which will
  be populated with the configuration data.

**Return value**

``parse()`` always returns ``true``.
