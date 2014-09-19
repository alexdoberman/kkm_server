#include "request_processor.h"
#include "kkm_server_def.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "request_types.h"
#include "request_parser.h"
#include "kkm_server_engine.h"
#include "reply_writer.h"

CRequestProcessor::CRequestProcessor(TKKMConfig cfg)
	: m_cfg(cfg)
{}

CRequestProcessor::~CRequestProcessor()
{}

const TKKMConfig& CRequestProcessor::get_cfg() const
{
	return m_cfg;
}

void CRequestProcessor::process(std::istream & ssIn, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	do
	{
		THeader header;
		boost::property_tree::ptree pt;
		try
		{
			boost::property_tree::read_json(ssIn, pt);
			nRet = kkm_request_parser::parse_header(pt, header);
			if (nRet != kResult_Success)
			{
				LOG_ERR("RequestProcessor::process_request, parse_header  err: " << nRet);
				break;
			}
		}
		catch (std::exception const& e)
		{
			LOG_ERR("kkm_request_engine::process_request err: " << e.what());
			nRet = kResult_ParseError;
			break;
		}
		
		if (header.nTypeOp == OP_TYPE_CHECK)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TCheck check;
			nRet = kkm_request_parser::parse_check(pt, check);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_check err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_check(m_cfg, header, check, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_RETURN_CHECK)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TReturnCheck check;
			nRet = kkm_request_parser::parse_return_check(pt, check);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_return_check err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_return_check(m_cfg, header, check, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_CANCEL_CHECK)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TCancellationCheck check;
			nRet = kkm_request_parser::parse_cancellation_check(pt, check);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_cancellation_check err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_cancel_check(m_cfg, header, check, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_REPORT_Z)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TReportZ report;
			nRet = kkm_request_parser::parse_report_z(pt, report);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_report_z err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_report_z(m_cfg, header, report, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_REPORT_X)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TReportX report;
			nRet = kkm_request_parser::parse_report_x(pt, report);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_report_x err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_report_x(m_cfg, header, report, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_GET_DEVICE)
		{
			//safe operation
			kkm_request_engine::process_request_get_device(m_cfg, header, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_GET_MODE)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			kkm_request_engine::process_request_get_mode(m_cfg, header, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_RESET_MODE)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			kkm_request_engine::process_request_reset_mode(m_cfg, header, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_ANNULATE_CHECK)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			kkm_request_engine::process_request_annulate_check(m_cfg, header, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_EXEC_COMMAND)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TExecCommand cmd;
			nRet = kkm_request_parser::parse_exec_command(pt, cmd);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_exec_command err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_exec_command(m_cfg, header, cmd, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_QUERY_KKM_STATE)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			kkm_request_engine::process_request_get_kkm_state(m_cfg, header, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_PRINT_PLAIN_TEXT)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TPrintText text;
			nRet = kkm_request_parser::parse_print_text(pt, text);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_print_text err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_print_plain_text(m_cfg, header, text, ssOut);
		}
		else if (header.nTypeOp == OP_TYPE_JPOS_PRINT)
		{
			//unsafe operation
			Poco::Mutex::ScopedLock guard(get_lock(header.sDevice));

			TPrintText text;
			nRet = kkm_request_parser::parse_print_text(pt, text);
			if (nRet != kResult_Success)
			{
				LOG_ERR("kkm_request_engine::parse_print_text err: " << nRet);
				break;
			}
			kkm_request_engine::process_request_jpos_print_text(m_cfg, header, text, ssOut);
		}
		else
		{
			//unknown request
			nRet = kResult_UnknownRequest;
			LOG_ERR("kkm_request_engine:: unknown request err: " << nRet);
			break;
		}
    }
    while(false);

	if (nRet != kResult_Success)
	{
		TErrCode reply = {0, nRet};
		boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
		boost::property_tree::write_json(ssOut, pt);
	}
}

TMutex& CRequestProcessor::get_lock(std::string sPortId)
{
	Poco::Mutex::ScopedLock guard(m_mtx);
	std::map <std::string , std::unique_ptr<TMutex>>::iterator it =  m_mapMtx.find(sPortId);
	if (it == m_mapMtx.end() )
	{
		auto r = m_mapMtx.insert(std::make_pair(sPortId, std::unique_ptr<TMutex>(new TMutex)));
		return (*(r.first)->second);
	}
	return (*it->second);
}
