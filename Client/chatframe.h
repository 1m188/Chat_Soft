#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <QFrame>
#include "QTextEdit"
#include "extern.h"

//�������
class ChatFrame : public QFrame
{
	Q_OBJECT

public:
	ChatFrame(QWidget *parent, UserInfo friendInfo);
	~ChatFrame();

private:
	UserInfo friendInfo; //������������ĺ��ѵ��û���Ϣ
	QTextEdit *sendTextEdit; //������Ϣ��textedit
	QTextEdit *recvTextEdit; //������Ϣ��textedit

public:
	const QString getID() const { return friendInfo.id; } //��ȡ�ú���id
	const QString getName() const { return friendInfo.name; } //��ȡ�ú����ǳ�

signals:
	void sendMsgSignal(QString msg, QString recverID); //����������Ϣ

	private slots:
	void sendButtonClicked(); //�������Ͱ�ť

	public slots:
	void getMsgSlot(QString msg, QString senderID); //���պ��ѵ�������Ϣ
};

#endif // CHATFRAME_H
