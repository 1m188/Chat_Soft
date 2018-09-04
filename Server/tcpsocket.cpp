#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
	: QTcpSocket(parent), buffer(QByteArray()), headLen(sizeof(qint32)), msgLen(0)
{
	connect(this, &TcpSocket::readyRead, this, &TcpSocket::getMsg);
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::writeMsg(QString msg)
{
	QByteArray sendMsg; //Ҫ���͵İ�
	QDataStream out(&sendMsg, QIODevice::WriteOnly); //�������ݰ���д��QDataStream
	out.setVersion(QDataStream::Qt_5_9);
	out << static_cast<qint32>(msg.size() * 2) << msg; //����һ��4�ֽڵ�Ҫ���͵���Ϣ�Ĵ�С������QString��תΪunicode��ÿ��ԭ�����ַ�ռ�����ֽڣ������ֽڴ�СҪ��ԭ����size�Ļ�����x2����Ȼ���ٶ���Ҫ���͵���Ϣ //�ؼ�֮��
	write(sendMsg); //����
}

void TcpSocket::getMsg()
{
	buffer.append(readAll());
	int totalLen = buffer.size();
	while (totalLen)
	{
		QDataStream in(&buffer, QIODevice::ReadOnly);
		in.setVersion(QDataStream::Qt_5_9);

		if (msgLen == 0)
		{
			if (totalLen >= headLen)
			{
				in >> msgLen;
				if (totalLen >= headLen + msgLen)
				{
					QString msg;
					in >> msg;
					emit getMsgSignal(msg);
					buffer = buffer.right(totalLen - headLen - msgLen - 4);
					msgLen = 0;
					totalLen = buffer.size();
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
		else
		{
			if (totalLen >= headLen + msgLen)
			{
				int temp = 0;
				in >> temp;
				QString msg;
				in >> msg;
				emit getMsgSignal(msg);
				buffer = buffer.right(totalLen - headLen - msgLen - 4);
				msgLen = 0;
				totalLen = buffer.size();
			}
			else
			{
				break;
			}
		}
	}
}