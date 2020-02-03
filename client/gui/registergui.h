#ifndef REGISTERGUI_H
#define REGISTERGUI_H

#include "QDialog"
#include "QLineEdit"

//ע�����
class RegisterGui : public QDialog
{
	Q_OBJECT

public:
	RegisterGui(QWidget *parent);
	~RegisterGui();

private:
	QLineEdit *nameLineEdit; //�ǳ������
	QLineEdit *passwordLineEdit; //���������

signals:
	void registerRequestSignal(QString acountInfo); //ע������

	private slots:
	void registerButtonClicked(); //ע�ᰴť����

	public slots:
	void registerSuccessSlot(QString id); //ע��ɹ���Ӧ
};

#endif // REGISTERGUI_H
