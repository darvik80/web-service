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

#include "types.h"

namespace json_rpc {
    
    class JsonBase : public IMarshal, IUnmarshal, IValidator {
    public:
        const std::string encode() {
            auto doc = marshal();
            std::ostringstream os;
            boost::property_tree::write_json(os, doc, false);
            
            return os.str();
        }
        void decode(const std::string& json) throw (std::exception) {
            jsonTree doc;
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
        static void marshal(jsonTree& pt, const std::string& tag, const T value) {
            pt.put<T>(tag, value);
        }
        template<typename T>
        static void marshal(jsonTree& pt, const std::string& tag, const boost::optional<T>& value) {
            if (value) {
                marshal(pt, tag, value.get());
            }
        }
        static void marshal(jsonTree& pt, const std::string& tag, const jsonTree& value) {
            pt.put_child(tag, value);
        }
        
        template<typename T>
        static void unmarshal(const jsonTree& pt, const std::string& tag, T& value) {
            value = pt.get<T>(tag);
        }
        static void unmarshal(const jsonTree& pt, const std::string& tag, boost::optional<jsonTree>& value) {
            value = pt.get_child_optional(tag);
        }
        static void unmarshal(const jsonTree& pt, const std::string& tag, jsonTree& value) {
            value = pt.get_child(tag);
        }
        template<typename T>
        static void unmarshal(const jsonTree& pt, const std::string& tag, boost::optional<T>& value) {
            value = pt.get_optional<T>(tag);
        }
    };
    
#define BEGIN_JSON_MARSHAL const json_rpc::jsonTree marshal() throw (std::exception) { json_rpc::jsonTree root;
#define __ITEM_JSON_MARSHAL_OBJ(name) { auto v = name.marshal(); root.put_child(#name, v); }
#define __ITEM_JSON_MARSHAL_OBJ_OPT(name) if (name) { auto v = name.get().marshal(); root.put_child(#name, v); }
#define __ITEM_JSON_MARSHAL_OBJ_TAG(tag, name) { auto v = name.marshal(); root.put_child(#tag, v); }
#define __ITEM_JSON_MARSHAL_OBJ_TAG_OPT(tag, name) if (name) { auto v = name.get().marshal(); root.put_child(#tag, v); }
#define __ITEM_JSON_MARSHAL(name) json_rpc::helper::marshal(root, #name, name);
#define __ITEM_JSON_MARSHAL_TAG(tag, name) json_rpc::helper::marshal(root, #tag, name);
#define END_JSON_MARSHAL return root; }
    
#define BEGIN_JSON_UNMARSHAL void unmarshal(const json_rpc::jsonTree& root) throw (std::exception) {
#define __ITEM_JSON_UNMARSHAL(name) json_rpc::helper::unmarshal(root, #name, name);
#define __ITEM_JSON_UNMARSHAL_TAG(tag, name) json_rpc::helper::unmarshal(root, #tag, name);
#define __ITEM_JSON_UNMARSHAL_OBJ(name) if (auto v = root.get_child_optional(#name)) {  name = boost::in_place(); name.get().unmarshal(v.get()); }
#define __ITEM_JSON_UNMARSHAL_OBJ_TAG(tag, name) if (auto v = root.get_child_optional(#tag)) {  name = boost::in_place(); name.get().unmarshal(v.get()); }
#define END_JSON_UNMARSHAL }

    class JsonObject : public JsonBase {
        std::map<std::string, jsonTree> m_items;
        
    public:
        std::string id;
        std::string method;
        boost::optional<jsonTree> params;
    public:
        BEGIN_JSON_MARSHAL
            __ITEM_JSON_MARSHAL(id)
            __ITEM_JSON_MARSHAL(method)
            __ITEM_JSON_MARSHAL_TAG(old_method, method)
        END_JSON_MARSHAL
        
        BEGIN_JSON_UNMARSHAL
            __ITEM_JSON_UNMARSHAL(id)
            __ITEM_JSON_UNMARSHAL(method)
        END_JSON_UNMARSHAL
    };
}

#endif /* helper_h */
