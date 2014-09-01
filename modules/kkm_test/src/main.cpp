#include "kkm_test.h"
#include <QtWidgets/QApplication>

#include <QFile>
#include <QByteArray>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	kkm_test w;
	w.show();
	return a.exec();
}
