#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

//�Ϳͻ������ӵ�socket
class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private:
	QByteArray buffer; //��Ϣ������
	const qint32 headLen; //��ͷ��С
	qint32 msgLen; //�����С

signals:
	void getMsgSignal(QString msg); //��õ�����������Ϣ���ź�

private slots:
	void getMsg(); //�����µ���Ϣ������ʱ��͵��������ͨ��ճ������ķ�ʽ��ȡ�µ���Ϣ

public:
	void writeMsg(QString msg); //������Ϣ��ճ������
};

#endif // TCPSOCKET_H
