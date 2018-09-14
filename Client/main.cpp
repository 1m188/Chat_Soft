#include <QtWidgets/QApplication>
#include "data.h"
#include "logingui.h"
#include "maingui.h"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	QApplication::addLibraryPath("./plugins"); //�������汾ʱҪ����
#endif // NDEBUG

	QApplication a(argc, argv);

	Data *data = new Data(nullptr); //��ʼ��������

	//ʵ������½���沢�Һ������ཨ����ϵ
	LoginGui *loginGui = new LoginGui(nullptr);
	data->addSignalSlots(loginGui);
	loginGui->show();

	//�ж��Ի�����ʾ���
	if (loginGui->exec() == QDialog::Accepted)
	{
		//���λAccept����������棬���������ಢ�ҽ�����ʽ���¼�ѭ��
		MainGui *mainGui = new MainGui(nullptr);
		data->addSignalSlots(mainGui);
		mainGui->show();
		a.exec();
	}
	delete data;
	return 0;
}
