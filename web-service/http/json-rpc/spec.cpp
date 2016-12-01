//
//  spec.cpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/1/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#include "spec.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <stdexcept>

using namespace std;
using namespace boost;

namespace json_rpc {

    void Request::decode(const string& json) {
        jsonTree pt;
        istringstream is(json);
        property_tree::read_json(is, pt);
        
        auto jsonrpc = pt.get<string>("jsonrpc");
        if (Version != jsonrpc) {
            throw invalid_argument(jsonrpc);
        }
        m_id = pt.get_child_optional("id");
        m_method = pt.get<string>("method");
        m_params = pt.get_child_optional("params");
    }
    
    const string Request::encode() {
        jsonTree pt;
        
        pt.put("jsonrpc", Version);
        if (m_id) {
            pt.put_child("id", m_id.get());
        }
        pt.put("method", m_method);
        if (m_params) {
            pt.put_child("params", m_params.get());
        }

        ostringstream os;
        property_tree::write_json(os, pt, false);
        
        return os.str();
    }

    void Response::decode(const string& json) {
        jsonTree pt;
        istringstream is(json);
        property_tree::read_json(is, pt);
        
        auto jsonrpc = pt.get<string>("jsonrpc");
        if (Version != jsonrpc) {
            throw invalid_argument(jsonrpc);
        }
        m_id = pt.get_child_optional("id");
        m_result = pt.get_child_optional("result");
        m_error= pt.get_child_optional("error");
    }
    
    const string Response::encode() {
        jsonTree pt;
        
        pt.put("jsonrpc", Version);
        if (m_id) {
            pt.put_child("id", m_id.get());
        }
        if (m_result) {
            pt.put_child("result", m_result.get());
        }
        if (m_error) {
            pt.put_child("result", m_error.get());
        }
        
        ostringstream os;
        property_tree::write_json(os, pt, false);
        
        return os.str();
    }

}
