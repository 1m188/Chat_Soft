#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
	: QTcpSocket(parent)
{

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