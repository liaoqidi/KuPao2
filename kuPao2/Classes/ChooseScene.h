#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

#include "MainScene.h"

using namespace cocos2d::extension;
using namespace cocos2d::gui;
using namespace cocos2d;


class ChooseLayer;
class MotoBoard;

class ChooseLayer : public UILayer{

	UIButton* pBoard1;
	UIButton* pBoard2;
	UIImageView* pBoard1Image;
	UIImageView* pBoard2Image;
	UIButton* pSet;
	UIButton* pStart;
	UIButton* pTask;
	CCScene* pToScene;
	UIImageView* pTan;
public:

	bool init();
	void initCom();
	void hideComToChangeScene();
	void actionDone();
	void chooseBoard(CCObject* pSender);
	void callBack(CCObject* pSender,TouchEventType type);
	CREATE_FUNC(ChooseLayer);
};

class MotoBoard : public MenuBoard{

public:

	bool init();
	void doSure();
	void doCancel();
	CREATE_FUNC(MotoBoard);
};