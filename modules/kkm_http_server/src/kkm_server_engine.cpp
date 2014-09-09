#include "kkm_server_engine.h"
#include "kkm_server_def.h"

#include <stdio.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "request_parser.h"
#include "request_engine.h"
#include "reply_writer.h"
#include "jposadapter.h"

TResult  kkm_request_engine::process_request_check(const TKKMConfig& cfg, const THeader &header, const TCheck & check, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;
	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_check, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::check(spKKMControl, check, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();

	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_cancel_check(const TKKMConfig& cfg, const THeader &header, const TCancellationCheck & check, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_check, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::cancellation_check(spKKMControl, check, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_return_check(const TKKMConfig& cfg, const THeader &header, const TReturnCheck & check, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_return_check, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::return_check(spKKMControl, check, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_report_x(const TKKMConfig& cfg, const THeader &header, const TReportX & report, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_report_x, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::report_x(spKKMControl, report, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_report_z(const TKKMConfig& cfg, const THeader &header, const TReportZ & report, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_report_z, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::report_z(spKKMControl, report, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_get_device(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	std::vector <TPortInfo> vPortInfo;
	getPortInfo(vPortInfo);

	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(vPortInfo);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_get_mode(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut)
{
	TResult nRet      = kResult_Success;

	uint8_t cMode     = 0x00;
	uint8_t cSubMode  = 0x00;
	uint8_t nFlag     = 0x00;  
	int nKKMErr       = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_get_mode, connect  err: " << nRet);
			break;
		}

		nRet = spKKMControl->getMode(cMode, cSubMode, nFlag, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TKKMModeReply reply = {cMode, cSubMode, nFlag, nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_reset_mode(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_reset_mode, connect  err: " << nRet);
			break;
		}
		
		nRet = spKKMControl->resetMode(nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_annulate_check(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_annulate_check, connect  err: " << nRet);
			break;
		}
		
		nRet = spKKMControl->cancelCheck(nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_exec_command(const TKKMConfig& cfg, const THeader &header, const TExecCommand & cmd , std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TExecCommandReply reply;
	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_exec_command, connect  err: " << nRet);
			break;
		}

		nRet = spKKMControl->exec_command(cmd.sExecCommand, reply.sReply);
	}
	while (false);
	spKKMControl->disconnect();

	reply.nErr = nRet;
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);
	return nRet;
}

TResult kkm_request_engine::process_request_get_kkm_state(const TKKMConfig& cfg, const THeader &header, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TKKMStateReply reply;
		reply.cModel = 0;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_get_kkm_state, connect  err: " << nRet);
			break;
		}
		
		TKKMState kkmState;	
		nRet = spKKMControl->getKKMState(kkmState, nKKMErr);
		if (nRet == kResult_Success)
		{
			char buff[10];
			sprintf(buff, "%u", kkmState.nKKMVersion);
			reply.sKKMVersion = buff;
			
			sprintf(buff, "%u", kkmState.nSerialNumber);
			reply.sSerialNumber = buff;

			reply.cModel = kkmState.cModel;
		}

	}
	while (false);
	spKKMControl->disconnect();
	
	reply.nKKMErr = nKKMErr;
	reply.nErr    = nRet;


	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);
	return nRet;
}

TResult kkm_request_engine::process_request_print_plain_text(const TKKMConfig& cfg, const THeader &header, const TPrintText& text, std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	int nKKMErr     = 0;

	TKKMInfo kkminfo;
	memset(&kkminfo, 0, sizeof(kkminfo));
	strcpy(kkminfo.pszPortName, header.sDevice.c_str());
	kkminfo.nPassword = cfg.nKKMPassword;

	TSmartKKMControl spKKMControl = createKKMControl();
	do
	{
		nRet = spKKMControl->connect(kkminfo);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_print_plain_text, connect  err: " << nRet);
			break;
		}
		
		nRet = kkm_request::print_plain_text(spKKMControl, text, nKKMErr);
	}
	while (false);
	spKKMControl->disconnect();


	TErrCode reply = {nKKMErr, nRet};
	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}

TResult kkm_request_engine::process_request_jpos_print_text(const TKKMConfig& cfg, const THeader &header, const TPrintText& text,  std::ostream& ssOut)
{
	TResult nRet    = kResult_Success;
	TJPOSResult ret;
	do
	{
		nRet = jpos_print(header.sDevice, text.vStringLine, ret);
		if(nRet != kResult_Success)
		{
			LOG_ERR("kkm_request_engine::process_request_jpos_print_text, print  err: " << nRet);
			break;
		}
	}
	while(false);

	TExecPosUtilReply reply;
		reply.nErrPos   = ret.nErr;
		reply.nExErrPos = ret.nErrEx;
		reply.sDesc     = ret.sDesk;
		reply.nErr      = nRet; 

	boost::property_tree::ptree pt = kkm_reply_writer::write_reply(reply);
	boost::property_tree::write_json(ssOut, pt);

	return nRet;
}
