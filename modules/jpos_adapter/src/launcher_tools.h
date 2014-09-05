#pragma once
#include <vector>
#include <string>

#include "jposadapter.h"

namespace launcher_tools
{
	TResult launch_pos_tool(const std::string& sPath, const std::vector<std::string> & vsArgs, int& resultCode , std::string &sStdOut);
} //launcher_tools
