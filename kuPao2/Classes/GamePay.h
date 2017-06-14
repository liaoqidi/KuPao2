/*
	֧���������Ϸ����ģ������
*/


#ifndef _GAMEPAY_H_
#define _GAMEPAY_H_

#include "cocos2d.h"
#include <vector>
#include "SignScene.h"
#include "MainBoard.h"

using namespace std;
using namespace cocos2d;

enum  PAY_WAY
{
	PAY_GIFT,   //�����
	PAY_GOLD,   //���
	PAY_MONEY,  //��ʯ
	PAY_BLOOD,  //����
	PAY_VIP  	//��Ա
};

class GamePay{
public:
	GamePay();

	static GamePay* getInstance();

	void gameInit(int initNum);
	void paySuccess(int payStyle);
	void payFail();
	void saveAndAdd(const char* pName, int num);
	bool getIsSecond();
	bool getIsDistinct();
	SignLayer* signLayer;
	MainBoard* mainBoard;
private:
	//�Ƿ���ε���{0:һ��ȷ�� 1:����ȷ��}
	int m_isSecond;
	//ģ������{0:���� 1:ģ��}
	int m_isDistinct;

	static GamePay* m_instance;
};

#endif 
