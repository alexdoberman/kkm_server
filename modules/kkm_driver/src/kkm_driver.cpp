#include "kkm_driver.h"
#include "kkm_control.h"
#include <QtSerialPort/QSerialPortInfo>


KKM_DRIVER_LIB TSmartKKMControl createKKMControl()
{
    TSmartKKMControl  spKKMControl = TSmartKKMControl (new CKKMControl());
    return spKKMControl;
}

KKM_DRIVER_LIB TResult getPortInfo(std::vector <TPortInfo> &vPortInfo)
{
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) 
	{
		TPortInfo portInfo;

		portInfo.sPort         = info.portName().toStdString();
		portInfo.sLocation     = info.systemLocation().toStdString();
		portInfo.sDescription  = info.description().toStdString();
		portInfo.sManufacturer = info.manufacturer().toStdString();
		portInfo.sVendorId     = (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()).toStdString();
		portInfo.sProductId    = (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()).toStdString();
		portInfo.bBusy         = info.isBusy();

		vPortInfo.push_back(portInfo);
	}
	return kKKMResult_Success;
}
