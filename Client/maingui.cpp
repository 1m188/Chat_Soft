#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QMenuBar"
#include "QGridLayout"
#include "addfriendgui.h"
#include "QMessageBox"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent), myInfo({}), friendList(new QListWidget(this)), framePlace(new QWidget(this)), chatFramePool({}), isAddFriendGuiOpen(false)
{
	//�����������
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	//���ý���������ʹ�С
	setWindowTitle(myInfo.id + "-" + myInfo.name);
	resize(1300, 800);
	setMinimumSize(size());

	//�ѽ��������Ļ�м�
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//�ؼ�
	friendList->setFont(QFont(u8"΢���ź�", 12));
	connect(friendList, &QListWidget::itemDoubleClicked, this, &MainGui::friendListItemDoubleClicked);

	//����˵�
	QMenuBar *menuBar = new QMenuBar(this);
	menuBar->setStyleSheet("QMenuBar{background-color:rgb(240,240,240)}");

	QMenu *menu = new QMenu(menuBar);
	menu->setTitle(tr(u8"�˵�"));
	menuBar->addMenu(menu);

	QAction *addFriendAction = new QAction(menu);
	addFriendAction->setText(tr(u8"��Ӻ���"));
	connect(addFriendAction, &QAction::triggered, this, &MainGui::addFriendActionTriggered);
	menu->addAction(addFriendAction);

	QAction * delFriendAction = new QAction(menu);
	delFriendAction->setText(tr(u8"ɾ������"));
	connect(delFriendAction, &QAction::triggered, this, &MainGui::delFriendActionTriggered);
	menu->addAction(delFriendAction);

	QAction *videoAction = new QAction(menu);
	videoAction->setText(tr(u8"��Ƶͨ��"));
	connect(videoAction, &QAction::triggered, this, &MainGui::videoActionTriggered);
	menu->addAction(videoAction);

	//����
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(friendList, 0, 0, -1, 4);
	layout->addWidget(framePlace, 0, 4, -1, 16);
	layout->setMenuBar(menuBar);
}

MainGui::~MainGui()
{

}

void MainGui::resizeEvent(QResizeEvent * event)
{
	//��������������б��а�ÿ��������涼�ı��С
	for (ChatFrame *c : chatFramePool)
	{
		c->resize(framePlace->size());
	}
	return QWidget::resizeEvent(event);
}

void MainGui::friendListItemDoubleClicked(QListWidgetItem *item)
{
	//������������б���������ѵ���������Ƿ��
	for (ChatFrame *c : chatFramePool)
	{
		//�������
		if (c->getID() == item->data(Qt::UserRole + FriendInfoNum::id).toString())
		{
			//�ŵ����
			c->raise();
			//�������������
			setWindowTitle(windowTitle().split("---")[0] + "---" + c->getID() + "-" + c->getName());
			//����
			return;
		}
	}

	//û�н����Ļ����ǿ϶���û�д򿪣����
	ChatFrame *chatFrame = new ChatFrame(framePlace, UserInfo(item->data(Qt::UserRole + FriendInfoNum::id).toString(), item->data(Qt::UserRole + FriendInfoNum::name).toString()));
	setWindowTitle(windowTitle().split("---")[0] + "---" + item->data(Qt::UserRole + FriendInfoNum::id).toString() + "-" + item->data(Qt::UserRole + FriendInfoNum::name).toString());
	chatFrame->resize(framePlace->size());
	connect(chatFrame, &ChatFrame::sendMsgSignal, this, &MainGui::sendMsgSignal); //����������Ϣ
	connect(this, &MainGui::getMsgSignal, chatFrame, &ChatFrame::getMsgSlot); //����������Ϣ
	chatFramePool.push_back(chatFrame); //������������б�
	chatFrame->show();
}

void MainGui::getMyInfoSlot(QString id, QString name)
{
	//�����Լ����û���Ϣ
	myInfo.id = id;
	myInfo.name = name;
	//�������������
	setWindowTitle(myInfo.id + "-" + myInfo.name);
}

