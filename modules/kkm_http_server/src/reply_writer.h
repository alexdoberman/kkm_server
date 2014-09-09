#pragma once

#include <boost/property_tree/ptree.hpp>

#include <types.h>
#include "reply_types.h"

namespace kkm_reply_writer
{
    boost::property_tree::ptree  write_reply(const TErrCode& answer);
	boost::property_tree::ptree  write_reply(const TDevDesc& answer);
	boost::property_tree::ptree  write_reply(const TKKMModeReply& answer);
	boost::property_tree::ptree  write_reply(const TExecCommandReply& answer);
	boost::property_tree::ptree  write_reply(const TKKMStateReply & answer);
	boost::property_tree::ptree  write_reply(const TExecPosUtilReply & answer);
};
