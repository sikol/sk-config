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

#ifndef SK_CONFIG_CONFIG_PARSE_HXX
#define SK_CONFIG_CONFIG_PARSE_HXX

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/utf8.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <sk/config/detail/error_formatter.hxx>
#include <sk/config/detail/comment_parser.hxx>
#include <sk/config/error.hxx>

namespace sk::config {

    /*
     * Wrapper around x3::phrase_parse to handle errors.
     */
    template <typename Iterator>
    auto parse(Iterator first, Iterator last, auto const &grammar, auto &ret,
               std::string const &filename = "") {
        namespace x3 = boost::spirit::x3;

        std::vector<error_detail> errors;
        auto error_handler = detail::error_formatter(
            first, last, std::back_inserter(errors), filename);

        auto const grammar_ =
            x3::with<x3::error_handler_tag>(std::ref(error_handler))[grammar];

        bool r = x3::phrase_parse(first, last, grammar_, detail::comment_parser,
                                  ret);
        if (r == false || (first != last))
            throw parse_error("could not parse the entire input", errors);
        return true;
    }

    auto parse(std::ranges::range auto const &r, auto const &grammar, auto &ret,
               std::string const &filename = "") {
        return sk::config::parse(std::ranges::begin(r), std::ranges::end(r),
                                 grammar, ret, filename);
    }

    auto parse(char const *s, auto const &grammar, auto &ret,
               std::string const &filename = "") {
        return sk::config::parse(std::string_view(s), grammar, ret, filename);
    }

    auto parse_file(std::filesystem::path filename, auto const &grammar,
                    auto &ret) {

        std::ifstream fs;
        fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        auto utf8name = boost::spirit::x3::to_utf8(filename.native());

        try {
            fs.open(filename);
            fs.unsetf(std::ios::skipws);
            boost::spirit::istream_iterator begin(fs), end;
            return parse(begin, end, grammar, ret, utf8name);
        } catch (std::ios_base::failure const &e) {
            error_detail ed;
            ed.file = utf8name;
            ed.line = 0;
            ed.column = 0;

            std::ostringstream strm;
            auto error_code = e.code();
            strm << filename << ": cannot read file: " << error_code.message();
            ed.message = strm.str();

            throw parse_error(ed.message, std::vector<error_detail>{ed});
        }
    }
} // namespace sk::config

#endif // YARROW_CONFIG_PARSE_HXX
