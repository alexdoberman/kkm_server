#pragma once

#define LOG_MODULE                         ("kkm_driver")
#include "logmacros_def.h"

namespace defs
{
	static const char cENQ                = 0x05; //Query
	static const char cACK                = 0x06; //Asc
	static const char cSTX                = 0x02; //Begin text
	static const char cETX                = 0x03; //End text
	static const char cEOT                = 0x04; //End transmit
	static const char cNAK                = 0x15; //Negative
	static const char cDLE                = 0x10; //Mask symbol

    //timeouts
    static const int  nReadDataTimeout    = 2000;
    static const int  nSendACKTimeout     = 500;
    static const int  nSendENQTimeout     = 500;
    static const int  nReadENQTimeout     = 10000;
    static const int  nReadEOTTimeout     = 10000;
	static const int  nReadByteTimeout    = 500;
}
