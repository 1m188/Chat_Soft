#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "QSqlQuery"
#include "clientthread.h"

//������
class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

private:
	QVector<ClientThread *> threadPool; //�����ϵĿͻ��˵��̳߳�
	QSqlDatabase database; //���ӵ��ı���MySQL���ݿ�
	QSqlQuery query; //��ѯ

	void loginRequestHandle(ClientThread *sender, QStringList msgList); //��½��������
	void registerRequestHandle(ClientThread *sender, QStringList msgList); //ע����������
	void myInfoRequestHandle(ClientThread *sender, QStringList msgList); //�����û���Ϣ��������
	void friendListRequestHandle(ClientThread *sender, QStringList msgList); //�����б���������
	void messageHandle(ClientThread *sender, QStringList msgList); //����������Ϣ������
	void addFriendRequestHandle(ClientThread *sender, QStringList msgList); //��Ӻ�����������

protected:
	void incomingConnection(qintptr handle) override; //��д���ӵ�����ʱ����

	private slots:
	void disconnectToClientSlot(); //�Ͽ����Ӳ�
	void getMsgFromClientSlot(QString msg); //�����Ϣ�Ĵ�����
};

#endif // SERVER_H
