//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <tuple>
#include <string>

namespace http {
    namespace server {
        
        struct request;
        
        /// Parser for incoming requests.
        class request_parser
        {
        public:
            /// Construct ready to parse the request method.
            request_parser();
            
            /// Reset to initial parser state.
            void reset();
            
            /// Result of parse.
            enum result_type { good, bad, indeterminate, content};
            
            /// Parse some data. The enum return value is good when a complete request has
            /// been parsed, bad if the data is invalid, indeterminate when more data is
            /// required. The InputIterator return value indicates how much of the input
            /// has been consumed.
            template <typename InputIterator>
            std::tuple<result_type, InputIterator> parse(request& req,
                                                         InputIterator begin, InputIterator end)
            {
                result_type result;
                switch (state_) {
                    case content_fixed_size:
                    {
                        auto block_size = std::min((size_t)(end - begin), left_acumulate_);
                        append_content(req, &(*begin), block_size);
                        left_acumulate_ -= block_size;
                        if (!left_acumulate_) {
                            result = good;
                        }
                        begin = begin + block_size;
                    }
                        break;
                    case content_unknown_size:
                    {
                        auto block_size = end - begin;
                        if (!block_size) {
                            result = good;
                        } else {
                            append_content(req, &(*begin), block_size);
                            result = indeterminate;
                            begin = end;
                        }
                    }
                        break;
                    default:
                        while (begin != end)
                        {
                            result = consume(req, *begin++);
                            if (result == good || result == bad)
                                return std::make_tuple(result, begin);
                            else if (result == content)
                                return parse(req, begin, end);
                        }
                }

                if (result == good || result == bad) {
                    return std::make_tuple(result, begin);
                }
                
                return std::make_tuple(indeterminate, begin);
            }
            
        private:
            /// Handle the next character of input.
            result_type consume(request& req, char input);
            void append_content(request& req, char* input, size_t block_size);

            /// The name of the content length header.
            static std::string content_length_name_;

            /// The name of the content type header.
            static std::string content_type_name_;

            /// Content length as decoded from headers. Defaults to 0.
            std::size_t content_length_;

            std::size_t left_acumulate_;

            /// Check if a byte is an HTTP character.
            static bool is_char(int c);
            
            /// Check if a byte is an HTTP control character.
            static bool is_ctl(int c);
            
            /// Check if a byte is defined as an HTTP tspecial character.
            static bool is_tspecial(int c);
            
            /// Check if a byte is a digit.
            static bool is_digit(int c);
            
            /// Check if two characters are equal, without regard to case.
            static bool tolower_compare(char a, char b);
            
            /// Check whether the two request header names match.
            bool headers_equal(const std::string& a, const std::string& b);
            
            /// The current state of the parser.
            enum state
            {
                method_start,
                method,
                uri,
                http_version_h,
                http_version_t_1,
                http_version_t_2,
                http_version_p,
                http_version_slash,
                http_version_major_start,
                http_version_major,
                http_version_minor_start,
                http_version_minor,
                expecting_newline_1,
                header_line_start,
                header_lws,
                header_name,
                space_before_header_value,
                header_value,
                expecting_newline_2,
                expecting_newline_3,
                content_unknown_size,
                content_fixed_size,
            } state_;
        };
        
    } // namespace server
} // namespace http

#endif // HTTP_REQUEST_PARSER_HPP
