/*
	支付结果和游戏清晰模糊处理
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
	PAY_GIFT,   //大礼包
	PAY_GOLD,   //金币
	PAY_MONEY,  //钻石
	PAY_BLOOD,  //体力
	PAY_VIP  	//会员
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
	//是否二次弹窗{0:一次确认 1:两次确认}
	int m_isSecond;
	//模糊清晰{0:清晰 1:模糊}
	int m_isDistinct;

	static GamePay* m_instance;
};

#endif 
