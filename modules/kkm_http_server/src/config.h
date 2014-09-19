#pragma once
#include <stdint.h>

struct TKKMConfig
{
    unsigned short nKKM_HTTP_Port;
    uint32_t       nKKMPassword;
	std::string    sResponseContentType;   
	bool           bResponseWrapHack;
};