//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <utility>
#include <vector>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "request_router.hpp"
#include "logger.hpp"

namespace http {
    namespace server {
        
        connection::connection(boost::asio::ip::tcp::socket socket,
                               connection_manager& manager, request_router& router)
        : socket_(std::move(socket)),
        connection_manager_(manager),
        request_router_(router)
        {
        }
        
        void connection::start()
        {
            do_read();
        }
        
        void connection::stop()
        {
            socket_.close();
        }
        
        void connection::do_read()
        {
            auto self(shared_from_this());
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                                    {
                                       if (!ec || (boost::asio::error::eof == ec) || (boost::asio::error::connection_reset == ec)) {
                                            if ((boost::asio::error::eof == ec) || (boost::asio::error::connection_reset == ec)) {
                                                LOG_DEBUG << "Handle eof or connection_reset, " << ec << ", bytes transfered: " << bytes_transferred;
                                            } else {
                                                LOG_DEBUG << "Handle data, bytes transfered: " << bytes_transferred;
                                            }
                                            request_parser::result_type result;

                                            std::tie(result, std::ignore) = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);
                                            
                                            if (result == request_parser::good) {
                                                if (!request_router_.handle_request(request_, reply_)) {
                                                    reply_ = reply::stock_reply(reply::not_found);
                                                }
                                                
                                                do_write();
                                            }
                                            else if (result == request_parser::bad) {
                                                reply_ = reply::stock_reply(reply::bad_request);
                                                do_write();
                                            }
                                            else {
                                                do_read();
                                            }
                                        } else if (ec != boost::asio::error::operation_aborted) {
                                            LOG_DEBUG << "Handle error: " << ec << ", bytes transfered: " << bytes_transferred;
                                            connection_manager_.stop(shared_from_this());
                                        }
                                    });
        }
        
        void connection::do_write()
        {
            auto self(shared_from_this());
            boost::asio::async_write(socket_, reply_.to_buffers(),
                                     [this, self](boost::system::error_code ec, std::size_t)
                                     {
                                         if (!ec)
                                         {
                                             // Initiate graceful connection closure.
                                             boost::system::error_code ignored_ec;
                                             socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                                              ignored_ec);
                                         }
                                         
                                         if (ec != boost::asio::error::operation_aborted)
                                         {
                                             connection_manager_.stop(shared_from_this());
                                         }
                                     });
        }
        
    } // namespace server
} // namespace http
