#include "kkmcontrol.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KKMControl w;
	w.show();
	return a.exec();
}
