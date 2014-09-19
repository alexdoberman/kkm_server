#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/CountingStream.h>
#include <Poco/NullStream.h>
#include <Poco/StreamCopier.h>

#include "kkm_server_def.h"
#include "request_processor.h"
#include "config.h"

#include <sstream>



using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::CountingInputStream;
using Poco::StreamCopier;

class KKMRequestHandler: public HTTPRequestHandler
{
public:
	KKMRequestHandler( CRequestProcessor& refProc) 
		: m_refProc (refProc)
	{
	}
	
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		
		std::cout << "Request from : "<< request.clientAddress().toString() << std::endl;
		std::cout << "	Method: " << request.getMethod() << std::endl;
		std::cout << "	URI: " << request.getURI() << std::endl;
		
		NameValueCollection::ConstIterator it = request.begin();
		NameValueCollection::ConstIterator end = request.end();
		for (; it != end; ++it)
		{
			std::cout <<"	"<< it->first << ": " << it->second << std::endl;
		}

		std::stringstream ssRequest;
		CountingInputStream istr(request.stream());
		StreamCopier::copyStream(istr, ssRequest);
		
		LOG_DBG("Request from: "<< request.clientAddress().toString() );
		LOG_DBG("	Method: "<< request.getMethod()  );
		LOG_DBG("	URI: "<< request.getURI());
		LOG_DBG("Request stream: -----------------------------------------------------------------");
		LOG_DBG(ssRequest.str());
		LOG_DBG("---------------------------------------------------------------------------------");

		response.setChunkedTransferEncoding(true); 
		response.setContentType(m_refProc.get_cfg().sResponseContentType);
		std::ostream& ostr = response.send();

		if (m_refProc.get_cfg().bResponseWrapHack)
			ostr<< "response(";

		m_refProc.process(ssRequest, ostr);

		if (m_refProc.get_cfg().bResponseWrapHack)
			ostr<< ")";

	}
private:
	CRequestProcessor& m_refProc;
};


class KKMRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	KKMRequestHandlerFactory(TKKMConfig cfg)
		:m_cProc(cfg)
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		return new KKMRequestHandler(m_cProc);
	}
private:
	CRequestProcessor m_cProc;
};