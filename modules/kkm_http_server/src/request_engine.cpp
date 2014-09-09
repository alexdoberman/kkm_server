#include "request_engine.h"
#include "kkm_server_def.h"

#include <boost/scope_exit.hpp>

TResult kkm_request::check(TSmartKKMControl spKKMControl , TCheck check, int& nErrCode)
{
	bool bModeSet   = false;
	bool bCheckOpen = false;

	TResult nRet    = kResult_Success;
	int     nKKMErr = 0;
	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->setMode(0x01, check.nPassword , nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check, method setMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = true;

		nRet = spKKMControl->openCheck(0x00, 0x01, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check, method openCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bCheckOpen = true;

		for (size_t i = 0 ; i < check.vCheckItem.size(); ++i)
		{
			TCheckItem checkItem = check.vCheckItem[i];

			
			nRet = spKKMControl->printLine(checkItem.sPrintLine, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::check, method printLine,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}

			
			nRet = spKKMControl->registaration(0x00, checkItem.nPrice, checkItem.nQuantity, checkItem.nDepartment, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::check, method registaration,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}

			if (checkItem.bUseDiscount)
			{
				TDiscount disc = checkItem.discount;
				uint8_t bType = (disc.kDiscountType == kDiscountType_Percent)? 0x00: 0x01;
				uint8_t bSign = (disc.kSign == kDiscountSign_Minus)? 0x00: 0x01;

				nRet = spKKMControl->discount(0x00, 0x01, bType, bSign, disc.nValue, nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::check, method discount,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
					break;
				}
			}
		}

		if (nRet != kResult_Success || nKKMErr != 0)
			break;

		if (check.bUseAllDiscount)
		{
			TDiscount disc = check.discount;
			uint8_t bType = (disc.kDiscountType == kDiscountType_Percent)? 0x00: 0x01;
			uint8_t bSign = (disc.kSign == kDiscountSign_Minus)? 0x00: 0x01;

			nRet = spKKMControl->discount(0x00, 0x00, bType, bSign, disc.nValue, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::check, method all check discount,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}		
		}
		
		
		nRet = spKKMControl->closeCheck(0x00, check.nTypePayment, check.nCheckSumm, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check, method closeCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bCheckOpen = false;

		nRet = spKKMControl->resetMode(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = false;

	}
	while (false);
	nErrCode = nKKMErr;


	//try rolback
	{
		if (nErrCode != 0)
		{
			if (bModeSet && bCheckOpen)
			{
				//annulate check
				LOG_ERR("kkm_request::check, BEGIN ROLLBACK  ================================================");
				
				TResult nRet    = kResult_Success;
				int     nKKMErr = 0;

				nRet = spKKMControl->cancelCheck(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::check, method rollback cancelCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}

				nRet = spKKMControl->resetMode(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::check, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}
				LOG_ERR("kkm_request::check, END ROLLBACK    ================================================");
			}
			else if (bModeSet && !bCheckOpen)
			{
				LOG_ERR("kkm_request::check, BEGIN ROLLBACK  ================================================");
				
				TResult nRet    = kResult_Success;
				int     nKKMErr = 0;

				nRet = spKKMControl->resetMode(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::check, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}
				LOG_ERR("kkm_request::check, END ROLLBACK    ================================================");
			}
		}
	}
	return nRet;
}

TResult kkm_request::return_check(TSmartKKMControl spKKMControl , TReturnCheck check, int& nErrCode)
{
	bool bModeSet   = false;
	bool bCheckOpen = false;
	TResult nRet    = kResult_Success;
	int     nKKMErr = 0;
	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::return_check, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->setMode(0x01, check.nPassword , nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::return_check, method setMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = true;
		//
		nRet = spKKMControl->openCheck(0x00, 0x02, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::return_check, method openCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bCheckOpen = true;

		for (size_t i = 0 ; i < check.vCheckItem.size(); ++i)
		{
			TReturnCheckItem checkItem = check.vCheckItem[i];

			nRet = spKKMControl->printLine(checkItem.sPrintLine, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::return_check, method printLine,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}

			nRet = spKKMControl->return_(0x00, checkItem.nPrice, checkItem.nQuantity, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::return_check, method return_,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}
		}

		if (nRet != kResult_Success || nKKMErr != 0)
			break;
		
		
		nRet = spKKMControl->closeCheck(0x00, check.nTypePayment, 0x00, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::return_check, method closeCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bCheckOpen = false;

		nRet = spKKMControl->resetMode(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::return_check, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = false;

	}
	while (false);
	nErrCode = nKKMErr;

	//try rolback
	{
		if (nErrCode != 0)
		{
			if (bModeSet && bCheckOpen)
			{
				//annulate check
				LOG_ERR("kkm_request::return_check, BEGIN ROLLBACK  ================================================");
				
				TResult nRet    = kResult_Success;
				int     nKKMErr = 0;

				nRet = spKKMControl->cancelCheck(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::return_check, method rollback cancelCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}

				nRet = spKKMControl->resetMode(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::return_check, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}
				LOG_ERR("kkm_request::return_check, END ROLLBACK    ================================================");
			}
			else if (bModeSet && !bCheckOpen)
			{
				LOG_ERR("kkm_request::return_check, BEGIN ROLLBACK  ================================================");
				
				TResult nRet    = kResult_Success;
				int     nKKMErr = 0;

				nRet = spKKMControl->resetMode(nKKMErr);
				if(nRet != kResult_Success || nKKMErr != 0)
				{
					LOG_ERR("kkm_request::return_check, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				}
				LOG_ERR("kkm_request::return_check, END ROLLBACK    ================================================");
			}
		}
	}
	return nRet;
}

TResult kkm_request::cancellation_check(TSmartKKMControl spKKMControl , TCancellationCheck check, int& nErrCode)
{
	TResult nRet    = kResult_Success;
	int     nKKMErr = 0;
	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::cancellation_check, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->setMode(0x01, check.nPassword , nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::cancellation_check, method setMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		//
		//nRet = pKKMControl->openCheck(0x00, 0x03, nKKMErr);
		//if(nRet != kResult_Success || nKKMErr != 0)
		//{
		//	LOG_ERR("kkm_request::cancellation_check, method openCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
		//	break;
		//}

		for (size_t i = 0 ; i < check.vCheckItem.size(); ++i)
		{
			TCancellationCheckItem checkItem = check.vCheckItem[i];

			nRet = spKKMControl->printLine(checkItem.sPrintLine, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::cancellation_check, method printLine,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}

			nRet = spKKMControl->cancellation(0x00, checkItem.nPrice, checkItem.nQuantity, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::cancellation_check, method cancellation,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}
		}

		if (nRet != kResult_Success || nKKMErr != 0)
			break;
		
		
		nRet = spKKMControl->closeCheck(0x00, check.nTypePayment, 0x00, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::cancellation_check, method closeCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->resetMode(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::cancellation_check, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

	}
	while (false);
	nErrCode = nKKMErr;
	return nRet;
}

TResult kkm_request::report_x(TSmartKKMControl spKKMControl , const TReportX &report, int& nErrCode)
{
	TResult nRet      = kResult_Success;
	int     nKKMErr   = 0;
	bool bModeSet     = false;
	bool bReportPrint = false;
	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_x, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->setMode(0x02, report.nPassword , nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_x, method setMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = true;

		//
		nRet = spKKMControl->reportX(report.cReportType, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_x, method reportX,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bReportPrint = true;

		nRet = spKKMControl->resetMode(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_x, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

	}
	while (false);
	nErrCode = nKKMErr;

	//try rollback
	if (nErrCode != 0)
	{
		if (bModeSet && !bReportPrint)
		{
			LOG_ERR("kkm_request::report_x, BEGIN ROLLBACK  ================================================");
				
			TResult nRet    = kResult_Success;
			int     nKKMErr = 0;

			nRet = spKKMControl->resetMode(nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::report_x, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			}
			LOG_ERR("kkm_request::report_x, END ROLLBACK    ================================================");
		}
	}
	return nRet;
}

TResult kkm_request::report_z(TSmartKKMControl spKKMControl , const TReportZ &report, int& nErrCode)
{
	TResult nRet      = kResult_Success;
	int     nKKMErr   = 0;
	bool bModeSet     = false;
	bool bReportPrint = false;

	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_z, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		nRet = spKKMControl->setMode(0x03, report.nPassword , nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_z, method setMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bModeSet = true;

		//
		nRet = spKKMControl->reportZ(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_z, method reportZ,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
		bReportPrint = true;

		nRet = spKKMControl->resetMode(nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::report_z, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}
	}
	while (false);
	nErrCode = nKKMErr;
	//try rollback

	if (nErrCode != 0)
	{
		if (bModeSet && !bReportPrint)
		{
			LOG_ERR("kkm_request::report_z, BEGIN ROLLBACK  ================================================");
				
			TResult nRet    = kResult_Success;
			int     nKKMErr = 0;

			nRet = spKKMControl->resetMode(nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::report_z, method resetMode resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			}
			LOG_ERR("kkm_request::report_z, END ROLLBACK    ================================================");
		}
	}

	return nRet;
}

TResult kkm_request::check_kkm_state(TSmartKKMControl spKKMControl, int& nErrCode)
{
	TResult nRet      = kResult_Success;
	int     nKKMErr   = 0;

	do
	{
		//annulate check
		TKKMState kkmState;
		nRet = spKKMControl->getKKMState(kkmState, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::check_kkm_state, method getKKMState,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		uint8_t checkState = kkmState.cCheckState&0x07;
		if (checkState!=0)
		{
			LOG_WRN("kkm_request::check_kkm_state FIND OPEN CHECK. BEGIN ANNULATE CHECK. ==================================");
			nRet = spKKMControl->cancelCheck(nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::check_kkm_state, method cancelCheck,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}
			LOG_WRN("kkm_request::check_kkm_state END ANNULATE CHECK. =====================================================");
		}

		uint8_t cMode    = kkmState.cMode & 0x0F;
		uint8_t	cSubMode = (kkmState.cMode & 0xF0) >> 4;

		if ((cMode != 0) || (cSubMode != 0))
		{
			LOG_WRN("kkm_request::check_kkm_state FIND INPROPER MODE. BEGIN RESET MODE. ==================================");
			nRet = spKKMControl->resetMode(nKKMErr);

			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::check_kkm_state, method resetMode,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}
			LOG_WRN("kkm_request::check_kkm_state END RESET MODE. ========================================================");		
		}
	}
	while (false);
	nErrCode = nKKMErr;
	return nRet;
}

TResult kkm_request::print_plain_text(TSmartKKMControl spKKMControl, const TPrintText& text, int& nErrCode)
{
	TResult nRet    = kResult_Success;
	int     nKKMErr = 0;
	do
	{
		nRet = kkm_request::check_kkm_state(spKKMControl, nKKMErr);
		if(nRet != kResult_Success || nKKMErr != 0)
		{
			LOG_ERR("kkm_request::print_plain_text, method check_kkm_state,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
			break;
		}

		for (size_t i = 0 ; i < text.vStringLine.size(); ++i)
		{
			std::string sLine = text.vStringLine[i];

			nRet = spKKMControl->printLine(sLine, nKKMErr);
			if(nRet != kResult_Success || nKKMErr != 0)
			{
				LOG_ERR("kkm_request::print_plain_text, method printLine,  nRet: " <<nRet << "  nKKMErr: "<<nKKMErr);
				break;
			}
		}
	}
	while (false);
	nErrCode = nKKMErr;
	return nRet;
}



