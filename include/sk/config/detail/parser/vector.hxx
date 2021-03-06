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

#ifndef SK_CONFIG_DETAIL_VECTOR_PARSER_HXX_INCLUDED
#define SK_CONFIG_DETAIL_VECTOR_PARSER_HXX_INCLUDED

#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>

#include <sk/config/parser_policy.hxx>

namespace sk::config::detail::parser {

    /*
     * Parse a list of values.  Depending on the parser policy, we accept
     * two different list formats.
     *
     * Inline list:
     *   option 1, 42, 666;
     *
     * Braced list:
     *   option {
     *     1;
     *     42;
     *     666;
     *   };
     *
     */

    template <typename T> struct vector : boost::spirit::x3::parser<vector<T>> {
        typedef std::vector<typename T::attribute_type> attribute_type;
        static bool const has_attribute = true;

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator &first, Iterator const &last,
                   Context const &context, boost::spirit::x3::unused_type,
                   Attribute &attr) const {
            namespace x3 = boost::spirit::x3;

            auto const &policy = x3::get<parser_policy_tag>(context).get();

            static T parser;
            static auto const inline_grammar = parser % ',';
            static auto const braced_grammar =
                '{' > *(parser > policy.option_terminator()) > '}';

            if (policy.allow_inline_lists) {
                if (inline_grammar.parse(first, last, context, x3::unused,
                                         attr))
                    return true;
            }

            if (policy.allow_braced_lists) {
                if (braced_grammar.parse(first, last, context, x3::unused,
                                         attr))
                    return true;
            }

            return false;
        }
    };

} // namespace sk::config::detail::parser

#endif // SK_CONFIG_DETAIL_VECTOR_PARSER_HXX_INCLUDED
