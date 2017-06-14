/*
	支付sdk管理类
*/

#ifndef _SDKManager_H_
#define _SDKManager_H_

#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class SDKManager{
public:
	void pay(int priceId);
	void init();
	int m_payStyle;
	static SDKManager *getinstance();
private:
	static SDKManager *m_instance;
};
#endif

#endif
