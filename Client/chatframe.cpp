#include "chatframe.h"
#include "QPushButton"
#include "QGridLayout"
#include "QEvent"
#include "QKeyEvent"

ChatFrame::ChatFrame(QWidget *parent, UserInfo friendInfo)
	: QFrame(parent), friendInfo(friendInfo), sendTextEdit(new QTextEdit(this)), recvTextEdit(new QTextEdit(this))
{
	//�ؼ�
	recvTextEdit->setReadOnly(true); //��������Ϣ�ı�������Ϊֻ��
	sendTextEdit->installEventFilter(this); //��װ�¼�����������ʵ�ֻس���������Ϣ��������+�س������еĹ���

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

bool ChatFrame::eventFilter(QObject * watched, QEvent * event)
{
	//�����ǰ����۽��������ı���
	if (watched == sendTextEdit)
	{
		//����¼�Ϊ����
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *key = static_cast<QKeyEvent *>(event);
			//������µ��ǻس�����û���κ����μ�
			if (key->modifiers() == Qt::KeyboardModifier::NoModifier && key->key() == Qt::Key_Return)
			{
				//�򴥷�������Ϣ
				sendButtonClicked();
				return true;
			}
			//����������µļ��ǻس�
			else if (key->key() == Qt::Key_Return)
			{
				key->setModifiers(Qt::KeyboardModifier::NoModifier); //ȡ�����μ������÷����ı���ִ�л��в���
				return false;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	//���򽻸��������غ���ʵ��
	else
	{
		return QWidget::eventFilter(watched, event);
	}
}

void ChatFrame::sendButtonClicked()
{
	//��ȡҪ���͵���Ϣ
	QString msg = sendTextEdit->toPlainText();
	//�����Ϊ��
	if (msg != "")
	{
		//���½�����Ϣ�ı���
		recvTextEdit->append(u8"��: " + msg);
		//����
		emit sendMsgSignal(msg, friendInfo.id);
		//��շ����ı���
		sendTextEdit->clear();
	}
}

void ChatFrame::getMsgSlot(QString msg, QString senderID)
{
	//��������Ϣ�Ƿ����Լ���
	if (senderID == friendInfo.id)
	{
		//���½�����Ϣ�ı���
		recvTextEdit->append(friendInfo.name + ": " + msg);
	}
}