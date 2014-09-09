#include "kkm_control.h"
#include "kkm_driver_def.h"
#include "str_conv.h"
#include "kkm_api.h"

#include <algorithm>
#include <sstream>
#include <iterator>
#include <iomanip>

#include <QByteArray>
#ifdef _WIN32
    #include <windows.h>
#else 
    #include <unistd.h>
#endif

void m_sleep(uint32_t nMSec)
{
#ifdef _WIN32
    Sleep(nMSec);
#else 
    usleep(nMSec*1000);
#endif
}

CKKMControl::CKKMControl()
	: m_pSerial(nullptr)
	, m_bConnected(false)
{
	m_pSerial = new QSerialPort;
}

CKKMControl::~CKKMControl()
{
	delete m_pSerial;
}

TResult CKKMControl::connect(TKKMInfo kkmDesc)
{
	m_kkmDesc = kkmDesc;

    TResult nRet = kResult_Success;
	m_bConnected = false;

    LOG_DBG("connect ==>");
    do
    {
        m_pSerial->setPortName(m_kkmDesc.pszPortName);
        if (m_pSerial->open(QIODevice::ReadWrite) == false) 
        {
			LOG_ERR("Error open port = '"<<m_kkmDesc.pszPortName<<"'"<<m_pSerial->errorString().toStdString());
			nRet = kResult_ErrorOpenPort;
			break;
		}

		if (m_pSerial->setBaudRate(115200)
                && m_pSerial->setDataBits(QSerialPort::Data8)
                && m_pSerial->setParity(QSerialPort::NoParity)
				&& m_pSerial->setStopBits(QSerialPort::OneStop)
				&& m_pSerial->setFlowControl(QSerialPort::NoFlowControl) == false) 
        {
			LOG_ERR("Error configure port = '"<<m_kkmDesc.pszPortName<<"'"<<m_pSerial->errorString().toStdString());
			m_pSerial->close();
			nRet = kResult_ErrorConfigurePort;
			break;
        } 
		
		LOG_INF("Connected to "<<m_kkmDesc.pszPortName<<" :"<< m_pSerial->baudRate()<<", " <<m_pSerial->dataBits() 
			<< ", "<<m_pSerial->parity()<<", "<<m_pSerial->stopBits()<<", "<<m_pSerial->flowControl());
		m_bConnected = true;
    }
    while (false);

	LOG_DBG("connect <==");
    return nRet;

}

TResult CKKMControl::disconnect()
{
	LOG_DBG("disconnect ==>");
    m_pSerial->close();
	LOG_DBG("disconnect <==");
	return kResult_Success;
}

TResult CKKMControl::exec_command(const std::string & sCmd, std::string& sAnsver)
{
	LOG_DBG("CKKMControl::exec_command ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand = hex2ba(sCmd.c_str());
	QByteArray baAnswer;
	
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);

	sAnsver = ba2hex(baAnswer);
	LOG_DBG("CKKMControl::exec_command <==");

	return nRet;
}

