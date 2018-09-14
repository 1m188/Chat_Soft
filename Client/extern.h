#ifndef EXTERN_H
#define EXTERN_H

#include "QString"

//�����������Ϣ������ö��
const enum FriendInfoNum
{
	id,
	name,
};

//�û���Ϣ
struct UserInfo
{
	QString id; //id
	QString name; //�ǳ�

	UserInfo::UserInfo() :id(""), name("") {}
	UserInfo::UserInfo(QString id, QString name) : id(id), name(name) {}
};

#endif // !EXTERN_H
