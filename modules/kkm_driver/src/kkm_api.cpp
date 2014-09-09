#include "kkm_api.h"
#include "str_conv.h"

CKKMApi::CKKMApi(QSerialPort  *pSerial, TKKMInfo kkmDesc)
	: m_pSerial(pSerial)
	, m_kkmDesc(kkmDesc)
{}

CKKMApi::~CKKMApi()
{}

TResult CKKMApi::exec_command(const QByteArray& baCmd, QByteArray& baAnswer, int nTimeOut)
{
	TResult nRet = kResult_Success;
	LOG_DBG("exec_command ==>");

	do
	{
		sendENQ();
		readACK();
		//sendENQ_readACK();

		QByteArray baRawCommand = makeCommand(baCmd);
		LOG_DBG("command : '"<<ba2hex(baRawCommand)<<"'");
		m_pSerial->write(baRawCommand);

		readACK();
		sendEOT();

		readENQ(nTimeOut);
		sendACK();

		nRet = loadKKMData(baAnswer);
		if (nRet == kResult_Success)
		{
			if (baAnswer.size() > 2)
			{
				baAnswer = baAnswer.mid(1, baAnswer.size()-2);
			}
			else
			{
				nRet = kResult_ReadError;
				LOG_ERR("incorrect answer size");
				break;
			}
		}

		sendACK();
		readEOT();
	}
	while (false);
	LOG_DBG("exec_command <==");
	return nRet;
}

TResult CKKMApi::sendACK()
{
	LOG_DBG("sendACK ==>");
    QByteArray ba(&defs::cACK, 1);
    m_pSerial->write(ba);
	LOG_DBG("sendACK <==");
    return kResult_Success;
}

TResult CKKMApi::sendENQ()
{
	LOG_DBG("sendENQ ==>");
    QByteArray ba(&defs::cENQ, 1);
    m_pSerial->write(ba);
	LOG_DBG("sendENQ <==");
    return kResult_Success;
}

TResult CKKMApi::sendEOT()
{
	LOG_DBG("sendEOT ==>");
	QByteArray ba(&defs::cEOT, 1);
    m_pSerial->write(ba);
	LOG_DBG("sendEOT <==");
    return kResult_Success;
}

//TResult CKKMApi::readACK()
//{
//	LOG_DBG("readACK ==>");
//	//todo append repeats
//	TResult nRet = kResult_Success;
//
//    QByteArray readData;
//	//nRet = readChunk(readData, defs::nSendACKTimeout);
//	nRet = readChunk(readData, defs::nReadDataTimeout);
//	if (nRet == kResult_Success)
//	{
//		bool bCorrect = (readData == QByteArray(&defs::cACK,1));
//		LOG_DBG("readACK : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");	
//	}
//	LOG_DBG("readACK <==");
//    return nRet;
//}

//TResult CKKMApi::readENQ(int nTimeOut)
//{
//	LOG_DBG("readENQ ==>");
//	TResult nRet = kResult_Success;
//
//	QByteArray readData;
//	nRet = readChunk(readData, nTimeOut);
//	if (nRet == kResult_Success)
//	{
//		bool bCorrect = (readData == QByteArray(&defs::cENQ,1));
//		LOG_DBG("readENQ : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");	
//	}
//	LOG_DBG("readENQ <==");
//	return nRet;
//}

//TResult CKKMApi::readEOT()
//{
//	LOG_DBG("readEOT ==>");
//	//todo append repeats
//	TResult nRet = kResult_Success;
//
//	QByteArray readData;
//	nRet = readChunk(readData, defs::nReadEOTTimeout);
//	if (nRet == kResult_Success)
//	{
//		bool bCorrect = (readData == QByteArray(&defs::cEOT,1));
//		LOG_DBG("readEOT : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");
//		if (bCorrect)
//			nRet = kResult_ErrorAnswer;
//	}
//
//	LOG_DBG("readEOT <==");
//    return nRet;
//}

