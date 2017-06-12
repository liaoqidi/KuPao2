/*
	֧���������Ϸ����ģ������
*/


#ifndef _GAMEPAY_H_
#define _GAMEPAY_H_

#include "cocos2d.h"
using namespace cocos2d;

enum  PAY_WAY
{
	PAY_GIFT,   //�����
	PAY_GOLD,	//���
	PAY_MONEY,  //��ʯ
	PAY_VIP,    //��Ա
};


class GamePay{
public:
	GamePay();
	void gameInit(const char* str);
	void paySuccess(int payStyle);
	void payFail();

	bool m_isSecond;		      //�Ƿ���ε���
	bool m_isDistinct;	          //ģ������
	int  m_pointType;             //��������
	std::vector<std::string> vec; //
};

#endif 
