//
//  application.cpp
//  web-service
//
//  Created by Ivan Kishchenko on 12/26/16.
//  Copyright © 2016 Ivan Kishchenko. All rights reserved.
//

#include "application.hpp"

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>

#include "http/server.hpp"

#include "logger.hpp"
#include "http/json-rpc/JSONRPC2Handler.hpp"

using namespace std;
using namespace boost;

void Application::create(int argc, const char * argv[]) {
    std::string appName = boost::filesystem::basename(argv[0]);
    
    program_options::options_description desc("Options");
    desc.add_options()
    ("help", "Print help messages")
    ("config", program_options::value<string>()->default_value("./config.json"), "config file path")
    ("http_server", program_options::value<string>()->default_value("localhost"), "http server address, default is localhost")
    ("http_port", program_options::value<string>()->default_value("8080"), "http server address, default is 8080")
    ("http_webroot", program_options::value<string>()->default_value("/Users/darvik/OSXProjects/web-service/webroot"), "http root path");

    program_options::store(program_options::parse_command_line(argc, argv, desc), m_vm);
    
    if (m_vm.count("help")) {
        cout << "Basic Command Line Parameter App" << endl << desc << endl;
        
        return;
    }
    program_options::notify(m_vm);

    cout << "App: " << appName << endl;
    if (m_vm.count("config")) {
        LOG_INFO << "config: " << m_vm["config"].as<string>() ;
        //TODO: parse config
    }

    LOG_INFO << "Application started";
}

void Application::run() {
    asio::io_service io;
    asio::signal_set signals(io, SIGTERM, SIGINT, SIGQUIT);
    // Start an asynchronous wait for one of the signals to occur.
    
    signals.async_wait(
                        [this](boost::system::error_code ec, int /*signo*/)
                        {
                            if (!ec) {
                                LOG_INFO << " A signal(SIGINT) occurred.";
                            }

                        });
    
    http::server::server server(io, m_vm["http_server"].as<string>(), m_vm["http_port"].as<string>(), boost::make_shared<http::server::request_handler>(m_vm["http_webroot"].as<string>()));
    server.register_handler("POST", "/rpc", boost::make_shared<JSONRPC2Handler>());
    
    io.run();
}

void Application::destroy() {
    LOG_INFO << "Application stopped";
}
