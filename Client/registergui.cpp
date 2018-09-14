#include "registergui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QPushButton"
#include "QMessageBox"

RegisterGui::RegisterGui(QWidget *parent)
	: QDialog(parent), nameLineEdit(new QLineEdit(this)), passwordLineEdit(new QLineEdit(this))
{
	//�����������
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowFlags(Qt::WindowCloseButtonHint);

	//���ñ���ʹ�С
	setWindowTitle(tr(u8"ע��"));
	setFixedSize(600, 300);

	//�ƶ�����Ļ����
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//�ؼ�+����
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"΢���ź�", 12));
	infoLabel->setText(tr(u8"������Ҫע����ǳƺ�����"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 6);

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setAlignment(Qt::AlignCenter);
	nameLabel->setFont(QFont(u8"΢���ź�", 10));
	nameLabel->setText(tr(u8"�ǳ�"));
	nameLabel->resize(nameLabel->sizeHint());
	nameLabel->move(infoLabel->x() + infoLabel->width() / 2 - 150, infoLabel->y() + infoLabel->height() + 50);

	QLabel *passwordLabel = new QLabel(this);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordLabel->setFont(QFont(u8"΢���ź�", 10));
	passwordLabel->setText(tr(u8"����"));
	passwordLabel->resize(passwordLabel->sizeHint());
	passwordLabel->move(nameLabel->x(), nameLabel->y() + nameLabel->height() + 50);

	nameLineEdit->resize(nameLineEdit->sizeHint());
	nameLineEdit->move(width() / 2 + (width() / 2 - nameLabel->x() - nameLabel->width()) - 70, nameLabel->y());

	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->resize(passwordLineEdit->sizeHint());
	passwordLineEdit->move(nameLineEdit->x(), passwordLabel->y());

	QPushButton *registerButton = new QPushButton(this);
	registerButton->setDefault(true);
	registerButton->setText(tr(u8"ע��"));
	registerButton->resize(registerButton->sizeHint());
	registerButton->move(width() - registerButton->width() - 10, height() - registerButton->height() - 10);
	connect(registerButton, &QPushButton::clicked, this, &RegisterGui::registerButtonClicked);
}

RegisterGui::~RegisterGui()
{

}

void RegisterGui::registerButtonClicked()
{
	//���ע���ǳƻ�����Ϊ��
	if (nameLineEdit->text() == "" || passwordLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr(u8"����"), tr(u8"�ǳƻ����벻��Ϊ�գ�"));
	}
	else
	{
		//������ע����Ϣ
		emit registerRequestSignal(nameLineEdit->text() + ' ' + passwordLineEdit->text());
	}
}

void RegisterGui::registerSuccessSlot(QString id)
{
	QMessageBox::about(this, tr(u8"ע��ɹ�"), tr(u8"���id��" + id.toUtf8()));
	nameLineEdit->clear();
	passwordLineEdit->clear();
}