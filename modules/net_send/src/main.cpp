#pragma once

#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"
#include <Poco/URI.h>
#include <Poco/Exception.h>

#include "base64.h"

using namespace Poco::Net;
using namespace Poco;
using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		cout << "Usage: " << argv[0] << " <uri> <base64_encode_line_1> <base64_encode_line_2> ..." << endl;
		cout << "       send line_1 line_2 ... to  <uri> and print it" << endl;
		return -1;
	}

	try
	{
		// prepare session
		URI uri(argv[1]);

		Poco::Net::SocketAddress sa(uri.getHost(), uri.getPort());
		Poco::Net::StreamSocket socket(sa);
		Poco::Net::SocketStream str(socket);

		for (size_t i = 2; i<argc; i++)
		{
			std::string sDecoded = base64_decode(argv[i]);
			if (sDecoded.empty())
				sDecoded = argv[i];

			str << sDecoded << "\r\n";
			std::cout <<"send :'"<<sDecoded<<"'"<<std::endl;
		}
		str.flush();
		socket.shutdownSend();


		//std::cout <<"----ANSVER----"<<std::endl;
		//Poco::StreamCopier::copyStream(str, std::cout);
		//std::cout <<"--------------"<<std::endl;
	}
	catch (Exception &ex)
    {
		cerr << ex.displayText() << endl;
    }  
    catch (const std::exception &ex)
    {
	    cerr << ex.what() << endl;
    }
    return 0;
}