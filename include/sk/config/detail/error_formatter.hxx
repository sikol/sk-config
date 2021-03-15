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

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*
 * This is a copy of Spirit's error_handler except it create error_detail
 * objects instead of printing directly to a stream.
 */

#ifndef SK_CONFIG_DETAIL_ERROR_FORMATTER_HXX_INCLUDED
#define SK_CONFIG_DETAIL_ERROR_FORMATTER_HXX_INCLUDED

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/utf8.hpp>
#include <ostream>

#include <sk/config/error_detail.hxx>

namespace sk::config::detail {
    // tag used to get our error handler from the context
    //    struct error_handler_tag;

    template <typename Iterator, typename OutputIterator>
    struct error_formatter {
      public:
        typedef Iterator iterator_type;

        error_formatter(Iterator first, Iterator last, OutputIterator err_out_,
                        std::string file_ = "", int tabs_ = 4)
            : err_out(err_out_), file(file_), tabs(tabs_),
              pos_cache(first, last) {}

        typedef void result_type;

        void operator()(Iterator err_pos,
                        std::string const &error_message) ;
        void operator()(Iterator err_first, Iterator err_last,
                        std::string const &error_message) ;
        void operator()(boost::spirit::x3::position_tagged pos,
                        std::string const &message)  {
            auto where = pos_cache.position_of(pos);
            (*this)(where.begin(), where.end(), message);
        }

        template <typename AST>
        void tag(AST &ast, Iterator first, Iterator last) {
            return pos_cache.annotate(ast, first, last);
        }

        boost::iterator_range<Iterator>
        position_of(boost::spirit::x3::position_tagged pos) const {
            return pos_cache.position_of(pos);
        }

        boost::spirit::x3::position_cache<std::vector<Iterator>> const &
        get_position_cache() const {
            return pos_cache;
        }

      private:
        auto get_line(Iterator line_start, Iterator last) const -> std::string;
        void skip_whitespace(Iterator &err_pos, Iterator last) const;
        void skip_non_whitespace(Iterator &err_pos, Iterator last) const;
        Iterator get_line_start(Iterator first, Iterator pos) const;
        std::size_t position(Iterator i) const;

        OutputIterator err_out;
        std::string file;
        int tabs;
        boost::spirit::x3::position_cache<std::vector<Iterator>> pos_cache;
    };

    template <typename Iterator, typename OutputIterator>
    std::string
    error_formatter<Iterator, OutputIterator>::get_line(Iterator start,
                                                        Iterator last) const {
        namespace x3 = boost::spirit::x3;

        auto end = start;
        while (end != last) {
            auto c = *end;
            if (c == '\r' || c == '\n')
                break;
            else
                ++end;
        }
        typedef typename std::iterator_traits<Iterator>::value_type char_type;
        std::basic_string<char_type> line{start, end};
        return x3::to_utf8(line);
    }

    template <typename Iterator, typename OutputIterator>
    void error_formatter<Iterator, OutputIterator>::skip_whitespace(
        Iterator &err_pos, Iterator last) const {
        // make sure err_pos does not point to white space
        while (err_pos != last) {
            char c = *err_pos;
            if (std::isspace(c))
                ++err_pos;
            else
                break;
        }
    }

    template <typename Iterator, typename OutputIterator>
    void error_formatter<Iterator, OutputIterator>::skip_non_whitespace(
        Iterator &err_pos, Iterator last) const {
        // make sure err_pos does not point to white space
        while (err_pos != last) {
            char c = *err_pos;
            if (std::isspace(c))
                break;
            else
                ++err_pos;
        }
    }

    template <class Iterator, typename OutputIterator>
    inline Iterator error_formatter<Iterator, OutputIterator>::get_line_start(
        Iterator first, Iterator pos) const {
        Iterator latest = first;
        for (Iterator i = first; i != pos; ++i)
            if (*i == '\r' || *i == '\n')
                latest = i;
        return latest;
    }

    template <typename Iterator, typename OutputIterator>
    std::size_t
    error_formatter<Iterator, OutputIterator>::position(Iterator i) const {
        std::size_t line{1};
        typename std::iterator_traits<Iterator>::value_type prev{0};

        for (Iterator pos = pos_cache.first(); pos != i; ++pos) {
            auto c = *pos;
            switch (c) {
            case '\n':
                if (prev != '\r')
                    ++line;
                break;
            case '\r':
                ++line;
                break;
            default:
                break;
            }
            prev = c;
        }

        return line;
    }

    template <typename Iterator, typename OutputIterator>
    void error_formatter<Iterator, OutputIterator>::operator()(
        Iterator err_pos, std::string const &error_message) {
        Iterator first = pos_cache.first();
        Iterator last = pos_cache.last();

        error_detail err;

        // make sure err_pos does not point to white space
        skip_whitespace(err_pos, last);

        err.file = file;
        err.line = position(err_pos);
        err.message = error_message;

        Iterator start = get_line_start(first, err_pos);
        if (start != first)
            ++start;

        err.context = get_line(start, last);
        err.column = static_cast<std::size_t>(std::distance(start, err_pos));

        *err_out++ = err;
    }

    template <typename Iterator, typename OutputIterator>
    void error_formatter<Iterator, OutputIterator>::operator()(
        Iterator err_first, Iterator err_last,
        std::string const &error_message) {
        Iterator first = pos_cache.first();
        Iterator last = pos_cache.last();

        // make sure err_pos does not point to white space
        skip_whitespace(err_first, last);

        err.file = file;
        err.line = position(err_pos);
        err.message = error_message;

        Iterator start = get_line_start(first, err_first);
        if (start != first)
            ++start;
        err.context = get_line(start, last);
        err.column = std::distance(start, err_first);

        *err_out++ = err;
    }

    template <typename Iterator, typename OutputIterator>
    error_formatter(Iterator, Iterator, OutputIterator)
        -> error_formatter<Iterator, OutputIterator>;

} // namespace sk::config::detail

#endif // SK_CONFIG_DETAIL_ERROR_FORMATTER_HXX_INCLUDED