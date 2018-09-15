#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "tcpsocket.h"
#include "logingui.h"
#include "maingui.h"
#include "extern.h"

#ifdef _DEBUG
#include "QDebug"
#endif // _DEBUG

//��̨�߳����е�������
class Data : public QObject
{
	Q_OBJECT

public:
	Data(QObject *parent);
	~Data();

private:
	TcpSocket *connectToServer; //���ӵ���������socket
	UserInfo *myInfo; //�Լ����û���Ϣ

	void loginSuccessHandle(QStringList msgList); //��½�ɹ�����Ӧ
	void loginFailedHandle(QStringList msgList); //��½ʧ�ܵ���Ӧ
	void loginRepeatHandle(QStringList msgList); //�ظ���½����Ӧ
	void registerSuccessHandle(QStringList msgList); //ע��ɹ�����Ӧ
	void getMyInfoHandle(QStringList msgList); //��ȡ�Լ����û���Ϣ����Ӧ
	void getFriendListHandle(QStringList msgList); //��ȡ�����б����Ӧ
	void messageHandle(QStringList msgList); //���պ��ѷ��͹�����������Ϣ����Ӧ
	void noThisUserHandle(QStringList msgList); //��Ӻ���ʱ����û������û�

public:
	//�������������½�������������źż��źŲ۵����ӵĽӿ�
	void addSignalSlots(LoginGui *loginGui) { emit addSignalSlotsForClassSignal(loginGui); }
	void addSignalSlots(MainGui *mainGui) { emit addSignalSlotsForClassSignal(mainGui); }

signals:
	//����˵���Ľӿ��з��͵��źţ�ʹ�����������ӹ�������һ���߳������
	void addSignalSlotsForClassSignal(LoginGui *loginGui);
	void addSignalSlotsForClassSignal(MainGui *mainGui);

	void loginSignal(); //��½�ź�
	void loginFailedSignal(); //��½ʧ��
	void loginRepeatSignal(); //�ظ���½
	void registerSuccessSignal(QString id); //ע��ɹ�

	void getMyInfoSignal(QString id, QString name); //��ȡ�Լ����û���Ϣ
	void getFriendListSignal(QStringList friendList); //��ȡ�����б�
	void getMsgSignal(QString msg, QString senderID); //����������Ϣ

	void noThisUserSignal(); //��Ӻ���ʱ����û������û�

	private slots:
	//��������һ���߳�����Ӧ����źż��źŲ����ӵ��źŲ�
	void addSignalSlotsForClassSlot(LoginGui *loginGui);
	void addSignalSlotsForClassSlot(MainGui *mainGui);

	void init(); //data������һ���߳��еĳ�ʼ��
	void getMsgFromServer(QString msg); //�ӷ�������ȡ��Ϣ

	void loginRequestSlot(QString acountInfo); //���͵�½����
	void registerRequestSlot(QString acountInfo); //����ע������

	void sendMsgSlot(QString msg, QString recverID); //����������Ϣ

	void addFriendSlot(QString friendID); //������Ӻ�������
};

#endif // DATA_H
