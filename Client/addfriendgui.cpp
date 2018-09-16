#include "addfriendgui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QRegExpValidator"
#include "QPushButton"
#include "QMessageBox"

AddFriendGui::AddFriendGui(QWidget *parent, UserInfo myInfo)
	: QWidget(parent), myInfo(myInfo), idLineEdit(new QLineEdit(this))
{
	//�����������
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, false);

	//���ý������ʹ�С
	setWindowTitle(tr(u8"��Ӻ���"));
	setFixedSize(600, 300);

	//�ƶ�����Ļ����λ��
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//�ؼ�+����
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"΢���ź�", 15));
	infoLabel->setText(tr(u8"������Ҫ��ӵĺ��ѵ�ID"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 2 / 2 - infoLabel->height() / 2);

	QLabel *idLabel = new QLabel(this);
	idLabel->setAlignment(Qt::AlignCenter);
	idLabel->setFont(QFont(u8"΢���ź�", 10));
	idLabel->setText(tr(u8"�û�ID"));
	idLabel->resize(idLabel->sizeHint());
	idLabel->move(width() / 2 - idLabel->width() - 80, infoLabel->y() + infoLabel->height() + 65);

	idLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$"), idLineEdit)); ///����id����������
	idLineEdit->resize(idLineEdit->sizeHint());
	idLineEdit->move(width() / 2 + width() / 2 - idLabel->x() - idLabel->width() - 50, idLabel->y() + idLabel->height() / 2 - idLineEdit->height() / 2);
	connect(idLineEdit, &QLineEdit::returnPressed, this, &AddFriendGui::addButtonClicked);

	QPushButton *addButton = new QPushButton(this);
	addButton->setFont(QFont(u8"΢���ź�", 9));
	addButton->setText(tr(u8"���"));
	addButton->resize(addButton->sizeHint());
	addButton->move(width() - addButton->width() - 10, height() - addButton->height() - 10);
	connect(addButton, &QPushButton::clicked, this, &AddFriendGui::addButtonClicked);
}

AddFriendGui::~AddFriendGui()
{

}

void AddFriendGui::closeEvent(QCloseEvent * event)
{
	//���͹ر��źţ�����������ý����Ѿ��رգ��������´�
	emit closeSignal();
	return QWidget::closeEvent(event);
}

void AddFriendGui::addButtonClicked()
{
	//��ȡҪ��ӵĺ���id
	QString friendID = idLineEdit->text();
	//���Ϊ��
	if (friendID == "")
	{
		QMessageBox::critical(this, tr(u8"����"), tr(u8"����id����Ϊ�գ�"), QMessageBox::Ok);
	}
	else if (friendID == myInfo.id)
	{
		QMessageBox::critical(this, tr(u8"����"), tr(u8"��������Լ�Ϊ���ѣ�"), QMessageBox::Ok);
	}
	else
	{
		//��������Ӻ����ź�
		emit addFriendRequestSignal(friendID);
	}
}

void AddFriendGui::addFriendRepeatSlot()
{
	QMessageBox::information(this, tr(u8"�ظ����"), tr(u8"���Ѿ������������ѣ�"), QMessageBox::Ok);
}

void AddFriendGui::noThisUserSlot()
{
	QMessageBox::critical(this, tr(u8"����"), tr(u8"û�д��û���"), QMessageBox::Ok);
}