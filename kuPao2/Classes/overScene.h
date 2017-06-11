#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class OverLayer;

class OverLayer : public CCLayer{

	CCMenuItemImage* pSure;

	CCLabelAtlas* pContentMetle;
	CCLabelAtlas* pContentScore;
	CCLabelAtlas* pContentGold;
	CCLabelAtlas* pBestMetle;
	CCLabelAtlas* pBestSocre;
	CCSprite* pOverBoard;
	int giftGOld;
	int contentScore;

	int score1;
	int gold1;

	bool isShowGold;
public:

	bool init();
	void setNum(int metle,int score);
	void callBack(CCObject* pSender);
	void addStartLight(float);
	void update(float delta);
	CREATE_FUNC(OverLayer);
};