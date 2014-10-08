#ifndef KKMCONTROL_H
#define KKMCONTROL_H

#include <QtWidgets/QMainWindow>
#include "ui_kkmcontrol.h"

class KKMControl : public QMainWindow
{
	Q_OBJECT

public:
	KKMControl(QWidget *parent = 0);
	~KKMControl();

public slots:
	void onBtnStart();
	void onBtnStop();

private:
	Ui::KKMControlClass ui;
};

#endif // KKMCONTROL_H
