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

#include <sk/config/error.hxx>
#include <sk/config/parse.hxx>

/*
 * Declarative parser tests.
 */

TEST_CASE("unsigned short value") {
    namespace cr = sk::config::parser;

    struct test_config {
        unsigned short v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 42;", grammar, c);
    REQUIRE(c.v == 42);
}

TEST_CASE("short value") {
    namespace cr = sk::config::parser;

    struct test_config {
        short v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v -42;", grammar, c);
    REQUIRE(c.v == -42);
}

TEST_CASE("unsigned int value") {
    namespace cr = sk::config::parser;

    struct test_config {
        unsigned int v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 42;", grammar, c);
    REQUIRE(c.v == 42);
}

TEST_CASE("int value") {
    namespace cr = sk::config::parser;

    struct test_config {
        int v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v -42;", grammar, c);
    REQUIRE(c.v == -42);
}

TEST_CASE("unsigned long value") {
    namespace cr = sk::config::parser;

    struct test_config {
        unsigned long v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 42;", grammar, c);
    REQUIRE(c.v == 42);
}

TEST_CASE("long value") {
    namespace cr = sk::config::parser;

    struct test_config {
        long v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v -42;", grammar, c);
    REQUIRE(c.v == -42);
}

TEST_CASE("unsigned long long value") {
    namespace cr = sk::config::parser;

    struct test_config {
        unsigned long long v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 42;", grammar, c);
    REQUIRE(c.v == 42);
}

TEST_CASE("long long value") {
    namespace cr = sk::config::parser;

    struct test_config {
        long long v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v -42;", grammar, c);
    REQUIRE(c.v == -42);
}

TEST_CASE("float value") {
    namespace cr = sk::config::parser;

    struct test_config {
        float v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 5.4;", grammar, c);
    REQUIRE(c.v == Approx(5.4f));
}

TEST_CASE("double value") {
    namespace cr = sk::config::parser;

    struct test_config {
        double v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 5.4;", grammar, c);
    REQUIRE(c.v == Approx(5.4));
}

TEST_CASE("long double value") {
    namespace cr = sk::config::parser;

    struct test_config {
        long double v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 5.4;", grammar, c);
    REQUIRE(c.v == Approx(5.4));
}

TEST_CASE("double with int argument") {
    namespace cr = sk::config::parser;

    struct test_config {
        double v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 5;", grammar, c);
    REQUIRE(c.v == Approx(5));
}

#if 0
TEST_CASE("hex int") {
    namespace cr = sk::config::parser;

    struct test_config {
        unsigned v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 0x50;", grammar, c);
    REQUIRE(c.v == 0x50);
}
#endif

TEST_CASE("bare string") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::string v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v bare-string;", grammar, c);
    REQUIRE(c.v == "bare-string");
}

TEST_CASE("single-quoted string string") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::string v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse(
        "v 'a string with \\t tab \\n newline \\' quote \\\\ backslash';", grammar,
        c);
    REQUIRE(c.v == "a string with \t tab \n newline ' quote \\ backslash");
}

TEST_CASE("double-quoted string") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::string v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse(
        "v \"a string with \\t tab \\n newline \\\" quote \\\\ backslash\";", grammar,
        c);
    REQUIRE(c.v == "a string with \t tab \n newline \" quote \\ backslash");
}
