//
//  config.hpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/26/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <stdio.h>

#include <string>
#include "json/json-helper.h"

class Config {
public:
    int httpPort;
    std::string httpAddress;
public:
    BEGIN_JSON_UNMARSHAL
    __ITEM_JSON_UNMARSHAL(httpPort)
    __ITEM_JSON_UNMARSHAL(httpAddress)
    END_JSON_UNMARSHAL
};

#endif /* config_hpp */
