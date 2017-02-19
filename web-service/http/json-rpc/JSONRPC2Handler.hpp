//
//  JSONRPC2Handler.hpp
//  web-service
//
//  Created by Ivan Kishchenko on 2/19/17.
//  Copyright © 2017 Ivan Kishchenko. All rights reserved.
//

#ifndef JSONRPC2Handler_hpp
#define JSONRPC2Handler_hpp

#include <stdio.h>
#include "request.hpp"
#include "reply.hpp"
#include "request_handler.hpp"

#include <map>
#include <string>

#include "JSONRPC2Method.hpp"

class JSONRPC2Handler : public http::server::request_handler_abstract {
public:
    virtual bool handle_request(const http::server::request& req, http::server::reply& rep);
private:
    std::map<std::string, SPMethod> m_methods;
};

#endif /* JSONRPC2Handler_hpp */
