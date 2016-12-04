//
//  main.cpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/1/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#include <iostream>
#include "http/json-rpc/spec.hpp"
#include "http/json-rpc/helper.h"

int main(int argc, const char * argv[]) {

    json_rpc::jsonTree pt;
    json_rpc::helper::marshal(pt, "key1", "hello");
    
    boost::optional<std::string> op("Test");
    json_rpc::helper::marshal(pt, "key2", op);
    
    json_rpc::jsonTree tree;
    json_rpc::helper::marshal(pt, "key3", tree);

    boost::optional<json_rpc::jsonTree> opTree = tree;
    json_rpc::helper::marshal(pt, "key4", opTree);

    json_rpc::Error err(-32700, "Parse Error");
    json_rpc::helper::marshal(pt, "err", &err);
    
    std::ostringstream os;
    boost::property_tree::write_json(os, pt);
    std::cout << os.str() << std::endl;
    
    try {
        json_rpc::Response response;
        response.decode("{\"jsonrpc\": \"2.0\", \"id\":\"id#002\",\"error\":{\"code\": 100, \"message\":\"Test\"}}");
        std::cout << response.error.get().code << ": " << response.error.get().message << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Handle exception:" << ex.what() << std::endl;
    }
    
    /*
    std::string t;
    json_rpc::helper::unmarshal(pt, "key1", t);
    
    json_rpc::JsonObject test;
    test.id = "id#001";
    test.method = "Product.Test";
    
    std::cout << test.encode() << std::endl;

    test.decode("{\"id\":\"id#002\",\"method\":\"Product.UpdateProducts\", \"params\":\"test\"}");
    
    std::cout << test.method << std::endl;
    */
    
    return 0;
}
