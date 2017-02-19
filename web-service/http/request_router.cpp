#include "request_router.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "logger.hpp"

#include "request.hpp"
#include "reply.hpp"

namespace http {
    namespace server {
        
        request_router::request_router(const std::string& doc_root)
        : doc_root_(doc_root)
        {
        }
        
        bool request_router::handle_request(const request& req, reply& rep)
        {
            // Decode url to path.
            std::string request_path;
            if (!url_decode(req.uri, request_path))
            {
                rep = reply::stock_reply(reply::bad_request);
                return true;
            }
            
            // Request path must be absolute and not contain "..".
            if (request_path.empty() || request_path[0] != '/'
                || request_path.find("..") != std::string::npos)
            {
                rep = reply::stock_reply(reply::bad_request);
                return true;
            }
            
            LOG_INFO << "Route: " << request_path;
            auto route = routes_.find(req.method+":"+request_path);
            if (route != routes_.end()) {
                if (!route->second->handle_request(req, rep)) {
                    rep = reply::stock_reply(reply::bad_request);
                }
                
                return true;
            }
            
            rep = reply::stock_reply(reply::not_found);
            return true;
        }
        
        bool request_router::url_decode(const std::string& in, std::string& out)
        {
            out.clear();
            out.reserve(in.size());
            for (std::size_t i = 0; i < in.size(); ++i)
            {
                if (in[i] == '%')
                {
                    if (i + 3 <= in.size())
                    {
                        int value = 0;
                        std::istringstream is(in.substr(i + 1, 2));
                        if (is >> std::hex >> value)
                        {
                            out += static_cast<char>(value);
                            i += 2;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (in[i] == '+')
                {
                    out += ' ';
                }
                else
                {
                    out += in[i];
                }
            }
            return true;
        }
        
        void request_router::register_handler(const std::string& method, const std::string& handle, boost::shared_ptr<request_handler_abstract> route) {
            routes_[method+":"+handle] = route;
        }

        
    } // namespace server
} // namespace http