TResult CKKMControl::setMode(uint8_t cMode, uint32_t nPassword, int& nErrCode)
{
	LOG_DBG("setMode ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x56);
		baCommand.append(cMode);
		baCommand.append(val2BCD(nPassword, 8));
	QByteArray baAnswer;
	
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("setMode <==");

	return nRet;
}

TResult CKKMControl::resetMode(int& nErrCode)
{
	LOG_DBG("resetMode ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x48);
	QByteArray baAnswer;
	
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode =(TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("resetMode <==");

	return nRet;
}

TResult CKKMControl::getMode(uint8_t& cMode, uint8_t& cSubMode, uint8_t& nFlag, int& nErrCode)    
{
	LOG_DBG("getMode ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x45);
	QByteArray baAnswer;
	
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
		{
			cMode    = baAnswer.at(1) & 0x0F;
			cSubMode = (baAnswer.at(1) & 0xF0) >> 4;
			nFlag    = baAnswer.at(2);
		}
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("getMode <==");
	return nRet;
}

TResult CKKMControl::getKKMState(TKKMState& kkmState, int& nErrCode)
{
	LOG_DBG("getKKMState ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x3F);
	QByteArray baAnswer;
	
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	
	TKKMStateRaw kkmStateRaw;
	memset(&kkmStateRaw, 0, sizeof(kkmStateRaw));
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 30 && baAnswer.at(0) == (char)0x44)
		{
			memcpy(&kkmStateRaw, baAnswer.data(), sizeof(kkmStateRaw));
			//fill kkmState
			
			kkmState.nKKMVersion   = BCD2val(QByteArray::fromRawData((const char *)kkmStateRaw.vKKMVersion,
				                                                      sizeof (kkmStateRaw.vKKMVersion)));
			kkmState.nSerialNumber = BCD2val(QByteArray::fromRawData((const char *)kkmStateRaw.vSerialNumber, 
				                                                      sizeof (kkmStateRaw.vSerialNumber)));
			kkmState.cModel        = kkmStateRaw.cModel;
			kkmState.cMode         = kkmStateRaw.cMode;
			kkmState.cCheckState   = kkmStateRaw.cCheckState;
		}
		else if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
		{
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		}
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("getKKMState <==");
	return nRet;
}


TResult CKKMControl::printLine(const std::string & sText, int& nErrCode)
{
	LOG_DBG("printLine ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x4C);
		baCommand.append(utf8ToDOS(sText));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("printLine <==");
	return nRet;
}

TResult CKKMControl::openSession(const std::string & sText, int& nErrCode)
{
	LOG_DBG("openSession ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x9A);
		baCommand.append((char)0x00);
		baCommand.append(utf8ToDOS(sText));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("openSession <==");
	return nRet;
}

TResult CKKMControl::openCheck(uint8_t cTestMode, uint8_t cCheckType,  int& nErrCode)
{
	LOG_DBG("openCheck ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x92);
		baCommand.append(cTestMode);
		baCommand.append(cCheckType);
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("openCheck <==");
	return nRet;
}

TResult CKKMControl::cancelCheck( int& nErrCode)
{
	LOG_DBG("cancelCheck ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x59);
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode)baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("cancelCheck <==");
	return nRet;
}

TResult CKKMControl::cashIn(uint8_t cTestMode, uint32_t nSumm, int& nErrCode)
{
	LOG_DBG("cashIn ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x49);
		baCommand.append(cTestMode);
		baCommand.append(val2BCD(nSumm, 10));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode =(TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("cashIn <==");
	return nRet;
}

TResult CKKMControl::cashOut(uint8_t cTestMode, uint32_t nSumm, int& nErrCode)
{
	LOG_DBG("cashOut ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x4F);
		baCommand.append(cTestMode);
		baCommand.append(val2BCD(nSumm, 10));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode)baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("cashOut <==");
	return nRet;
}

TResult CKKMControl::registaration(uint8_t cTestMode, uint32_t nPrice, uint32_t nQuantity, uint8_t  nDepartment, int& nErrCode)
{
	LOG_DBG("registaration ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x52);
		baCommand.append(cTestMode);
		baCommand.append(val2BCD(nPrice, 10));
		baCommand.append(val2BCD(nQuantity, 10));
		baCommand.append(val2BCD(nDepartment, 2));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("registaration <==");
	return nRet;
}

TResult CKKMControl::return_(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode)
{
	LOG_DBG("return_ ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x57);
		baCommand.append(cTestMode);
		baCommand.append(val2BCD(nPrice, 10));
		baCommand.append(val2BCD(nQuantity, 10));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("return_ <==");
	return nRet;
}

TResult CKKMControl::cancellation(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode)
{
	LOG_DBG("cancellation ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x41);
		baCommand.append(cTestMode);
		baCommand.append(val2BCD(nPrice, 10));
		baCommand.append(val2BCD(nQuantity, 10));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode =(TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("cancellation <==");
	return nRet;
}

TResult CKKMControl::discount(uint8_t cTestMode, uint8_t cArea, uint8_t cType, uint8_t cSign, uint32_t nValue, int& nErrCode)
{
	LOG_DBG("discount ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x43);
		baCommand.append(cTestMode);
		baCommand.append(cArea);
		baCommand.append(cType);
		baCommand.append(cSign);
	if (cType)
		baCommand.append(val2BCD(nValue, 10));
	else
		baCommand.append(val2BCD(nValue, 6));

	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode =(TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("discount <==");
	return nRet;
}

TResult CKKMControl::closeCheck(uint8_t cTestMode, uint8_t cTypeClose, uint32_t nSumm,  int& nErrCode)
{
	LOG_DBG("closeCheck ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x4A);
		baCommand.append(cTestMode);
		baCommand.append(cTypeClose);
		baCommand.append(val2BCD(nSumm, 10));
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer, 20000);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode)baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}
	LOG_DBG("closeCheck <==");
	return nRet;
}

TResult CKKMControl::reportZ(int& nErrCode)
{
	//todo
	LOG_DBG("reportZ ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x5A);
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer, 40000);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode =(TKKMErrorCode) baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}


	//pool state
	uint8_t cMode     = 0x00;
	uint8_t cSubMode  = 0x00;
	uint8_t nFlag     = 0x00;  
	

	while(nRet == kResult_Success && nErrCode == 0)
	{
		m_sleep(500);
		int nErr          = 0;
		nRet = getMode(cMode, cSubMode, nFlag, nErr);
		if ((cMode != 3) || (cSubMode != 2))
			break;
	}

	if (nRet == kResult_Success && nErrCode == 0)
	{
		if ((cMode != 7) || (cSubMode != 1))
		{
			if(nFlag & 0x01)
				nErrCode = EX_KKM_ERROR_OUT_PAPER;
			else if (nFlag & 0x02)
				nErrCode = EX_KKM_ERROR_KKM_NOT_CONNECTED;
			else if (nFlag & 0x04)
				nErrCode = EX_KKM_ERROR_PRINTER_CUT_MECH;
		}
		else
		{
			while(nRet == kResult_Success)
			{
				m_sleep(500);
				int nErr          = 0;
				nRet = getMode(cMode, cSubMode, nFlag, nErr);
				if ((cMode != 7) || (cSubMode != 1))
					break;
			}
		}	
	}
	LOG_DBG("reportZ <==");
	return nRet;
}

TResult CKKMControl::reportX(uint8_t cReportType, int& nErrCode)
{
	LOG_DBG("reportX ==>");
	TResult nRet = kResult_Success;

	QByteArray baCommand;
		baCommand.append(0x67);
		baCommand.append(cReportType);
	
	QByteArray baAnswer;
	CKKMApi kkmApi(m_pSerial, m_kkmDesc);
	nRet = kkmApi.exec_command(baCommand, baAnswer, 40000);
	
	if (nRet == kResult_Success)
	{
		if (baAnswer.size() == 3 && baAnswer.at(0) == (char)0x55)
			nErrCode = (TKKMErrorCode)baAnswer.at(1);
		else
			nRet = kResult_ErrorAnswer;
	}


	//pool state
	uint8_t cMode     = 0x00;
	uint8_t cSubMode  = 0x00;
	uint8_t nFlag     = 0x00;  
	

	while(nRet == kResult_Success && nErrCode == 0)
	{
		m_sleep(500);
		int nErr          = 0;
		nRet = getMode(cMode, cSubMode, nFlag, nErr);
		if ((cMode != 2) || (cSubMode != 2))
			break;
	}

	if (nRet == kResult_Success && nErrCode == 0)
	{
		if ((cMode == 2) && (cSubMode == 0))
		{
			if(nFlag & 0x01)
				nErrCode = EX_KKM_ERROR_OUT_PAPER;
			else if (nFlag & 0x02)
				nErrCode = EX_KKM_ERROR_KKM_NOT_CONNECTED;
			else if (nFlag & 0x04)
				nErrCode = EX_KKM_ERROR_PRINTER_CUT_MECH;
		}
		else
		{
			nErrCode = EX_KKM_ERROR_REPORT_BREAK;
		}	
	}

	LOG_DBG("reportX <==");
	return nRet;
}


