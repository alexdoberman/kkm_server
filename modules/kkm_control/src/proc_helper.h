#pragma once

#include <windows.h>

namespace proc_helper
{
    DWORD find(const wchar_t * szApp);
    int start(const wchar_t * sApp);
    int stop(int nPid);
}