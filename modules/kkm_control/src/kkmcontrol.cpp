#include "kkmcontrol.h"
#include <QProcess>


KKMControl::KKMControl(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	
	QObject::connect(ui.btnStart,               SIGNAL(clicked()),                             this, SLOT(onBtnStart()));
	QObject::connect(ui.btnStop,                SIGNAL(clicked()),                             this, SLOT(onBtnStop()));

}

KKMControl::~KKMControl()
{

}

void KKMControl::onBtnStart()
{
	//QProcess p;
	QProcess::startDetached("notepad.exe");
}

void KKMControl::onBtnStop()
{
	
}