TResult CKKMApi::readACK()
{
	LOG_DBG("readACK ==>");
	//todo append repeats
	TResult nRet = kResult_Success;

    QByteArray readData;
	//nRet = readChunk(readData, defs::nSendACKTimeout);
	nRet = readChunk(readData, defs::nReadDataTimeout);
	if (nRet == kResult_Success)
	{
		bool bCorrect = (readData == QByteArray(&defs::cACK,1));
		LOG_DBG("readACK : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");	
		while (!bCorrect)
		{
			LOG_WRN("repeat readACK");
			nRet = readChunk(readData, defs::nReadDataTimeout);
			if (nRet != kResult_Success)
				break;
			bCorrect = (readData == QByteArray(&defs::cACK,1));
			LOG_DBG("readACK : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");
		}
	}
	LOG_DBG("readACK <==");
    return nRet;
}

TResult CKKMApi::readENQ(int nTimeOut)
{
	LOG_DBG("readENQ ==>");
	TResult nRet = kResult_Success;

	QByteArray readData;
	nRet = readChunk(readData, nTimeOut);
	if (nRet == kResult_Success)
	{
		bool bCorrect = (readData == QByteArray(&defs::cENQ,1));
		LOG_DBG("readENQ : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");	
		while (!bCorrect)
		{
			LOG_WRN("repeat readENQ");
			nRet = readChunk(readData, nTimeOut);
			if (nRet != kResult_Success)
				break;
			bCorrect = (readData == QByteArray(&defs::cENQ,1));
			LOG_DBG("readENQ : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");
		}
	}
	LOG_DBG("readENQ <==");
	return nRet;
}

TResult CKKMApi::readEOT()
{
	LOG_DBG("readEOT ==>");
	//todo append repeats
	TResult nRet = kResult_Success;

	QByteArray readData;
	nRet = readChunk(readData, defs::nReadEOTTimeout);
	if (nRet == kResult_Success)
	{
		bool bCorrect = (readData == QByteArray(&defs::cEOT,1));
		LOG_DBG("readEOT : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");

		while (!bCorrect)
		{
			LOG_WRN("repeat readEOT");
			nRet = readChunk(readData, defs::nReadEOTTimeout);
			if (nRet != kResult_Success)
				break;
			bCorrect = (readData == QByteArray(&defs::cEOT,1));
			LOG_DBG("readEOT : "<<bCorrect <<" '"<<ba2hex(readData)<<"'");
		}
	}

	LOG_DBG("readEOT <==");
    return nRet;
}

TResult CKKMApi::sendENQ_readACK()
{
	LOG_DBG("sendENQ_readACK ==>");
	TResult nRet = kResult_Success;

	sendENQ();
	nRet = readACK();

	int nRepeatCount = 2;
	while  (nRet != kResult_Success && nRepeatCount <= 5)
	{
		LOG_DBG("repeat sendENQ_readACK i = " << nRepeatCount);
		sendENQ();
		nRet = readACK();
		++nRepeatCount;
	}

	LOG_DBG("sendENQ_readACK <==");
    return nRet;
}

TResult CKKMApi::readChunk(QByteArray& baOutData, int nTimeOut)
{
	TResult nRet = kResult_Success;
	QByteArray readData;
	readData.append(m_pSerial->readAll());
	if (m_pSerial->waitForReadyRead(nTimeOut))
	{
		readData.append(m_pSerial->readAll());
	}
	baOutData = readData;

	if (m_pSerial->error() == QSerialPort::ReadError) 
	{
		LOG_ERR("readChunk , Failed to read from port '"<<m_kkmDesc.pszPortName<<"' , error: "<<m_pSerial->errorString().toStdString());
		nRet = kResult_ReadError;
	} 
	else if (m_pSerial->error() == QSerialPort::TimeoutError && readData.isEmpty()) 
	{
		LOG_ERR("readChunk , No data was currently available for reading from port '"<<m_kkmDesc.pszPortName<<"'");
		nRet = kResult_Timeout;
	}
	return nRet;
}

