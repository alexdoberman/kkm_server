#include "kkm_test.h"
#include <QtSerialPort/QSerialPortInfo>
#include "../../kkm_driver/include/kkm_driver.h"

kkm_test::kkm_test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	QObject::connect(ui.btnFind,                   SIGNAL(clicked()),                             this, SLOT(onBtnFind()));
	QObject::connect(ui.btnExecCommand,            SIGNAL(clicked()),                             this, SLOT(onBtnExecCommand()));
}

kkm_test::~kkm_test()
{

}

void kkm_test::onBtnFind()
{
	ui.cmbDev->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) 
	{
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
		ui.cmbDev->addItem(s, QVariant::fromValue(info.portName()));
	}

}



void kkm_test::onBtnExecCommand()
{
	if (ui.cmbDev->currentIndex() != -1)
	{
		QVariant  vCom = ui.cmbDev->itemData(ui.cmbDev->currentIndex());

		TKKMInfo kkminfo;
		memset(&kkminfo, 0, sizeof(kkminfo));
		strcpy(kkminfo.pszPortName, vCom.toString().toStdString().c_str());
		
		
		TSmartKKMControl spKKMControl = createKKMControl();
		
		spKKMControl->connect(kkminfo);
		std::string sAnswer;
		spKKMControl->exec_command(ui.editCommand->text().toStdString(), sAnswer);
		spKKMControl->disconnect();

		ui.textEdit->clear();
		ui.textEdit->setText(sAnswer.c_str());
	}
}

