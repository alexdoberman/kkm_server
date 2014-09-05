#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#ifdef JPOS_ADAPTER_EXPORTS
#    define JPOSADAPTER_DLL __declspec(dllexport) 
#else
#    define JPOSADAPTER_DLL __declspec(dllimport) 
#endif

typedef int32_t             TResult;
enum EJPOSResult
{
    kJPOSResult_Success = 0,
    kJPOSResult_Fail,               //Procedure failed for unknown reason;


    kJPOSResult_ForceDWORD                    = 0x7FFFFFFF,
};

struct TJPOSResult
{
	int32_t  nErr;
	int32_t  nErrEx;
	std::string sDesk;  
};

JPOSADAPTER_DLL TResult jpos_print(const std::string & sDevName, const std::vector<std::string>& vLines, TJPOSResult& ret);
