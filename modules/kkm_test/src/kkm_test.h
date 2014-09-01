#ifndef KKM_TEST_H
#define KKM_TEST_H

#include <QtWidgets/QMainWindow>
#include "ui_kkm_test.h"

class kkm_test : public QMainWindow
{
	Q_OBJECT

public:
	kkm_test(QWidget *parent = 0);
	~kkm_test();

public slots:
	void onBtnFind();
	void onBtnExecCommand();

private:
	Ui::kkm_testClass ui;
};

#endif // KKM_TEST_H
