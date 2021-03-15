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

TEST_CASE("std::vector<int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::vector<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    sk::config::parse(R"(
int-value 1, 42;
int-value 666;
)",
                      grammar, c);

    REQUIRE(c.items.size() == 3);
    REQUIRE(c.items[0] == 1);
    REQUIRE(c.items[1] == 42);
    REQUIRE(c.items[2] == 666);
}

TEST_CASE("std::deque<int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::deque<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    sk::config::parse(R"(
int-value 1, 42;
int-value 666;
)",
                      grammar, c);

    REQUIRE(c.items.size() == 3);
    REQUIRE(c.items[0] == 1);
    REQUIRE(c.items[1] == 42);
    REQUIRE(c.items[2] == 666);
}

TEST_CASE("std::list<int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::list<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    sk::config::parse(R"(
int-value 1, 42;
int-value 666;
)",
                      grammar, c);

    REQUIRE(c.items.size() == 3);

    auto begin = c.items.begin();

    REQUIRE(*begin == 1);
    ++begin;

    REQUIRE(*begin == 42);
    ++begin;

    REQUIRE(*begin == 666);
}

TEST_CASE("std::set<int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::set<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    sk::config::parse(R"(
int-value 1, 42;
int-value 666;
)",
                      grammar, c);

    REQUIRE(c.items.size() == 3);

    auto begin = c.items.begin();

    REQUIRE(*begin == 1);
    ++begin;

    REQUIRE(*begin == 42);
    ++begin;

    REQUIRE(*begin == 666);
}

TEST_CASE("std::unordered_set<int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::unordered_set<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    sk::config::parse(R"(
int-value 1, 42;
int-value 666;
)",
                      grammar, c);

    REQUIRE(c.items.size() == 3);

    auto begin = c.items.begin();

    REQUIRE(*begin == 1);
    ++begin;

    REQUIRE(*begin == 42);
    ++begin;

    REQUIRE(*begin == 666);
}

TEST_CASE("std::set<int> fails on non-unique values") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::set<int> items;
    };

    auto grammar =
        cr::config<test_config>(cr::option("int-value", &test_config::items));
    test_config c;

    REQUIRE_THROWS(sk::config::parse(R"(
int-value 1, 42;
int-value 42;
)",
                                     grammar, c));
}

TEST_CASE("variant<int,string> is int") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::variant<int, std::string> v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 5;", grammar, c);
    REQUIRE(get<int>(c.v) == 5);
}

TEST_CASE("variant<int,string> is string") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::variant<int, std::string> v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));
    test_config c;
    sk::config::parse("v 'testing';", grammar, c);
    REQUIRE(get<std::string>(c.v) == "testing");
}

#if 0 // Broken
TEST_CASE("variant<float,int>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::variant<float, int> v1;
        std::variant<float, int> v2;
    };

    auto grammar = cr::config<test_config>(
        cr::option("v1", &test_config::v1),
        cr::option("v2", &test_config::v2));

    test_config c;
    try {

        sk::config::parse("v1 42; v2 42.0;", grammar, c);
    } catch (sk::config::parse_error const &e) {
        std::cerr << e;
    }
    REQUIRE(get<int>(c.v1) == 42);
    REQUIRE(get<float>(c.v2) == 42.0f);
}
#endif

TEST_CASE("tuple<>") {
    namespace cr = sk::config::parser;

    struct test_config {
        std::tuple<int, std::string, float> v;
    };

    auto grammar = cr::config<test_config>(cr::option("v", &test_config::v));

    test_config c;
    try {

        sk::config::parse("v 42, 'test string', 1.5;", grammar, c);
    } catch (sk::config::parse_error const &e) {
        std::cerr << e;
    }
    REQUIRE(get<0>(c.v) == 42);
    REQUIRE(get<1>(c.v) == "test string");
    REQUIRE(get<2>(c.v) == Approx(1.5));
}
