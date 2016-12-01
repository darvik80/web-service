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
    
    typedef boost::shared_ptr<boost::property_tree::ptree> jsonTree;
    
    class IMarshaller {
    public:
        virtual const jsonTree marshal() = 0;
    };

    class IUnmarshaller {
    public:
        const jsonTree marshal();
    };

    class Request {
        std::string m_method;
        boost::optional<jsonTree> m_params;
        
        boost::optional<jsonTree> m_id;
    public:
        void decode(const std::string& json);
        const std::string encode();
        
        const std::string& method() {
            return m_method;
        }
        const boost::optional<jsonTree>& params() {
            return m_params;
        }
        const boost::optional<jsonTree>& id() {
            return m_id;
        }
    };
    
    class Error {
        int code;
        std::string message;
        boost::optional<jsonTree> data;
    };
    
    class Response {
        boost::optional<jsonTree> m_result;
        boost::optional<Error> m_error;
        
        boost::optional<jsonTree> m_id;
    public:
        Response()
        void decode(const std::string& json);
        const std::string encode();
        
        const boost::optional<jsonTree>& result() {
            return m_result;
        }
        const boost::optional<jsonTree>& error() {
            return m_error;
        }
        
        const boost::optional<jsonTree>& id() {
            return m_id;
        }
    };
}

#endif /* spec_h */