void MainGui::updateFriendList(QStringList friendList)
{
	//�����ԭ���ĺ����б���ֹ�����ظ�������
	for (int i = 0; i < this->friendList->count(); i++)
	{
		delete this->friendList->item(i);
	}
	this->friendList->clear();

	//�������
	for (QString s : friendList)
	{
		QStringList sList = s.split('_');
		QListWidgetItem *item = new QListWidgetItem(this->friendList);
		item->setText(sList[1]);
		item->setData(Qt::UserRole + FriendInfoNum::id, sList[0]);
		item->setData(Qt::UserRole + FriendInfoNum::name, sList[1]);
		this->friendList->addItem(item);
	}

	//����Ǳ�ɾ�����ѵ�һ���������º����б���Ҫ����صĽ���ɾ��
	for (int i = 0; i < chatFramePool.count(); i++)
	{
		bool isExist = false; //�򿪵���������id�Ƿ��ں����б���

		//���򿪵���������id�Ƿ���ں����б���
		for (int j = 0; j < this->friendList->count(); j++)
		{
			if (this->friendList->item(j)->data(Qt::UserRole + FriendInfoNum::id).toString() == chatFramePool[i]->getID())
			{
				//�������
				isExist = true;
				break;
			}
		}

		//�������
		if (!isExist)
		{
			//��Ȼ�Ļ��͹رղ�ɾ��������
			chatFramePool[i]->close();
			chatFramePool[i]->deleteLater();
			chatFramePool.removeAt(i);
		}
	}
}

void MainGui::getMsgSlot(QString msg, QString senderID)
{
	//�ں����б����ҵ���Ӧ�ķ��ͷ���ģ��˫�������������������ѵ���������Ƿ�����ն����
	for (int i = 0; i < friendList->count(); i++)
	{
		if (friendList->item(i)->data(Qt::UserRole + FriendInfoNum::id).toString() == senderID)
		{
			friendListItemDoubleClicked(friendList->item(i));
			emit getMsgSignal(msg, senderID);
			break;
		}
	}
}

void MainGui::addFriendActionTriggered()
{
	//ʹ��Ӻ��ѽ�����ֵ��������Ľ������ɿ��Բ���������Ӻ��ѽ���ֻ����һ��
	if (!isAddFriendGuiOpen)
	{
		AddFriendGui *addFriendGui = new AddFriendGui(nullptr, myInfo);
		isAddFriendGuiOpen = true;
		connect(addFriendGui, &AddFriendGui::closeSignal, this, [&]() {isAddFriendGuiOpen = false; }); //��Ӻ��ѽ���Ψһ
		connect(addFriendGui, &AddFriendGui::addFriendRequestSignal, this, &MainGui::addFriendRequestSlot); //��Ӻ���
		connect(this, &MainGui::addFriendRepeatSignal, addFriendGui, &AddFriendGui::addFriendRepeatSlot); //�Ѿ�������������
		connect(this, &MainGui::noThisUserSignal, addFriendGui, &AddFriendGui::noThisUserSlot); //��Ӻ��ѵ�ʱ�򷵻�û������û�
		addFriendGui->show();
	}
}

void MainGui::addFriendRequestSlot(QString friendID)
{
	//��������б�������������id���������󣬽���
	for (int i = 0; i < friendList->count(); i++)
	{
		if (friendID == friendList->item(i)->data(Qt::UserRole + FriendInfoNum::id).toString())
		{
			emit addFriendRepeatSignal();
			return;
		}
	}

	//���򣬷��ͺ����������
	emit addFriendRequestSignal(friendID);
}

void MainGui::delFriendActionTriggered()
{
	//��ȡ��ǰѡ����
	QList<QListWidgetItem *> items = friendList->selectedItems();
	//�����
	if (items.count())
	{
		//�ж��Ƿ�Ҫɾ������
		if (QMessageBox::warning(this, tr(u8"����"), tr((QString(u8"�����Ҫɾ����ĺ��� %1 ��").arg(items[0]->data(Qt::UserRole + FriendInfoNum::name).toString()).toUtf8())), QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			//������ɾ����������
			emit delFriendRequestSignal(items[0]->data(Qt::UserRole + FriendInfoNum::id).toString());
			//������������˵Ļ���Ҫ�ر�
			for (int i = 0; i < chatFramePool.count(); i++)
			{
				if (chatFramePool[i]->getID() == items[0]->data(Qt::UserRole + FriendInfoNum::id).toString())
				{
					chatFramePool[i]->close();
					chatFramePool[i]->deleteLater();
					chatFramePool.removeAt(i);
					break;
				}
			}
			//����ɾ�������б������
			friendList->removeItemWidget(items[0]);
			delete items[0];
		}
	}
}

void MainGui::videoActionTriggered()
{
}