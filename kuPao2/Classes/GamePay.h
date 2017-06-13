/*
	֧���������Ϸ����ģ������
*/


#ifndef _GAMEPAY_H_
#define _GAMEPAY_H_

#include "cocos2d.h"
#include <string.h>
#include <vector>
using namespace std;
using namespace cocos2d;

enum  PAY_WAY
{
	PAY_GIFT = 0,   //�����
	PAY_GOLD,		//���
	PAY_MONEY,		//��ʯ
	PAY_BLOOD,      //����
	PAY_VIP,		//��Ա
};


class GamePay{
public:
	GamePay();
	void gameInit(char* str);
	void paySuccess(int payStyle);
	
	void payFail();                   
	//�ַ������
	vector<char*> splitWithStl(char *Str);
	//�Ƿ���ε���{0:һ��ȷ�� 1:����ȷ��}
	char *m_isSecond;
	//ģ������{0:���� 1:ģ��}
	char *m_isDistinct;
};

#endif 
