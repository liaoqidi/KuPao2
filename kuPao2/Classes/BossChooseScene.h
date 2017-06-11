#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;
using namespace cocos2d::gui;
using namespace cocos2d;

class BossChooseLayer;
class MapOpenBoard;

class BossChooseLayer : public UILayer{

	CCSprite* pBg;

	UIButton* pBoss[4];
	UILabelAtlas* pBossNum[3];

	CCSprite* pBossBackground1[4];
	CCSprite* pBossBackground2[4];

	int contentBossIndexX;
	int mapIndex;
	bool isTouching;
	int moveTimes;
public:

	bool init();
	void initCom();
	void hideCom();
	void done();
	void callBack(CCObject* pSender,TouchEventType type);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);	
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void bossChange(float x);
	void update(float delta);
	void checkBossKey();
	void showNode();
	void hideNode();
	CREATE_FUNC(BossChooseLayer);
};

class MapOpenBoard : public CCLayer{

	CCMenu* pMenu;
	CCMenuItemImage* pSure;
	CCMenuItemImage* pCancer;
	BossChooseLayer* pParentNode;
public:
	bool init();
	void call_back(CCObject* pSender);
	void setParentNode(BossChooseLayer*);
	CREATE_FUNC(MapOpenBoard);
};