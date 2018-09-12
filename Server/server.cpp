#include "server.h"
#include "QDebug"

Server::Server(QObject *parent)
	: QTcpServer(parent), threadPool({}), database(QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))), query(QSqlQuery(database))
{
	//��ʼ�������������ݿ�
	database.setDatabaseName("softchat");
	database.setHostName("127.0.0.1");
	database.setPort(3306);
	database.setUserName("root");
	database.setPassword("123456");
	database.open();

	//��ʼ��������ip��8888�˿�
	listen(QHostAddress::Any, 8888);
	qDebug() << u8"��ʼ��������IP��8888�˿�...";
}

Server::~Server()
{
	//�ͷ����ݿ�������Դ
	query.clear();
	database.close();
}

void Server::incomingConnection(qintptr handle)
{
	ClientThread *clientThread = new ClientThread(nullptr, handle);
	connect(clientThread, &ClientThread::disconnectToClientSignal, this, &Server::disconnectToClientSlot); //�Ͽ�������ת����Ӧ������
	connect(clientThread, &ClientThread::getMsgFromClientSignal, this, &Server::getMsgFromClientSlot); //�����Ϣ
	threadPool.push_back(clientThread); //�����̳߳�
	clientThread->start(); //�����߳�

	qDebug() << QString("a new connection with handle %1, the num of the client is %2").arg(handle).arg(threadPool.size());
}

void Server::disconnectToClientSlot()
{
	ClientThread *sender = dynamic_cast<ClientThread *>(this->sender());
	sender->exit(); //�˳��߳�
	threadPool.removeOne(sender); //���̳߳���ɾ��
	sender->deleteLater(); //�ͷ���Դ

	qDebug() << QString("a connection disconnect, the num of the client is %1").arg(threadPool.size());
}

void Server::getMsgFromClientSlot(QString msg)
{
	//��ȡ���ͷ�
	ClientThread *sender = dynamic_cast<ClientThread *>(this->sender());
	//������������Ϣ
	qDebug() << sender->getID() << "id send message" << msg;
	//��ȡ��Ϣ�б�
	QStringList msgList = msg.split(' ');

	//�ж�
	//��½����
	if (msgList[0] == "login")
	{
		loginRequestHandle(sender, msgList);
	}
	//ע������
	else if (msgList[0] == "register")
	{
		registerRequestHandle(sender, msgList);
	}
	//�Լ����û���Ϣ����
	else if (msgList[0] == "myInfo")
	{
		myInfoRequestHandle(sender, msgList);
	}
	//�����б�����
	else if (msgList[0] == "friendList")
	{
		friendListRequestHandle(sender, msgList);
	}
	//����������Ϣ
	else if (msgList[0] == "Message")
	{
		messageHandle(sender, msgList);
	}
}

void Server::loginRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡ��½id������
	QString id = msgList[1];
	QString password = msgList[2];

	//��֤�Ƿ��Ѿ���½
	for (ClientThread *c : threadPool)
	{
		if (c->getID() == id)
		{
			emit sender->sendMsgToClientSignal("loginRepeat"); //�ظ���½
			return;
		}
	}

	//��֤��½�Ƿ���ȷ
	query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password));
	if (query.next())
	{
		emit sender->sendMsgToClientSignal("loginSuccess"); //��½�ɹ�
		sender->setID(id);
	}
	else
	{
		emit sender->sendMsgToClientSignal("loginFailed"); //��½ʧ��
	}
}

void Server::registerRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡע����ǳƺ�����
	QString name = msgList[1];
	QString password = msgList[2];
	//��ȡ�µ�id
	query.exec("select * from acount");
	QString id = QString::number(query.size() + 1);

	query.exec(QString("insert into acount values('%1','%2','%3')").arg(id).arg(name).arg(password)); //�����û������Ϣ���û�����
	query.exec(QString("create table friend%1(friendid char(10) not null);").arg(id)); //����һ���û��ĺ��ѱ�
	emit sender->sendMsgToClientSignal(QString("registerSuccess") + ' ' + id); //ע��ɹ�
}

void Server::myInfoRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡ���ͷ�id
	QString id = sender->getID();
	query.exec(QString("select name from acount where id='%1';").arg(id)); //��ѯ�Լ�id��Ӧ��id���ǳ���Ϊ�Լ����û���Ϣ����
	//�����͵ķ�����Ϣ
	QString myInfo = "myInfo";
	myInfo += ' ';
	myInfo += id;
	myInfo += ' ';
	if (query.next())
	{
		myInfo += query.value("name").toString();
	}
	//������Ϣ
	emit sender->sendMsgToClientSignal(myInfo);
}

void Server::friendListRequestHandle(ClientThread * sender, QStringList msgList)
{
	QString id = sender->getID(); //��ȡ���ͷ�id
	query.exec(QString("select id,name from acount,friend%1 where acount.id=friend%2.friendid;").arg(id).arg(id)); //��ѯ��Ӧid�ĺ��ѱ��е����еĺ��ѵ�id���ǳ�
	//�����ͷ��ص���Ϣ
	QString friendList = "friendList";
	while (query.next())
	{
		friendList += ' ';
		friendList += query.value("id").toString();
		friendList += '_';
		friendList += query.value("name").toString();
	}
	//����
	emit sender->sendMsgToClientSignal(friendList);
}

void Server::messageHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡ���շ�id
	QString recvID = msgList[2];
	//�������ϵĿͻ��˳��в��ҽ��շ�
	for (ClientThread *c : threadPool)
	{
		if (c->getID() == recvID)
		{
			emit c->sendMsgToClientSignal(msgList.join(' '));
			return;
		}
	}
	//���û�����ߵĻ��ͷ���
	emit sender->sendMsgToClientSignal(u8"���û�Ŀǰû�����ߣ������Ϣ�����޷����");
}