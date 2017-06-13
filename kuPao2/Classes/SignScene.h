#pragma once

#include "cocos2d.h"
#include "GameScene.h"
using namespace cocos2d;

class SignLayer;
class SecondRevgift;

class SignLayer : public CCLayer{

private:
	CCSprite* pBackground1;
	CCSprite* pBackground2;
	CCSprite* pContentDay; 
	CCMenu* pMenu;
	CCUserDefault* pSava;
	CCMenuItemImage* pSign;
	CCMenuItemImage* pSignClose;
	CCNode* pNode;
	int contentDay;
	int keepDay; 
public:
	bool init();
	bool showSign();
	void revGift();				//ÁìÈ¡Àñ°ü
	void sign(CCObject* pSender);    
	void signClose(CCObject* pSender);
	CREATE_FUNC(SignLayer);
};
 
class SecondRevgift : public PopupLayer{
public:
	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(SecondRevgift);
};

static const int signXY[][2] = {

	{139,216},	//139
	{277,216},
	{413,216},
	{550,216},
	{214,105},
	{347,105},
	{486,105}
};