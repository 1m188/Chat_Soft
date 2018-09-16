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
	if (msgList[0] == "LoginRequest")
	{
		loginRequestHandle(sender, msgList);
	}
	//ע������
	else if (msgList[0] == "RegisterRequest")
	{
		registerRequestHandle(sender, msgList);
	}
	//�Լ����û���Ϣ����
	else if (msgList[0] == "MyInfoRequest")
	{
		myInfoRequestHandle(sender, msgList);
	}
	//�����б�����
	else if (msgList[0] == "FriendListRequest")
	{
		friendListRequestHandle(sender, msgList);
	}
	//����������Ϣ
	else if (msgList[0] == "Message")
	{
		messageHandle(sender, msgList);
	}
	//��Ӻ�������
	else if (msgList[0] == "AddFriendRequest")
	{
		addFriendRequestHandle(sender, msgList);
	}
	//ɾ����������
	else if (msgList[0] == "DelFriendRequest")
	{
		delFriendRequestHandle(sender, msgList);
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
			emit sender->sendMsgToClientSignal("LoginRepeat"); //�ظ���½
			return;
		}
	}

	//��֤��½�Ƿ���ȷ
	query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password));
	if (query.next())
	{
		emit sender->sendMsgToClientSignal("LoginSuccess"); //��½�ɹ�
		sender->setID(id);
	}
	else
	{
		emit sender->sendMsgToClientSignal("LoginFailed"); //��½ʧ��
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
	emit sender->sendMsgToClientSignal(QString("RegisterSuccess") + ' ' + id); //ע��ɹ�
}

void Server::myInfoRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡ���ͷ�id
	QString id = sender->getID();
	query.exec(QString("select name from acount where id='%1';").arg(id)); //��ѯ�Լ�id��Ӧ��id���ǳ���Ϊ�Լ����û���Ϣ����
	//�����͵ķ�����Ϣ
	QString myInfo = "MyInfo";
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
	QString friendList = "FriendList";
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
		//����ڵĻ�
		if (c->getID() == recvID)
		{
			//ԭ�ⲻ��������Ϣ
			emit c->sendMsgToClientSignal(msgList.join(' '));
			return;
		}
	}
	//���û�����ߵĻ������ͷ����������ʾ��Ϣ
	emit sender->sendMsgToClientSignal(QString("Message %1 %2 %3").arg(recvID).arg(msgList[1]).arg(tr(u8"���û�Ŀǰû�����ߣ������Ϣ�����޷����")));
}

void Server::addFriendRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡ��������ӷ�id
	QString friendID = msgList[1];

	//��֤�Ƿ�������û�
	query.exec(QString("select id from acount where id='%1';").arg(friendID));
	//���û������û��Ļ�
	if (!query.next())
	{
		emit sender->sendMsgToClientSignal(QString("NoThisUser"));
	}
	else
	{
		//�����˫����Ӻ���
		query.exec(QString("insert into friend%1 values('%2');").arg(sender->getID()).arg(friendID));
		query.exec(QString("insert into friend%1 values('%2');").arg(friendID).arg(sender->getID()));

		//�����ͷ����º����б�
		friendListRequestHandle(sender, QString("FriendListRequest").split(' '));

		//�������ӷ�Ҳ���ߵĻ�����Ҳ������ӷ����º����б�
		for (int i = 0; i < threadPool.count(); i++)
		{
			if (threadPool[i]->getID() == friendID)
			{
				friendListRequestHandle(threadPool[i], QString("FriendListRequest").split(' '));
				break;
			}
		}
	}
}

void Server::delFriendRequestHandle(ClientThread * sender, QStringList msgList)
{
	//��ȡҪ��ɾ���ĺ���id
	QString friendID = msgList[1];

	//�����󷽺ͱ����󷽻���ɾ��
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(sender->getID()).arg(friendID));
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(friendID).arg(sender->getID()));

	//������������ߵĻ����������󷽸��º����б�
	for (int i = 0; i < threadPool.count(); i++)
	{
		if (threadPool[i]->getID() == friendID)
		{
			friendListRequestHandle(threadPool[i], QString("FriendListRequest").split(' '));
			break;
		}
	}
}