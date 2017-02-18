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
#include <boost/program_options/variables_map.hpp>

class Application {
public:
    void create(int argc, const char * argv[]);
    void run();
    void destroy();
    
private:
    boost::program_options::variables_map m_vm;
};

#endif /* application_hpp */
