#pragma once

#include "kkm_driver.h"
#include "kkm_driver_def.h"

#include <QtSerialPort/QSerialPort>
#include <QByteArray>


#pragma pack(push, 1)
struct TKKMStateRaw
{
	uint8_t  cStub;
	uint8_t  cKassir;
	uint8_t  cNumberInHall;
	uint8_t  vKKMDate[3];
	uint8_t  vKKMTime[3];
	uint8_t  cFlag;
	uint8_t  vSerialNumber[4];
	uint8_t  cModel;
	uint8_t  vKKMVersion[2];
	uint8_t  cMode;
	uint8_t  vCheckNumber[2];
	uint8_t  vWorkNumber[2];
	uint8_t  cCheckState;
	uint8_t  vCheckSumm[5];
	uint8_t  cDecimalPoint;
	uint8_t  cPort;
};
#pragma pack(pop)


class CKKMControl : public IKKMControl
{
	typedef uint8_t TKKMErrorCode;
public:
    explicit CKKMControl();
    virtual ~CKKMControl();
public:
	virtual TResult connect(TKKMInfo kkmDesc);
	virtual TResult disconnect();
	virtual TResult exec_command(const std::string & sCmd, std::string& sAnsver);

	virtual TResult setMode(uint8_t cMode, uint32_t nPassword, int& nErrCode);
	virtual TResult resetMode(int& nErrCode);
	virtual TResult getMode(uint8_t& cMode, uint8_t& cSubMode, uint8_t& nFlag, int& nErrCode);
	virtual TResult printLine(const std::string & sText, int& nErrCode);
	virtual TResult getKKMState(TKKMState& kkmState, int& nErrCode);

	virtual TResult openSession(const std::string & sText, int& nErrCode);
	virtual TResult openCheck(uint8_t cTestMode, uint8_t cCheckType,  int& nErrCode);
	virtual TResult cancelCheck( int& nErrCode);
	virtual TResult cashIn(uint8_t cTestMode, uint32_t nSumm, int& nErrCode);
	virtual TResult cashOut(uint8_t cTestMode, uint32_t nSumm, int& nErrCode);
	virtual TResult registaration(uint8_t cTestMode, uint32_t nPrice, uint32_t nQuantity, uint8_t  nDepartment, int& nErrCode);
	virtual TResult return_(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode);
	virtual TResult cancellation(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode);
	virtual TResult closeCheck(uint8_t cTestMode, uint8_t cTypeClose, uint32_t nSumm,  int& nErrCode);
	virtual TResult discount(uint8_t cTestMode, uint8_t cArea, uint8_t cType, uint8_t cSign, uint32_t nValue, int& nErrCode);

	virtual TResult reportZ(int& nErrCode);
	virtual TResult reportX(uint8_t cReportType, int& nErrCode);

private:
    TKKMInfo      m_kkmDesc;
	QSerialPort  *m_pSerial;
	bool          m_bConnected;
};
