#ifndef LOGINGUI_H
#define LOGINGUI_H

#include <QDialog>
#include "QLineEdit"

//��½����
class LoginGui : public QDialog
{
	Q_OBJECT

public:
	LoginGui(QWidget *parent);
	~LoginGui();

private:
	QLineEdit *idLineEdit; //id�����
	QLineEdit *passwordLineEdit; //���������

signals:
	void loginRequestSignal(QString acountInfo); //��½����
	void registerRequestSignal(QString acountInfo); //ע������
	void registerSuccessSignal(QString id); //ע��ɹ�

	private slots:
	void loginButtonClicked(); //��½��ť����
	void registerButtonClicked(); //ע�ᰴť����

	public slots:
	void loginFailedSlot(); //��½ʧ��
	void loginRepeatSlot(); //�ظ���½
};

#endif // LOGINGUI_H
