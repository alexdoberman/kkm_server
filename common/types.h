#pragma once
#include <stdint.h>

typedef int32_t             TResult;

enum EResult
{
    kResult_Success = 0,
    kResult_Fail,               //Procedure failed for unknown reason;
	kResult_ErrorOpenPort,
	kResult_ErrorConfigurePort,
	kResult_ReadError,
	kResult_Timeout,
	kResult_ErrorAnswer,


	kResult_ParseError,
	kResult_UnknownRequest,

    kResult_ForceDWORD                    = 0x7FFFFFFF,
};
