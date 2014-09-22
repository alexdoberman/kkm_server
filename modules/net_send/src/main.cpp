#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>

#include <QString>
#include <QByteArray>
#include <QTextCodec>
#include <QRegExp>

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

inline QString decodeEntities( const QString& src )
{
    QString ret(src);
    QRegExp re("&#([0-9]+);");
    re.setMinimal(true);

    int pos = 0;
    while( (pos = re.indexIn(src, pos)) != -1 )
    {
        ret = ret.replace(re.cap(0), QChar(re.cap(1).toInt(0,10)));
        pos += re.matchedLength();
    }
    return ret;
}
inline QByteArray utf8ToDOS(std::string sIn)
{
	QByteArray baOut;
	QString str(sIn.c_str());
	QTextCodec *codec = QTextCodec::codecForName("IBM 866");
	if (codec)
		baOut  = codec->fromUnicode(str);
	else
		baOut = QByteArray(sIn.c_str());
	return baOut;
}
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

		//FILE * pFile = fopen ("input_lines.bin", "wb");

		for (size_t i = 2; i<argc; i++)
		{
                                   
			std::string sDecoded = base64_decode(argv[i]);
			if (sDecoded.empty())
				sDecoded = argv[i];

            QString  sDU = decodeEntities(QString(sDecoded.c_str()));
			QByteArray ba = utf8ToDOS(sDU.toStdString());
			//fwrite (ba.data() , sizeof(char), ba.length(), pFile);
			str.write(ba.data(), ba.length());
			str << "\r\n";
			//str << sDecoded << "\r\n";
			
			std::cout <<"send :'"<<sDecoded<<"'"<<std::endl;
		}
		str.flush();
		socket.shutdownSend();

		//fclose (pFile);

		//std::cout <<"----ANSVER----"<<std::endl;
		//Poco::StreamCopier::copyStream(str, std::cout);
		//std::cout <<"--------------"<<std::endl;
	}
	catch (Exception &ex)
    {
		std::cout <<"PosPrinter.Exception: err = "<<2001<<", errEx = "<<2001<<", desc = "<<ex.displayText() <<std::endl;
    }  
    catch (const std::exception &ex)
    {
		std::cout <<"PosPrinter.Exception: err = "<<2002<<", errEx = "<<2002<<", desc = "<<ex.what() <<std::endl;
    }
    return 0;
}