#include "server.h"

Server::Server(QObject *parent)
	: QTcpServer(parent), threadPool({}), database(QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))), query(QSqlQuery(database))
{
	database.setDatabaseName("softchat");
	database.setHostName("127.0.0.1");
	database.setPort(3306);
	database.setUserName("root");
	database.setPassword("123456");
	database.open();

	listen(QHostAddress::LocalHost, 8888);
}

Server::~Server()
{
	query.clear();
	database.close();
}

void Server::incomingConnection(qintptr handle)
{
	qDebug() << "a new connection with handle" << handle;

	ClientThread *clientThread = new ClientThread(nullptr, handle);
	connect(clientThread, &ClientThread::disconnectToClientSignal, this, &Server::disconnectToClientSlot);
	connect(clientThread, &ClientThread::getMsgFromClientSignal, this, &Server::getMsgFromClientSlot);
	threadPool.push_back(clientThread);
	clientThread->start();
}

void Server::disconnectToClientSlot()
{
	ClientThread *s = dynamic_cast<ClientThread *>(sender());
	s->exit();
	threadPool.removeOne(s);
	s->deleteLater();
}

void Server::getMsgFromClientSlot(QString msg)
{
	ClientThread *s = dynamic_cast<ClientThread *>(sender());

	qDebug() << s->getID() << "id send message" << msg;

	if (msg == "myInfo")
	{
		QString id = s->getID();
		query.exec(QString("select name from acount where id='%1';").arg(id)); //��ѯ�Լ�id��Ӧ��id���ǳ���Ϊ�Լ����û���Ϣ����
		QString myInfo = "myInfo";
		myInfo += ' ';
		myInfo += id;
		myInfo += ' ';
		if (query.next())
		{
			myInfo += query.value("name").toString();
		}
		emit s->sendMsgToClientSignal(myInfo);
	}
	else if (msg == "friendList")
	{
		QString id = s->getID();
		query.exec(QString("select id,name from acount,friend%1 where acount.id=friend%2.friendid;").arg(id).arg(id)); //��ѯ��Ӧid�ĺ��ѱ��е����еĺ��ѵ�id���ǳ�
		QString friendList = "friendList";
		while (query.next())
		{
			friendList += ' ';
			friendList += query.value("id").toString();
			friendList += '_';
			friendList += query.value("name").toString();
		}
		emit s->sendMsgToClientSignal(friendList);
	}
	else
	{
		QStringList msgList = msg.split(' ');

		if (msgList[0] == "login")
		{
			QString id = msgList[1];
			QString password = msgList[2];
			query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password)); //�����ѯ���������ӵĽ������
			if (query.next())
			{
				for (ClientThread *c : threadPool)
				{
					if (c->getID() == id)
					{
						emit s->sendMsgToClientSignal("loginRepeat"); //�ظ���½
						return;
					}
				}
				emit s->sendMsgToClientSignal("loginSuccess"); //��½�ɹ�
				s->setID(id);
			}
			else
			{
				emit s->sendMsgToClientSignal("loginFailed"); //��½ʧ��
			}
		}
		else if (msgList[0] == "register")
		{
			QString name = msgList[1];
			QString password = msgList[2];
			query.exec("select * from acount");
			QString id = QString::number(query.size() + 1);
			query.exec(QString("insert into acount values('%1','%2','%3')").arg(id).arg(name).arg(password)); //�����û������Ϣ���û�����
			query.exec(QString("create table friend%1(friendid char(10) not null);").arg(id)); //����һ���û��ĺ��ѱ�
			emit s->sendMsgToClientSignal(QString("registerSuccess") + ' ' + id); //ע��ɹ�
		}
		else if (msgList[0] == "Message")
		{
			QString recvID = msgList[2];
			for (ClientThread *c : threadPool)
			{
				if (c->getID() == recvID)
				{
					emit c->sendMsgToClientSignal(msg);
				}
			}
		}
	}
}