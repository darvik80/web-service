//
//  JSONRPC2Handler.cpp
//  web-service
//
//  Created by Ivan Kishchenko on 2/19/17.
//  Copyright © 2017 Ivan Kishchenko. All rights reserved.
//

#include "JSONRPC2Handler.hpp"

bool JSONRPC2Handler::handle_request(const http::server::request& req, http::server::reply& rep) {
    if (req.method != "POST") {
        rep = http::server::reply::stock_reply(http::server::reply::bad_request);
        return true;
    }

    if (req.content_type.find("application/json") == std::string::npos) {
        rep = http::server::reply::stock_reply(http::server::reply::bad_request);
        return true;
    }

    json_rpc::Error err(json_rpc::MethodNotFound, "The method does not exist");

    json_rpc::Response response;
    response.error = err;

    // Fill out the reply to be sent to the client.
    rep.status = http::server::reply::ok;
    rep.content = response.encode();
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = "application/json; charset=\"utf-8\"";

    return true;
}
