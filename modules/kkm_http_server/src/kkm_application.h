#pragma once

#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/HTTPServerParams.h"

#include "kkm_server.h"
#include "config.h"



using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;

class KKMServer: public Poco::Util::ServerApplication
{
public:
	KKMServer(){}

	~KKMServer(){}

protected:
	void initialize(Application& self)
	{
		loadConfiguration("cfg//kkm_server.properties"); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
		
	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	int main(const std::vector<std::string>& args)
	{
		//read config
		TKKMConfig cfg;
		cfg.nKKM_HTTP_Port  = (unsigned short) config().getInt("KKM_HTTP_Server.port", 9980);
		cfg.nKKMPassword    = (uint32_t) config().getInt("KKM_HTTP_Server.password", 0);

		std::cout << "KKM Server start, port : "<<cfg.nKKM_HTTP_Port <<std::endl;	
		// set-up a server socket
		ServerSocket svs(cfg.nKKM_HTTP_Port);
		// set-up a HTTPServer instance
		HTTPServer srv(new KKMRequestHandlerFactory(cfg), svs, new HTTPServerParams);
		// start the HTTPServer
		srv.start();
		// wait for CTRL-C or kill
		waitForTerminationRequest();
		// Stop the HTTPServer
		srv.stop();
		std::cout << "KKM Server stop." <<std::endl;
		return Application::EXIT_OK;
	}
};