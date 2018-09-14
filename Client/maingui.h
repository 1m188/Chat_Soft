#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>
#include "QListWidget"
#include "chatframe.h"
#include "extern.h"

//������
class MainGui : public QWidget
{
	Q_OBJECT

public:
	MainGui(QWidget *parent);
	~MainGui();

private:
	UserInfo myInfo; //�Լ����û���Ϣ
	QListWidget *friendList; //�����б�
	QWidget *framePlace; //��������С��Widget�������ڲ��ֵ�ʱ��ռ�أ��Լ�֮�����������ṩ������
	QVector<ChatFrame *> chatFramePool; //��������б�

protected:
	void resizeEvent(QResizeEvent *event) override; //��д�ı��С�¼���ʹ�ô��ڴ�С�ı��ʱ�������������֮�ı�

signals:
	void sendMsgSignal(QString msg, QString recverID); //����������Ϣ
	void getMsgSignal(QString msg, QString senderID); //��ȡ������Ϣ

	private slots:
	void friendListItemDoubleClicked(QListWidgetItem *item); //˫�������б��е�ĳһ���ʾ��ص��������
	void addFriendActionTriggered(); //��Ӻ���
	void delFriendActionTriggered(); //ɾ������
	void videoActionTriggered(); //��Ƶ����

	public slots:
	void getMyInfoSlot(QString id, QString name); //��ȡ�Լ����û���Ϣ
	void updateFriendList(QStringList friendList); //���º����б�
	void getMsgSlot(QString msg, QString senderID); //��ȡ������Ϣ
};

#endif // MAINGUI_H
