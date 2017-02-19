//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_ROUTER_HPP
#define HTTP_REQUEST_ROUTER_HPP

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "request_handler.hpp"

namespace http {
    namespace server {
        
        
        struct reply;
        struct request;
        
        /// The common handler for all incoming requests.
        class request_router
        {
        public:
            request_router(const request_router&) = delete;
            request_router& operator=(const request_router&) = delete;
            
            /// Construct with a directory containing files to be served.
            explicit request_router(boost::shared_ptr<request_handler_abstract> route);
            
            /// Handle a request and produce a reply.
            bool handle_request(const request& req, reply& rep);
            
            void register_handler(const std::string& method, const std::string& handle, boost::shared_ptr<request_handler_abstract> route);
        private:
            /// The directory containing the files to be served.
            std::string doc_root_;
            
            /// Perform URL-decoding on a string. Returns false if the encoding was
            /// invalid.
            static bool url_decode(const std::string& in, std::string& out);
            
            boost::shared_ptr<request_handler_abstract> default_;
            std::map<std::string, boost::shared_ptr<request_handler_abstract> > routes_;
        };
        
    } // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP
