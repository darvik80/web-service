//
//  application.hpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/26/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef application_hpp
#define application_hpp

#include <stdio.h>

class Application {
public:
    void create(int argc, const char * argv[]);
    void run();
    void destroy();
};

#endif /* application_hpp */
