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

#ifndef SK_CONFIG_CONFIG_LOCATION_HXX_INCLUDED
#define SK_CONFIG_CONFIG_LOCATION_HXX_INCLUDED

#include <cstdlib>
#include <string>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <sk/value.hxx>

namespace sk::config {

    // A location in the configuration file.
    struct location {
        location() : line(0), column(0) {}

        location(std::size_t l, std::size_t c) : line(l), column(c) {}

        location(location const &) = default;
        location(location &&) = default;
        location &operator=(location const &) = default;
        location &operator=(location &&) = default;

        template <typename Iterator>
        location(Iterator begin, Iterator current)
            : line(get_line(current)), column(get_column(begin, current))
        //, line_text(get_current_line(begin, current))
        {}

        std::size_t line;
        std::size_t column;
        //        std::string line_text;
    };

} // namespace sk::config

BOOST_FUSION_ADAPT_STRUCT(sk::config::location, line, column)

#endif // SK_CONFIG_CONFIG_LOCATION_HXX_INCLUDED