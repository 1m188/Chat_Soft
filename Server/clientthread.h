#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "tcpsocket.h"

//�����ϵĿͻ������ڵ��߳�
class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(QObject *parent, qintptr handle);
	~ClientThread();

private:
	TcpSocket *connectToClient; //�Ϳͻ������ӵ�socket
	qintptr handle; //socket�������
	QString id; //�û�id

public:
	const QString getID() const { return id; } //��ȡ����߳��������ӵ�client��id
	void setID(QString id) { this->id = id; } //��������߳������ӵ�client��id

protected:
	void run() override; //��дrun����

signals:
	void disconnectToClientSignal(); //�Ϳͻ��˶Ͽ������ź�
	void getMsgFromClientSignal(QString msg); //�ӿͻ��˻�ȡ��Ϣ�ź�
	void sendMsgToClientSignal(QString msg); //���ͻ��˷�����Ϣ�ź�
};

#endif // CLIENTTHREAD_H
