#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../extensions/CocoStudio/Armature/CCArmature.h"
using namespace cocos2d;
using namespace cocos2d::extension;

CCScene* getSceneWithLayer(CCLayer* pLayer);

class MainLayer;
class LoadLayer;

class MenuBoard;
class MoreGoldBoard;

class MainLayer : public CCLayer{

public:

	bool init();
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	MenuBoard *pMenuBoard;
	CREATE_FUNC(MainLayer);
	static CCScene* createMainScene();
};

class LoadLayer : public CCLayer{

	CCArmature* pBody;
public:

	bool init();
	void callBack();
	CREATE_FUNC(LoadLayer);
};

class MenuBoard : public CCLayer{

	CCMenu* pMenu;
	CCMenuItemImage* pSure;
	CCMenuItemImage* pCancer;
	CCSprite* pLight;
	CCSprite* pReliveBoard;
public:

	bool init();
	void addBackgroundSprite(char* pBackgroundName);
	void call_back(CCObject* pSender);
	void onExit();
	static MenuBoard* shareContentMenu();
	virtual void doSure() = 0;
	virtual void doCancel() = 0;
};

class MoreGoldBoard : public MenuBoard{

public:

	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(MoreGoldBoard);
};