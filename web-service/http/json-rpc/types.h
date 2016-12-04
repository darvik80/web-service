//
//  types.h
//  web-service
//
//  Created by Ivan Kishchenko on 12/2/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef types_h
#define types_h

namespace json_rpc {
    typedef boost::property_tree::ptree jsonTree;

    class IMarshal {
    public:
        virtual const jsonTree marshal() throw (std::exception) = 0;
    };

    class IUnmarshal {
    public:
        virtual void unmarshal(const jsonTree& doc) throw (std::exception) = 0;
    };

    class IValidator {
    public:
        virtual bool validate() = 0;
    };
}

#endif /* types_h */
