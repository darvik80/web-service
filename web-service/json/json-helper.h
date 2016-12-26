//
//  helper.h
//  web-service
//
//  Created by Ivan Kishchenko on 12/2/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include <map>
#include <string>
#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/utility/in_place_factory.hpp>

#include "json-types.h"

namespace json {
    
    class JsonBase : public IMarshal, IUnmarshal, IValidator {
    public:
        const std::string encode() {
            auto doc = marshal();
            std::ostringstream os;
            boost::property_tree::write_json(os, doc, false);
            
            return os.str();
        }
        void decode(const std::string& json) throw (std::exception) {
            tree doc;
            std::istringstream is(json);
            boost::property_tree::read_json(is, doc);
            
            unmarshal(doc);
            
            if (!validate()) {
                throw std::invalid_argument(json);
            }
        }
        
        virtual bool validate() {
            return true;
        }
    };
    
    class helper {
    public:
        template<typename T>
        static void marshal(tree& pt, const std::string& tag, const T value) {
            pt.put<T>(tag, value);
        }
        template<typename T>
        static void marshal(tree& pt, const std::string& tag, const boost::optional<T>& value) {
            if (value) {
                marshal(pt, tag, value.get());
            }
        }
        static void marshal(tree& pt, const std::string& tag, const tree& value) {
            pt.put_child(tag, value);
        }
        
        template<typename T>
        static void unmarshal(const tree& pt, const std::string& tag, T& value) {
            value = pt.get<T>(tag);
        }
        static void unmarshal(const tree& pt, const std::string& tag, boost::optional<tree>& value) {
            //value = pt.get_child_optional(tag);
        }
        static void unmarshal(const tree& pt, const std::string& tag, tree& value) {
            value = pt.get_child(tag);
        }
        template<typename T>
        static void unmarshal(const tree& pt, const std::string& tag, boost::optional<T>& value) {
            value = pt.get_optional<T>(tag);
        }
    };
    
#define BEGIN_JSON_MARSHAL const json::tree marshal() throw (std::exception) { json::tree root;
#define __ITEM_JSON_MARSHAL_OBJ(name) { auto v = name.marshal(); root.put_child(#name, v); }
#define __ITEM_JSON_MARSHAL_OBJ_OPT(name) if (name) { auto v = name.get().marshal(); root.put_child(#name, v); }
#define __ITEM_JSON_MARSHAL_OBJ_TAG(tag, name) { auto v = name.marshal(); root.put_child(#tag, v); }
#define __ITEM_JSON_MARSHAL_OBJ_TAG_OPT(tag, name) if (name) { auto v = name.get().marshal(); root.put_child(#tag, v); }
#define __ITEM_JSON_MARSHAL(name) json::helper::marshal(root, #name, name);
#define __ITEM_JSON_MARSHAL_TAG(tag, name) json::helper::marshal(root, #tag, name);
#define END_JSON_MARSHAL return root; }
    
#define BEGIN_JSON_UNMARSHAL void unmarshal(const json::tree& root) throw (std::exception) {
#define __ITEM_JSON_UNMARSHAL(name) json::helper::unmarshal(root, #name, name);
#define __ITEM_JSON_UNMARSHAL_TAG(tag, name) json::helper::unmarshal(root, #tag, name);
#define __ITEM_JSON_UNMARSHAL_OBJ(name) if (auto v = root.get_child_optional(#name)) {  name = boost::in_place(); name.get().unmarshal(v.get()); }
#define __ITEM_JSON_UNMARSHAL_OBJ_TAG(tag, name) if (auto v = root.get_child_optional(#tag)) {  name = boost::in_place(); name.get().unmarshal(v.get()); }
#define END_JSON_UNMARSHAL }
}

#endif /* helper_h */
