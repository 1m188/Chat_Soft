#include "clientthread.h"

ClientThread::ClientThread(QObject *parent, qintptr handle)
	: QThread(parent), connectToClient(nullptr), handle(handle), id(""), headLen(sizeof(qint32)), msgLen(0), buffer(QByteArray())
{
	
}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
	connectToClient = new QTcpSocket(nullptr);
	connectToClient->setSocketDescriptor(handle);
	connect(this, &QThread::finished, connectToClient, &QTcpSocket::deleteLater);
	connect(connectToClient, &QTcpSocket::readyRead, this, &ClientThread::getMsgFromClient);
	connect(this, &ClientThread::sendMsgToClientSignal, connectToClient, [&](QString msg) {connectToClient->write(msg.toUtf8()); connectToClient->waitForBytesWritten(); });
	exec();
}

void ClientThread::getMsgFromClient()
{
	//ճ�������
	buffer.append(connectToClient->readAll());
	int totalLen = buffer.size(); //ÿ���յ�������ȫ���ù����������������ܳ���
	//���ܳ���>=0��ʱ��ѭ��
	while (totalLen)
	{
		//��QDataStream������QByteArray����
		QDataStream in(&buffer, QIODevice::ReadOnly); //ֻ��
		in.setVersion(QDataStream::Qt_5_9);

		if (msgLen == 0) //����Ϣ����Ϊ0��ʱ��˵����ʱ��û�ж�ȡ�κ���Ϣ�ĳ��ȣ����Դ�ʱӦ�ö�ͷ
		{
			if (totalLen >= headLen) //����ܳ�>=ͷ����������Ȼ����ѭ���Ⱥ���İ�
			{
				in >> msgLen; //��һ��
				if (totalLen >= headLen + msgLen) //����ܳ�������ͷ��+�����Ļ�������������Ϊ��������Ϣ����Ȼ����ѭ�����´θ������Ϣ��
				{
					QString msg;
					in >> msg;
					emit getMsgFromClientSignal(msg);
					buffer = buffer.right(totalLen - headLen - msgLen - 4); //���»���������ͷ��С+��Ϣ��С+QDtaStreamд��QByteArray��ʱ���Զ����ϵ�4�ֽڵİ�ͷ������֮���Բ��ڷ��͵�ʱ��ȥ�������ڸ��»�������ʱ��������Ϊ�����û������QDataStream�Լ����ϵ�4���ֽ�����QDataStream�޷�ʶ����Ӧ�����ݣ������û�취��QDataStream����ȡ����//�ؼ�֮��
					msgLen = 0; //�Ѱ���=0�������´μ�����ͷ
					totalLen = buffer.size(); //�����ܳ���
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else //���򣬱ض��Ѿ��Ƕ��˰�������û�аѰ�����Ϣ��������
		{
			if (totalLen >= headLen + msgLen) //�ж��Ƿ�ɶ�����
			{
				QString msg;
				in >> msg;
				emit getMsgFromClientSignal(msg);
				buffer = buffer.right(totalLen - headLen - msgLen - 4);
				msgLen = 0;
				totalLen = buffer.size();
			}
			else //���Ȳ����Ļ�����ѭ������һ����Ϣ����
			{
				break;
			}
		}
	}
}