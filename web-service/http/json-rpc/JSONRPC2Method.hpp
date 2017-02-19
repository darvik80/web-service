//
//  JSONRPC2Method.hpp
//  web-service
//
//  Created by Ivan Kishchenko on 2/19/17.
//  Copyright © 2017 Ivan Kishchenko. All rights reserved.
//

#ifndef JSONRPC2Method_hpp
#define JSONRPC2Method_hpp

#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include "spec.h"
#include "json-types.h"

class Params : public json::IUnmarshal {
};

typedef boost::shared_ptr<Params> SPParams;

class Result : public json::IMarshal {
};

typedef boost::shared_ptr<Result> SPResult;

class Method {
public:
    virtual SPResult Handle(SPParams& args) throw (json_rpc::Error) = 0;
};

typedef boost::shared_ptr<Result> SPMethod;

#endif /* JSONRPC2Method_hpp */
