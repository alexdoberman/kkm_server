#include "request_parser.h"
#include "kkm_server_def.h"

#include <exception>

TResult kkm_request_parser::parse_header(boost::property_tree::ptree pt, THeader& header)
{
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			header.nTypeOp = pt.get<int>("header.idOp");
			header.sDevice = pt.get("header.idDev", std::string(""));
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_header err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_check(boost::property_tree::ptree pt, TCheck& check)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {

			int nDiscSign = pt.get("body.discount_sign", -1);
			if (nDiscSign != -1)
			{
				check.bUseAllDiscount        = true;
				check.discount.kSign         = (nDiscSign==0)?kDiscountSign_Minus:kDiscountSign_Plus;
				
				int nDiscType = pt.get("body.discount_type", 0);
				check.discount.kDiscountType = (nDiscType==0)?kDiscountType_Percent:kDiscountType_Summ;

				int nDiscVal = pt.get("body.discount_val", 0);
				check.discount.nValue = nDiscVal;
			}
			else
			{
				check.bUseAllDiscount        = false;
			}
			check.nCheckSumm   = pt.get<uint32_t>("body.summ");
			check.nPassword    = pt.get<uint32_t>("body.password");
			check.nTypePayment = pt.get<uint8_t>("body.type_payment");

			pt = pt.get_child("body.items");
			ptree::const_iterator end = pt.end();
			for (ptree::const_iterator it = pt.begin(); it != end; ++it) 
			{
				TCheckItem checkItem;

				checkItem.sPrintLine  =	it->second.get<std::string>("line");
				checkItem.nPrice      =	it->second.get<uint32_t>("price");
				checkItem.nQuantity   =	it->second.get<uint32_t>("quantity");
				checkItem.nDepartment =	it->second.get<uint8_t>("department");

				int nDiscSign = it->second.get("discount_sign", -1);
				if (nDiscSign != -1)
				{
					checkItem.bUseDiscount          = true;
					checkItem.discount.kSign         = (nDiscSign==0)?kDiscountSign_Minus:kDiscountSign_Plus;
				
					int nDiscType = it->second.get("discount_type", 0);
					checkItem.discount.kDiscountType = (nDiscType==0)?kDiscountType_Percent:kDiscountType_Summ;

					int nDiscVal = it->second.get("discount_val", 0);
					checkItem.discount.nValue = nDiscVal;
				}
				else
				{
					checkItem.bUseDiscount        = false;
				}
				check.vCheckItem.push_back(checkItem);
		    }
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_check err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_cancellation_check(boost::property_tree::ptree pt, TCancellationCheck& check)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			check.nTypePayment = pt.get<uint8_t>("body.type_payment");
			check.nPassword  = pt.get<uint32_t>("body.password");

			pt = pt.get_child("body.items");
			ptree::const_iterator end = pt.end();
			for (ptree::const_iterator it = pt.begin(); it != end; ++it) 
			{
				TCancellationCheckItem checkItem;

				checkItem.sPrintLine  =	it->second.get<std::string>("line");
				checkItem.nPrice      =	it->second.get<uint32_t>("price");
				checkItem.nQuantity   =	it->second.get<uint32_t>("quantity");
				check.vCheckItem.push_back(checkItem);
		    }
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_cancellation_check err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_return_check(boost::property_tree::ptree pt, TReturnCheck& check)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			check.nTypePayment = pt.get<uint8_t>("body.type_payment");
			check.nPassword  = pt.get<uint32_t>("body.password");

			pt = pt.get_child("body.items");
			ptree::const_iterator end = pt.end();
			for (ptree::const_iterator it = pt.begin(); it != end; ++it) 
			{
				TReturnCheckItem checkItem;

				checkItem.sPrintLine  =	it->second.get<std::string>("line");
				checkItem.nPrice      =	it->second.get<uint32_t>("price");
				checkItem.nQuantity   =	it->second.get<uint32_t>("quantity");
				check.vCheckItem.push_back(checkItem);
		    }
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_return_check err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_report_z(boost::property_tree::ptree pt, TReportZ& report)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			report.nPassword  = pt.get<uint32_t>("body.password");
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_report_z err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_report_x(boost::property_tree::ptree pt, TReportX& report)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			report.nPassword    = pt.get<uint32_t>("body.password");
			report.cReportType  = pt.get<uint8_t>("body.report_type");
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_report_x err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_exec_command(boost::property_tree::ptree pt, TExecCommand& cmd)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			cmd.sExecCommand    = pt.get<std::string>("body.exec_command");
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_exec_command err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

TResult kkm_request_parser::parse_print_text(boost::property_tree::ptree pt, TPrintText& cmd)
{
	using boost::property_tree::ptree;
	TResult nRet    = kResult_Success;
	do
	{
        try
        {
			pt = pt.get_child("body");
			ptree::const_iterator end = pt.end();
			for (ptree::const_iterator it = pt.begin(); it != end; ++it) 
			{
				std::string sLine = it->second.get<std::string>("");
				cmd.vStringLine.push_back(sLine);
		    }
		}
        catch (std::exception const& e)
        {
			LOG_ERR("kkm_request_parser::parse_print_text err: " << e.what());
			nRet = kResult_ParseError;
        }
    }
    while(false);
    return nRet;
}