TResult CKKMApi::loadKKMData(QByteArray& baOutData)
{
	LOG_DBG("loadKKMData ==>");
	TResult nRet = kResult_Success;
	
	do
	{
		QByteArray readData;
		QByteArray baPacket;

		if ((nRet = readChunk(readData, defs::nReadDataTimeout)) != kResult_Success)
			break;
		
		char cPrevByte = 0x0;
		char cByte     = 0x0;

		bool bSTX            = false;
		bool bPacketComplete = false;
		for (int i = 0; i < readData.size(); ++i)
		{
			cByte = readData.at(i);
			if (cByte == defs::cSTX)
				bSTX = true;

			if (bSTX)
			{
				baPacket.append(cByte);
				if ((cPrevByte != defs::cDLE)&&(cByte == defs::cETX))
				{
					bPacketComplete = true;
					break;	
				}
				cPrevByte = cByte;
			}
		}

		if(!bSTX)
		{
			LOG_DBG("CKKMApi::loadKKMData, can`t find STX byte.");
			nRet = kResult_ReadError;
			break;
		}

		if(!bPacketComplete)
		{
			do
			{
				QByteArray readDataEx;
				if ((nRet = readChunk(readDataEx, defs::nReadByteTimeout)) != kResult_Success)
					break;

				cPrevByte = 0x0;
				cByte     = 0x0;
				for (int i = 0; i < readDataEx.size(); ++i)
				{
					cByte = readDataEx.at(i);
					baPacket.append(cByte);
					if ((cPrevByte != defs::cDLE)&&(cByte == defs::cETX))
					{
						bPacketComplete = true;
						break;	
					}
					cPrevByte = cByte;
				}
			}
			while(!bPacketComplete);
			if(nRet != kResult_Success)
				break;
		}

		baOutData = demaskCommand(baPacket);
		LOG_DBG("CKKMApi::loadKKMData, data  : "<<ba2hex(baOutData));
	}
	while(false);
	LOG_DBG("loadKKMData <==");
	return nRet;
}

QByteArray CKKMApi::makeCommand(const QByteArray & baData)
{
	//todo append password

	QByteArray baResult;
	QByteArray baPasswordData = val2BCD(m_kkmDesc.nPassword, 4) + baData;

	//1. Mask byte DLE and ETX
	baResult = maskCommand(baPasswordData);

	//2. Append ETX
	baResult = baResult + defs::cETX;

	//3. Calc <CRC>
	char cCRC = baResult[0];
	for (int i = 1; i < baResult.size(); ++i) 
	{
		cCRC = cCRC ^ baResult[i];
	}

	//4. Append STX
	baResult = defs::cSTX + baResult;

	//5. Append <CRC>
	baResult = baResult + cCRC;

	return baResult;
}

QByteArray maskCommand(const QByteArray& baIn)
{
	QByteArray baResult;
	
	//1. Mask byte DLE and ETX
	for (int i = 0; i < baIn.size(); ++i)
	{
		if (baIn[i] == defs::cDLE || baIn[i] == defs::cETX)
			baResult = baResult + defs::cDLE;
		baResult = baResult + baIn[i];
	}
	return baResult;
}

QByteArray demaskCommand(const QByteArray& baIn)
{
	QByteArray baResult;
	bool bDLEFlag    = false;
	char cByte       = 0x0;

	for (int i = 0 ; i < baIn.size(); ++i)
	{
		if (bDLEFlag)
		{
			bDLEFlag = false;
		}
		else
		{
			if (baIn[i] == defs::cDLE)
			{
				if (i < baIn.size())
				{
					if (baIn[i+1] == defs::cDLE || baIn[i] == defs::cETX)
					{
						baResult = baResult + baIn[i +1];
						bDLEFlag = true;
					}
				}
			}
			else
			{
				baResult = baResult + baIn[i];
			}
		}
	}
	return baResult;
}

//TSmartKKMApi createKKMApi(TKKMInfo kkmDesc)
//{
//	return TSmartKKMApi (new CKKMApi (kkmDesc));
//}

