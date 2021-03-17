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

#ifndef SK_CONFIG_PARSER_NUMERIC_HPP
#define SK_CONFIG_PARSER_NUMERIC_HPP

#include <concepts>

#include <boost/spirit/home/x3/numeric/int.hpp>
#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/spirit/home/x3/numeric/uint.hpp>

#include <sk/config/detail/parser/bool.hxx>
#include <sk/config/parser_for.hxx>

namespace sk::config {

    /*
     * bool is here because it's an std::unsigned_integral, so if we
     * put it in its own header, anyone including numeric.hxx but not
     * the bool header would incorrectly parse bool instead of creating
     * a compile-time error.
     */
    template <> struct parser_for<bool> {
        using parser_type = detail::parser::bool_parser;
        using rule_type = bool;
        static constexpr char const name[] = "a boolean";
    };

    void propagate_value(auto & /*ctx*/, bool &to, bool from) {
        to = from;
    }

    // signed_integral
    template <std::signed_integral T> struct parser_for<T> {
        using parser_type = boost::spirit::x3::int_parser<T>;
        using rule_type = T;
        static constexpr char const name[] = "an integer";
    };

    template <std::signed_integral T>
    void propagate_value(auto & /*ctx*/, T &to, T from) {
        to = from;
    }

    // unsigned_integral
    template <std::unsigned_integral T> struct parser_for<T> {
        using parser_type = boost::spirit::x3::uint_parser<T>;
        using rule_type = T;
        static constexpr char const name[] = "a positive integer";
    };

    template <std::unsigned_integral T>
    void propagate_value(auto & /*ctx*/, T &to, T from) {
        to = from;
    }

    // floating_point
    template <typename T>
    struct config_real_policies : boost::spirit::x3::real_policies<T> {};

    template <std::floating_point T> struct parser_for<T> {
        using parser_type =
            boost::spirit::x3::real_parser<float, config_real_policies<float>>;
        using rule_type = T;
        static constexpr char const name[] = "a decimal number";
    };

    template <std::floating_point T>
    void propagate_value(auto & /*ctx*/, T &to, T from) {
        to = from;
    }

}; // namespace sk::config

#endif // SK_CONFIG_PARSER_NUMERIC_HPP
