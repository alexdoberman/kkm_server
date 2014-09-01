#pragma once
#include <stdint.h>

typedef int32_t             TResult;

enum EKKMResult
{
    kKKMResult_Success = 0,
    kKKMResult_Fail,               //Procedure failed for unknown reason;
	kKKMResult_ErrorOpenPort,
	kKKMResult_ErrorConfigurePort,
	kKKMResult_ReadError,
	kKKMResult_Timeout,
	kKKMResult_ErrorAnswer,


	kKKMResult_ParseError,
	kKKMResult_UnknownRequest,

    kKKMResult_ForceDWORD                    = 0x7FFFFFFF,
};
