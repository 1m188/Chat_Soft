#include <QtWidgets/QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
	//QApplication::addLibraryPath("./plugins"); //�������汾ʱҪ����

	QApplication a(argc, argv);

	Server server(nullptr);

	return a.exec();
}
