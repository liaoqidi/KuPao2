/*
	支付结果和游戏清晰模糊处理
*/


#ifndef _GAMEPAY_H_
#define _GAMEPAY_H_

#include "cocos2d.h"
using namespace cocos2d;

enum  PAY_WAY
{
	PAY_GIFT,   //大礼包
	PAY_GOLD,	//金币
	PAY_MONEY,  //钻石
	PAY_VIP,    //会员
};


class GamePay{
public:
	GamePay();
	void gameInit(const char* str);
	void paySuccess(int payStyle);
	void payFail();

	bool m_isSecond;		      //是否二次弹窗
	bool m_isDistinct;	          //模糊清晰
	int  m_pointType;             //购买类型
	std::vector<std::string> vec; //
};

#endif 
