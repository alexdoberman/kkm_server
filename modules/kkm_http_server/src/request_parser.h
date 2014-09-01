#pragma once
#include <sstream>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "kkm_driver_type.h"
#include "request_types.h"

namespace kkm_request_parser
{
    TResult parse_header(boost::property_tree::ptree pt, THeader& header);
    TResult parse_check(boost::property_tree::ptree pt, TCheck& check);
	TResult parse_return_check(boost::property_tree::ptree pt, TReturnCheck& check);
	TResult parse_cancellation_check(boost::property_tree::ptree pt, TCancellationCheck& check);
	TResult parse_report_z(boost::property_tree::ptree pt, TReportZ& report);
	TResult parse_report_x(boost::property_tree::ptree pt, TReportX& report);
	TResult parse_exec_command(boost::property_tree::ptree pt, TExecCommand& cmd);
	TResult parse_print_text(boost::property_tree::ptree pt, TPrintText& cmd);
};
