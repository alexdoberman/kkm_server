#pragma once
#include <vector>
#include <string>

#include "jposadapter.h"

namespace launcher_tools
{
	TResult launch_pos_tool(const std::string& cCmd, int& resultCode , std::vector<std::string> &vsStdOut);
} //launcher_tools
