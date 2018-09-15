#include "data.h"
#include "QThread"

Data::Data(QObject *parent)
	: QObject(parent), connectToServer(nullptr), myInfo(nullptr)
{
	//�ѱ���������źŲ���Ӧ������һ���߳���
	QThread *thread = new QThread();
	moveToThread(thread);
	connect(thread, &QThread::started, this, &Data::init); //�߳̿�ʼ��������ʼ��
	//��Դ����
	connect(this, &Data::destroyed, thread, &QThread::quit);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
	thread->start(); //����
}

Data::~Data()
{
	//�ͷ�����ڴ���Դ
	delete myInfo;
}

void Data::init()
{
	//���ⲿ�������õĽӿ��з��͵��źź�����źŲ۵�����
	connect(this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSlot));
	connect(this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSlot));

	connectToServer = new TcpSocket(nullptr); //��ʼ���ͷ��������ӵ�socket
	connect(this, &Data::destroyed, connectToServer, &TcpSocket::deleteLater); //��Դ����
	connect(connectToServer, &TcpSocket::getMsgSignal, this, &Data::getMsgFromServer); //�ӷ�������ȡ����Ϣ����
	connectToServer->connectToHost("127.0.0.1", 8888); //���ӵ���ط����������أ���8888�˿�

	myInfo = new UserInfo(); //��ʼ���Լ����û���Ϣ
}

void Data::addSignalSlotsForClassSlot(LoginGui *loginGui)
{
	connect(loginGui, &LoginGui::loginRequestSignal, this, &Data::loginRequestSlot); //��½����
	connect(loginGui, &LoginGui::registerRequestSignal, this, &Data::registerRequestSlot); //ע������
	connect(this, &Data::loginSignal, loginGui, &LoginGui::accept); //ʹ֮��½
	connect(this, &Data::loginFailedSignal, loginGui, &LoginGui::loginFailedSlot); //��½ʧ��
	connect(this, &Data::loginRepeatSignal, loginGui, &LoginGui::loginRepeatSlot); //�ظ���½
	connect(this, &Data::registerSuccessSignal, loginGui, &LoginGui::registerSuccessSignal); //ע��ɹ�
}

void Data::addSignalSlotsForClassSlot(MainGui *mainGui)
{
	connect(mainGui, &MainGui::sendMsgSignal, this, &Data::sendMsgSlot); //����������Ϣ
	connect(this, &Data::getMyInfoSignal, mainGui, &MainGui::getMyInfoSlot); //��ȡ�Լ����û���Ϣ
	connect(this, &Data::getFriendListSignal, mainGui, &MainGui::updateFriendList); //��ȡ�����б��Ҹ��º����б�
	connect(this, &Data::getMsgSignal, mainGui, &MainGui::getMsgSlot); //���յ�������Ϣ
	connect(mainGui, &MainGui::addFriendSignal, this, &Data::addFriendSlot); //������Ӻ�������
	connect(this, &Data::noThisUserSignal, mainGui, &MainGui::noThisUserSignal); //��Ӻ���ʱ����û������û�

	connectToServer->writeMsg("MyInfoRequest"); //�����Լ��û���Ϣ����
	connectToServer->writeMsg("FriendListRequest"); //���ͺ����б�����
}

void Data::getMsgFromServer(QString msg)
{
	//���������Ϣ
#ifdef _DEBUG
	qDebug() << msg;
#endif // _DEBUG

	//��ȡ��Ϣͨ���ո�ָ����б�
	QStringList msgList = msg.split(' ');
	//�ж�
	//��½�ɹ�
	if (msgList[0] == "LoginSuccess")
	{
		loginSuccessHandle(msgList);
	}
	//��½ʧ��
	else if (msgList[0] == "LoginFailed")
	{
		loginFailedHandle(msgList);
	}
	//�ظ���½
	else if (msgList[0] == "LoginRepeat")
	{
		loginRepeatHandle(msgList);
	}
	//ע��ɹ�
	else if (msgList[0] == "RegisterSuccess")
	{
		registerSuccessHandle(msgList);
	}
	//��ȡ�Լ����û���Ϣ
	else if (msgList[0] == "MyInfo")
	{
		getMyInfoHandle(msgList);
	}
	//��ȡ�����б�
	else if (msgList[0] == "FriendList")
	{
		getFriendListHandle(msgList);
	}
	//���պ��ѷ��͹�����������Ϣ
	else if (msgList[0] == "Message")
	{
		messageHandle(msgList);
	}
	//��Ӻ��ѣ�û������û��Ļ�Ӧ
	else if (msgList[0] == "NoThisUser")
	{
		noThisUserHandle(msgList);
	}
}

void Data::loginSuccessHandle(QStringList msgList)
{
	emit loginSignal();
}

void Data::loginFailedHandle(QStringList msgList)
{
	emit loginFailedSignal();
}

void Data::loginRepeatHandle(QStringList msgList)
{
	emit loginRepeatSignal();
}

void Data::registerSuccessHandle(QStringList msgList)
{
	emit registerSuccessSignal(msgList[1]);
}

void Data::getMyInfoHandle(QStringList msgList)
{
	//�����Լ��û���Ϣ
	myInfo->id = msgList[1];
	myInfo->name = msgList[2];
	//���ݸ�������
	emit getMyInfoSignal(msgList[1], msgList[2]);
}

void Data::getFriendListHandle(QStringList msgList)
{
	msgList.pop_front(); //ȥ��FriendList����Ϣͷ
	emit getFriendListSignal(msgList); //�Ѵ���ĺ����б����Ϣ���͹�ȥ
}

void Data::messageHandle(QStringList msgList)
{
	QString senderID = msgList[1]; //��ȡ���ͷ�id
	QString recverID = msgList[2]; //��ȡ���շ�id
	//ȥ��Message�����ͷ�id�ͽ��շ�id
	msgList.pop_front();
	msgList.pop_front();
	msgList.pop_front();
	//��ȡ��������Ϣ
	QString msg = msgList.join(' ');
	//������Ϣ�ͷ��ͷ�id
	emit getMsgSignal(msg, senderID);
}

void Data::noThisUserHandle(QStringList msgList)
{
	emit noThisUserSignal();
}



void Data::loginRequestSlot(QString acountInfo)
{
	QStringList acountInfoList = acountInfo.split(' ');
	connectToServer->writeMsg(QString("LoginRequest %1 %2").arg(acountInfoList[0]).arg(acountInfoList[1]));
}

void Data::registerRequestSlot(QString acountInfo)
{
	QStringList acountInfoList = acountInfo.split(' ');
	connectToServer->writeMsg(QString("RegisterRequest %1 %2").arg(acountInfoList[0].arg(acountInfoList[1])));
}

void Data::sendMsgSlot(QString msg, QString recverID)
{
	connectToServer->writeMsg(QString("Message %1 %2 %3").arg(myInfo->id).arg(recverID).arg(msg));
}

void Data::addFriendSlot(QString friendID)
{
	connectToServer->writeMsg(QString("AddFriendRequest %1").arg(friendID));
}