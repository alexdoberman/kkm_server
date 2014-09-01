#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "kkm_driver_type.h"
#include "kkm_driver.h"

struct TErrCode
{
    int        nKKMErr;
    TResult    nErr;
};


//for get device 
typedef   std::vector <TPortInfo> TDevDesc;

//for get mode
struct TKKMModeReply
{
	uint8_t    nMode;
	uint8_t    nSubMode;
	uint8_t    nFlag;

	int        nKKMErr;
    TResult    nErr;
};

//for exec command

struct TExecCommandReply
{
	std::string  sReply;
    TResult      nErr;
};

//for kkm_state
struct TKKMStateReply
{
	std::string  sSerialNumber;
	std::string  sKKMVersion;
	uint8_t      cModel;

	int        nKKMErr;
    TResult    nErr;
};


