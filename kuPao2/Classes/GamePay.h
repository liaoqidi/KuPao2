/*
	支付结果和游戏清晰模糊处理
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
	PAY_GIFT = 0,   //大礼包
	PAY_GOLD,		//金币
	PAY_MONEY,		//钻石
	PAY_BLOOD,      //体力
	PAY_VIP,		//会员
};


class GamePay{
public:
	GamePay();
	void gameInit(char* str);
	void paySuccess(int payStyle);
	
	void payFail();                   
	//字符串拆分
	vector<char*> splitWithStl(char *Str);
	//是否二次弹窗{0:一次确认 1:两次确认}
	char *m_isSecond;
	//模糊清晰{0:清晰 1:模糊}
	char *m_isDistinct;
};

#endif 
