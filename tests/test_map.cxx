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
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include <sk/config/block.hxx>
#include <sk/config/config.hxx>
#include <sk/config/option.hxx>
#include <sk/config/parse.hxx>
#include <sk/config/parser/numeric.hxx>
#include <sk/config/parser/string.hxx>
#include <sk/config/parser/map.hxx>

TEST_CASE("std::map<>") {
    namespace cfg = sk::config;

    struct test_block {
        std::string name;
        int value;
    };

    struct test_config {
        std::map<std::string, test_block> items;
    };

    auto grammar =                                                     //
        cfg::config<test_config>(                                      //
            cfg::block("item", &test_block::name, &test_config::items, //
                       cfg::option("value", &test_block::value)));
    test_config c;

    cfg::parse(R"(
item "one" { value 1; };
item "forty-two" { value 42; };
)",
               grammar, c);

    REQUIRE(c.items.size() == 2);
    REQUIRE(c.items["one"].value == 1);
    REQUIRE(c.items["forty-two"].value == 42);
}

TEST_CASE("std::map<> duplicate value") {
    namespace cfg = sk::config;

    struct test_block {
        std::string name;
        int value;
    };

    struct test_config {
        std::map<std::string, test_block> items;
    };

    auto grammar =                                                     //
        cfg::config<test_config>(                                      //
            cfg::block("item", &test_block::name, &test_config::items, //
                       cfg::option("value", &test_block::value)));
    test_config c;

    REQUIRE_THROWS(cfg::parse(R"(
item "one" { value 1; };
item "one" { value 42; };
)",
                              grammar, c));
}
