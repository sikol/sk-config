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

#include <catch.hpp>

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

#include <sk/config/config.hxx>
#include <sk/config/option.hxx>
#include <sk/config/parse.hxx>
#include <sk/config/parser/numeric.hxx>

TEST_CASE("custom option terminator") {
    namespace cfg = sk::config;
    namespace x3 = boost::spirit::x3;

    struct newline_policy : cfg::parser_policy {
        auto option_terminator() const {
            return x3::eol;
        }
    };

    struct test_config {
        int a, b;
    };

    auto grammar = cfg::config<test_config>(cfg::option("a", &test_config::a),
                                            cfg::option("b", &test_config::b));

    test_config c;
    try {
        cfg::parse<newline_policy>("a 42\nb 666\n", grammar, c);
    } catch (cfg::parse_error const &e) {
        std::cout << e;
    }

    REQUIRE(c.a == 42);
    REQUIRE(c.b == 666);
}

TEST_CASE("custom option separator") {
    namespace cfg = sk::config;
    namespace x3 = boost::spirit::x3;

    struct newline_policy : cfg::parser_policy {
        auto option_separator() const {
            return x3::lit('=');
        }
    };

    struct test_config {
        int a, b;
    };

    auto grammar = cfg::config<test_config>(cfg::option("a", &test_config::a),
                                            cfg::option("b", &test_config::b));

    test_config c;
    try {
        cfg::parse<newline_policy>("a = 42; b = 666;\n", grammar, c);
    } catch (cfg::parse_error const &e) {
        std::cout << e;
    }

    REQUIRE(c.a == 42);
    REQUIRE(c.b == 666);
}