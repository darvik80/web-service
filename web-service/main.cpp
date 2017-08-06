//
//  main.cpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/1/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#include <iostream>
#include "http/json-rpc/spec.h"
#include "json/json-helper.h"
#include "application.hpp"

#define BOOST_NETWORK_ENABLE_HTTPS

#include <boost/network/protocol/http/client.hpp>

using namespace boost::network;

int main(int argc, const char * argv[]) {
   
    http::client client;
    http::client::request request("https://mail.ru/");
    request << header("Connection", "close");
    http::client::response response = client.get(request);
    for(auto header : response.headers()) {
        std::cout << header.first << ": " << header.second << std::endl;
    }
    
    try {
        Application app;
        app.create(argc, argv);
        app.run();
        app.destroy();
        
    } catch (std::exception& e) {
        std::cerr << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl;
        
        return -1;
    }
    
    return 0;
}
