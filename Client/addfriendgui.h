#ifndef ADDFRIENDGUI_H
#define ADDFRIENDGUI_H

#include <QWidget>
#include "QLineEdit"
#include "extern.h"

//��Ӻ��ѽ���
class AddFriendGui : public QWidget
{
	Q_OBJECT

public:
	AddFriendGui(QWidget *parent, UserInfo myInfo);
	~AddFriendGui();

private:
	UserInfo myInfo; //�Լ����û���Ϣ
	QLineEdit *idLineEdit; //Ҫ��ӵĺ���id�����

protected:
	void closeEvent(QCloseEvent *event) override; //��д�Ĺر��¼�

signals:
	void closeSignal(); //����رյ�ʱ�򷢳��ź�
	void addFriendSignal(QString friendID); //��Ӻ����ź�

	private slots:
	void addButtonClicked(); //��Ӻ��Ѱ�ť����

	public slots:
	void addFriendRepeatSlot(); //�Ѿ�������������
	void noThisUserSlot(); //��Ӻ��ѵ�ʱ�򷵻�û������û�
};

#endif // ADDFRIENDGUI_H
