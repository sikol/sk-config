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

#ifndef SK_CONFIG_PARSER_UNORDERED_MAP_HXX_INCLUDED
#define SK_CONFIG_PARSER_UNORDERED_MAP_HXX_INCLUDED

#include <unordered_map>

#include <boost/spirit/home/x3.hpp>

#include <sk/config/detail/parser/map.hxx>
#include <sk/config/parser_for.hxx>

namespace sk::config {

    template <typename Key, typename Value>
    struct parser_for<std::unordered_map<Key, Value>> {
        using parser_type =
            detail::parser::map<typename parser_for<Key>::parser_type,
                                typename parser_for<Value>::parser_type>;
        using rule_type = std::vector<std::pair<Key, Value>>;
        static constexpr char const name[] = "a block";
    };

    namespace detail {

        template <typename T, typename U>
        void propagate_value(auto &ctx, std::unordered_map<T, U> &to, U &from,
                             auto &name) {
            namespace x3 = boost::spirit::x3;

            auto r = to.insert(std::make_pair(from.*name, from));

            if (!r.second) {
                auto it = x3::_where(ctx).begin();
                boost::throw_exception(
                    x3::expectation_failure<decltype(it)>(it, "unique value"));
            }
        }

        template <typename T, typename U>
        void propagate_value(auto &ctx, std::unordered_map<T, U> &to,
                             std::vector<std::pair<T, U>> &from) {
            namespace x3 = boost::spirit::x3;

            for (auto &&item : from) {
                auto r = to.insert(item);

                if (!r.second) {
                    auto it = x3::_where(ctx).begin();
                    boost::throw_exception(
                        x3::expectation_failure<decltype(it)>(it,
                                                              "unique value"));
                }
            }
        }

    } // namespace detail

} // namespace sk::config

#endif // SK_CONFIG_PARSER_UNORDERED_MAP_HXX_INCLUDED
