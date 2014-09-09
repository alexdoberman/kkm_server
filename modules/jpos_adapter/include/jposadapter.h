#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#ifdef JPOS_ADAPTER_EXPORTS
#    define JPOSADAPTER_DLL __declspec(dllexport) 
#else
#    define JPOSADAPTER_DLL __declspec(dllimport) 
#endif
#include <types.h>

struct TJPOSResult
{
	int32_t  nErr;
	int32_t  nErrEx;
	std::string sDesk;  
};

JPOSADAPTER_DLL TResult jpos_print(const std::string & sDevName, const std::vector<std::string>& vLines, TJPOSResult& ret);
