//
//  spec.h
//  web-service
//
//  Created by Ivan Kishchenko on 12/1/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#ifndef spec_h
#define spec_h

#include <stdio.h>
#include <string>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../../json/json-types.h"
#include "../../json/json-helper.h"

namespace json_rpc {
#define Version "2.0"
    
    enum Code {
        ParseError = -32700,	// Parse error	Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.
        InvalideRequest = -32600,	// Invalid Request	The JSON sent is not a valid Request object.
        MethodNotFound = -32601,	// Method not found	The method does not exist / is not available.
        InvalidParams = -32602,	// Invalid params	Invalid method parameter(s).
        InternalError = -32603,	// Internal error	Internal JSON-RPC error.
        ServerError = -32000, // to -32099	Server error	Reserved for implementation-defined server-errors.
    };
    
    class Request : public json::JsonBase {
    public:
        std::string method;
        boost::optional<json::tree> params;
        
        boost::optional<json::tree> id;
        std::string jsonrpc;
    public:
        Request() : jsonrpc(Version) {}
        BEGIN_JSON_MARSHAL
        __ITEM_JSON_MARSHAL(id)
        __ITEM_JSON_MARSHAL(method)
        __ITEM_JSON_MARSHAL(params)
        __ITEM_JSON_MARSHAL(jsonrpc)
        END_JSON_MARSHAL
        
        BEGIN_JSON_UNMARSHAL
        __ITEM_JSON_UNMARSHAL(id)
        __ITEM_JSON_UNMARSHAL(method)
        __ITEM_JSON_UNMARSHAL(params)
        __ITEM_JSON_UNMARSHAL(jsonrpc)
        END_JSON_UNMARSHAL
        
        virtual bool validate() {
            return jsonrpc == Version;
        }
    };
    
    class Error : public json::JsonBase {
    public:
        int code;
        std::string message;
        boost::optional<json::tree> data;
    public:
        Error() : code(0) {}
        Error(int code, const std::string& message) :code(code), message(message) {}
 
        BEGIN_JSON_MARSHAL
        __ITEM_JSON_MARSHAL(code)
        __ITEM_JSON_MARSHAL(message)
        __ITEM_JSON_MARSHAL(data)
        END_JSON_MARSHAL
        
        BEGIN_JSON_UNMARSHAL
        __ITEM_JSON_UNMARSHAL(code)
        __ITEM_JSON_UNMARSHAL(message)
        __ITEM_JSON_UNMARSHAL(data)
        END_JSON_UNMARSHAL
    };
    
    class Response : public json::JsonBase {
    public:
        boost::optional<json::tree> result;
        boost::optional<Error> error;
        
        boost::optional<json::tree> id;
        std::string jsonrpc;
    public:
        Response() : jsonrpc(Version) {}

        BEGIN_JSON_MARSHAL
        __ITEM_JSON_MARSHAL(id)
        __ITEM_JSON_MARSHAL(result)
        __ITEM_JSON_MARSHAL_OBJ_OPT(error)
        __ITEM_JSON_MARSHAL(jsonrpc)
        END_JSON_MARSHAL
        
        BEGIN_JSON_UNMARSHAL
        __ITEM_JSON_UNMARSHAL(id)
        __ITEM_JSON_UNMARSHAL(result)
        __ITEM_JSON_UNMARSHAL_OBJ(error)
        __ITEM_JSON_UNMARSHAL(jsonrpc)
        END_JSON_UNMARSHAL
        
        virtual bool validate() {
            return jsonrpc == Version;
        }
    };
}

#endif /* spec_h */
