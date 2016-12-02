//
//  helper.h
//  web-service
//
//  Created by Ivan Kishchenko on 12/2/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include "spec.hpp"

#include <boost/property_tree/json_parser.hpp>

namespace json_rpc {

    class Helper {
    public:
        template<typename T>
        void marshal(jsonTree& pt, const std::string& key, T value) {
            pt.put(key, value);
        }
        template<typename T>
        void marshal(jsonTree& pt, const std::string& key, boost::optional<T>& value) {
            if (value) {
                marshal(pt, key, value.get());
            }
        }
        void marshal(jsonTree& pt, const std::string& key, jsonTree& value) {
            pt.put(key, value);
        }
    };
    
}

#endif /* helper_h */
