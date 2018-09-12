#include <QtWidgets/QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	QApplication::addLibraryPath("./plugins"); //�������汾ʱҪ����
#endif // NDEBUG

	QApplication a(argc, argv);

	Server server(nullptr);

	return a.exec();
}
