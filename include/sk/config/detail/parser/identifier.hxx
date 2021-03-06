/*
 * Copyright (c) 2019, 2020, 2021 SiKol Ltd.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef SK_CONFIG_PARSER_IDENTIFIER_HXX_INCLUDED
#define SK_CONFIG_PARSER_IDENTIFIER_HXX_INCLUDED

#include <string>

#include <boost/spirit/home/x3.hpp>

namespace sk::config::detail::parser {

    template <typename Char>
    struct identifier : boost::spirit::x3::parser<identifier<Char>> {
        typedef std::basic_string<Char> attribute_type;
        static bool const has_attribute = true;

        template <typename Iterator, typename Context>
        bool parse(Iterator &first, Iterator const &last,
                   Context const &context, boost::spirit::x3::unused_type,
                   attribute_type &attr) const {
            namespace x3 = boost::spirit::x3;

            static auto const grammar =
                x3::lexeme[x3::ascii::alpha >>
                           *(x3::ascii::alnum | x3::char_('-') |
                             x3::char_('_'))];

            return grammar.parse(first, last, context, x3::unused, attr);
        }

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator &first, Iterator const &last,
                   Context const &context, boost::spirit::x3::unused_type,
                   Attribute &attr_param) const {
            attribute_type attr_;
            if (parse(first, last, context, boost::spirit::x3::unused, attr_)) {
                boost::spirit::x3::traits::move_to(attr_, attr_param);
                return true;
            }
            return false;
        }
    };

} // namespace sk::config::detail::parser

namespace boost::spirit::x3 {

    template <typename Char>
    struct get_info<sk::config::detail::parser::identifier<Char>> {
        typedef std::string result_type;
        result_type
        operator()(sk::config::detail::parser::identifier<Char> const &) const {
            return "identifier";
        }
    };

} // namespace boost::spirit::x3

#endif // SK_CONFIG_PARSER_IDENTIFIER_HXX_INCLUDED
