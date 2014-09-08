#pragma once

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    cout << "Usage: " << argv[0] << " <uri> <json_file>" << endl;
    cout << "       fetches the resource identified by <uri> and print it" << endl;
    return -1;
  }

  try
  {
    // prepare session
    URI uri(argv[1]);
    HTTPClientSession session(uri.getHost(), uri.getPort());

    // prepare path
    string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    // send request
    HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
		req.setChunkedTransferEncoding(true); 
		req.setContentType("text/plain");

	
		std::string sBody;
		ifstream f (argv[2]);
		if (f.is_open())
		{
			char ch;
			while(!f.eof())
			{
				f.get(ch);
				sBody.push_back(ch);
			}
			f.close();
		}

		req.setContentLength( sBody.length());
	session.sendRequest(req) << sBody;

    // get response
    HTTPResponse res;
    cout << res.getStatus() << " " << res.getReason() << endl;

    // print response
    istream &is = session.receiveResponse(res);
    StreamCopier::copyStream(is, cout);
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