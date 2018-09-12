#include "clientthread.h"

ClientThread::ClientThread(QObject *parent, qintptr handle)
	: QThread(parent), connectToClient(nullptr), handle(handle), id("")
{

}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
	//�����߳��д����Ϳͻ������ӵ�socket
	connectToClient = new TcpSocket(nullptr);
	connectToClient->setSocketDescriptor(handle);
	connect(this, &QThread::finished, connectToClient, &QTcpSocket::deleteLater); //�߳��˳����socket����
	connect(connectToClient, &TcpSocket::disconnected, this, &ClientThread::disconnectToClientSignal); //�Ͽ�����
	connect(connectToClient, &TcpSocket::getMsgSignal, this, &ClientThread::getMsgFromClientSignal); //������µ���Ϣ��ת���߳�������Ĵ�����
	connect(this, &ClientThread::sendMsgToClientSignal, connectToClient, [&](QString msg) {connectToClient->writeMsg(msg); }); //����ճ�����������Ϣ
	exec(); //�¼�ѭ��
}