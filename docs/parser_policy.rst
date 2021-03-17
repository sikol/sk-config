.. _parser policy:

Customising the parser with parser_policy
=========================================

The behaviour of the parser can be changed by providing a custom
parser policy to the ``sk::config::parse()`` function.

For example, say we want options to be formatted as ``opt: value``,
and instead of terminating options with a semicolon, we want to
allow newline as a terminator. We can express this policy like this:

.. code-block:: c++

    namespace x3 = boost::spirit::x3;
    namespace cfg = sk::config;

    struct my_policy : cfg::parser_policy {
        // Change the parser that will be used to parse end-of-option.
        static constexpr auto option_terminator() {
            return x3::eol;
        }

        // Change the parser that will be expected between an option
        // and its value.
        static constexpr auto option_separator() {
            return x3::lit(':');
        }
    };

Now we can use our custom policy when parsing:

.. code-block:: c++

    cfg::parse<my_policy>(filename, grammar, ret);

This will parse configuration files that look like this:

.. code-block::

    user "fred" {
        uid: 0
        full-name: "Fred Smith"
    }

``parser_policy`` reference
---------------------------

.. code-block:: c++

    struct parser_policy {
        /*
         * Return the parser used to separate option names from their
         * values.  By default this is eps, meaning no separator.
         */
        static constexpr auto option_separator() {
            return boost::spirit::x3::eps;
        }

        /*
         * Return the parser that should appear after an option's value.
         * 
         * This parser is always invoked with lexeme[], so it can match
         * whitespace.
         */
        static constexpr auto option_terminator() {
            return boost::spirit::x3::lit(';');
        }

        // Whether to accept inline lists, `val, val, val...`
        static constexpr bool allow_inline_lists = true;

        // Whether to accept braced lists, { val; val; val...; }
        static constexpr bool allow_braced_lists = true;

        /*
         * The parser to confix a braced element.
         */
        static constexpr auto braced(auto const& v) {
            return '{' > v > '}';
        }
    };