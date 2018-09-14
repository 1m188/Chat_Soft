#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

//�ͷ��������ӵ�socket
class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private:
	QByteArray buffer; //������Ϣ������
	const qint32 headLen; //��ͷ��С
	qint32 msgLen; //����

public:
	void writeMsg(QString msg); //������Ϣ������ճ������

signals:
	void getMsgSignal(QString msg); //���յ�����Ϣ�ź�

	private slots:
	void getMsg(); //ճ������
};

#endif // TCPSOCKET_H
