#include "chatframe.h"

ChatFrame::ChatFrame(QWidget *parent, UserInfo friendInfo)
	: QFrame(parent), friendInfo(friendInfo), sendTextEdit(new QTextEdit(this)), recvTextEdit(new QTextEdit(this))
{
	//�ؼ�
	recvTextEdit->setReadOnly(true);

	QPushButton *sendButton = new QPushButton(this);
	sendButton->setDefault(true);
	sendButton->setFont(QFont(u8"΢���ź�", 10));
	sendButton->setText(tr(u8"����"));
	sendButton->resize(sendButton->sizeHint());
	connect(sendButton, &QPushButton::clicked, this, &ChatFrame::sendButtonClicked);

	//����
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(recvTextEdit, 0, 0, 10, 10);
	layout->addWidget(sendTextEdit, 10, 0, 5, 10);
	layout->addWidget(sendButton, 15, 9, 1, 1);
}

ChatFrame::~ChatFrame()
{

}

void ChatFrame::sendButtonClicked()
{
	QString msg = sendTextEdit->toPlainText();
	if (msg != "")
	{
		recvTextEdit->append(u8"��: " + msg);
		emit sendMsgSignal(msg, friendInfo.id);
		sendTextEdit->clear();
	}
}

void ChatFrame::getMsgSlot(QString msg, QString senderID)
{
	if (senderID == friendInfo.id)
	{
		recvTextEdit->append(friendInfo.name + ": " + msg);
	}
}