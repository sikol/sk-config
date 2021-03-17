Strings
=======

* Include ``<sk/config/parser/string.hxx>`` or ``<sk/config.hxx>``.

``std::basic_string`` can be parsed in three ways:

* An unquoted string, which is an alphabetic character followed by 
  alphanumeric characters, ``-`` or ``_``.
* A quoted string delimited with either ``'`` or ``"``, with C-style embedded 
  escape sequences.
* A heredoc, starting with ``<<<TOKEN`` and terminated with ``TOKEN``, where
  ``TOKEN`` can be any valid identifier.

Examples:

.. code-block::

    # An unquoted string.
    opt some-string;

    # A quoted string with single quotes.
    opt 'longer string with spaces';

    # A quoted string with double quotes and escapes.
    opt "a \"string\" with a newline\n in it\n";

    # A heredoc string.
    opt <<<END
        This is a long string which can contain 
        embedded newlines.
    END;
