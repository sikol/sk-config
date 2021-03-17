Include files
=============

The easiest way to use the library is to include ``<sk/config.hxx>``,
which includes all functionality. If you prefer to only include 
required functionality, you can use the individual include files.

Core functionality:

* ``<sk/config/parse.hxx>`` - ``parse()`` function
* ``<sk/config/option.hxx>`` - ``option()`` function
* ``<sk/config/block.hxx>`` - ``block()`` function
* ``<sk/config/config.hxx>`` - ``config()`` function
* ``<sk/config/parser_policy.hxx>`` - ``parser_policy`` type

You will also need to include the parser for each type you want to
parse; see the individual pages for each type for the required
include files.
