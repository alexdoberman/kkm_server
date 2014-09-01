#pragma once
#if defined(_WIN32) || defined(_WIN32_)
	#if defined(KKM_DRIVER_EXPORTS)
		#define KKM_DRIVER_LIB __declspec(dllexport) 
	#else
		#define KKM_DRIVER_LIB __declspec(dllimport)
	#endif /* KKM_DRIVER_EXPORTS */ 
#elif defined(linux) || defined(_linux)
	#define KKM_DRIVER_LIB
#endif 


#include "kkm_driver_type.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#define EX_KKM_ERROR_OUT_PAPER            1000    
#define EX_KKM_ERROR_KKM_NOT_CONNECTED    1001
#define EX_KKM_ERROR_PAPER                1002 
#define EX_KKM_ERROR_PRINTER_CUT_MECH     1003
#define EX_KKM_ERROR_REPORT_BREAK         1004



struct TKKMInfo
{
	uint32_t        nPassword;
	char            pszPortName[64];
};

struct TPortInfo
{
	std::string sPort;
	std::string sLocation;
	std::string sDescription;
	std::string sManufacturer;
	std::string sVendorId;
	std::string sProductId;
	bool        bBusy;
};

#pragma pack(push, 1)
struct TKKMState
{
	uint32_t nSerialNumber;
	uint32_t nKKMVersion;
	uint8_t  cModel;
	uint8_t  cMode;
	uint8_t  cCheckState;
};
#pragma pack(pop)


class IKKMControl
{
public:
	virtual TResult connect(TKKMInfo kkmDesc)                                                                                   = 0;
	virtual TResult disconnect()                                                                                                = 0;
	virtual TResult exec_command(const std::string & sCmd, std::string& sAnsver)                                                = 0;

	virtual TResult setMode(uint8_t cMode, uint32_t nPassword, int& nErrCode)                                                   = 0;
	virtual TResult resetMode(int& nErrCode)                                                                                    = 0;
	virtual TResult getMode(uint8_t& cMode, uint8_t& cSubMode, uint8_t& nFlag, int& nErrCode)                                   = 0;
	virtual TResult printLine(const std::string & sText, int& nErrCode)                                                         = 0;
	virtual TResult getKKMState(TKKMState& kkmState, int& nErrCode)                                                             = 0;

	virtual TResult openSession(const std::string & sText, int& nErrCode)                                                       = 0;
	virtual TResult openCheck(uint8_t cTestMode, uint8_t cCheckType,  int& nErrCode)                                            = 0;
	virtual TResult cancelCheck( int& nErrCode) = 0;
	virtual TResult cashIn(uint8_t cTestMode, uint32_t nSumm, int& nErrCode)                                                    = 0;
	virtual TResult cashOut(uint8_t cTestMode, uint32_t nSumm, int& nErrCode)                                                   = 0;
	virtual TResult registaration(uint8_t cTestMode, uint32_t nPrice, uint32_t nQuantity, uint8_t  nDepartment, int& nErrCode)  = 0;
	virtual TResult return_(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode)                               = 0;
	virtual TResult cancellation(uint8_t cTestMode,uint32_t nPrice, uint32_t nQuantity, int& nErrCode)                          = 0;

	virtual TResult closeCheck(uint8_t cTestMode, uint8_t cTypeClose, uint32_t nSumm,  int& nErrCode)                           = 0;
	virtual TResult discount(uint8_t cTestMode, uint8_t cArea, uint8_t cType, uint8_t cSign, uint32_t nValue, int& nErrCode)    = 0;

	virtual TResult reportZ(int& nErrCode)                                                                                      = 0;
	virtual TResult reportX(uint8_t cReportType, int& nErrCode)                                                                 = 0;
};
typedef std::shared_ptr<IKKMControl> TSmartKKMControl;


KKM_DRIVER_LIB TSmartKKMControl createKKMControl();
KKM_DRIVER_LIB TResult getPortInfo(std::vector <TPortInfo> &vPortInfo);
