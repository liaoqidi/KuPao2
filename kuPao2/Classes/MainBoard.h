#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MainScene.h"
#include "GameScene.h"

using namespace cocos2d;
using namespace cocos2d::gui;
using namespace cocos2d::extension;

class MainBoard;
class UpdateBoard;
class MoreBlood;
class MoreGold;
class MoreMoney;

class MainBoard : public UILayer{

	UIButton* pStart;
	UIButton* pStartMode2;
	UIButton* pReturn;

	UIButton* pBloodBoard;
	UIButton* pGoldBoard;
	UIButton* pMoneyBoard;

	UIButton* pCheck[3];
	UIButton* pMenu[5];

	UILabelAtlas* pMin;
	UILabelAtlas* pSec;
	UIImageView* pPoint;
	UIImageView* pMax;
	UIImageView* pBackground;

	UIButton* pLordSure;

	UILabelAtlas* pGoldNum;
	UILabelAtlas* pMoneyNum;

	UIImageView* pBlood[5];
	UIScrollView* pShow[3];

	CCArray* pLightTask;

	bool isHideMainBoard;
	CCNode* pShowNode;

	UIImageView* pMainBoard;
	UIImageView* pLordBoard;
	UIImageView* pSetBoard;
	UIImageView* pUpdateBoard;
	UIImageView* pTaskBoard;

	//setBoard Com
	UIButton* pSet[3];
	UIImageView* pSetMenu[3];

	//UpdateBoard Com
	UILabelAtlas* pContentLevel;
	UILabelAtlas* pAddGold;
	UILabelAtlas* pAddScore;
	UILabelAtlas* pNextAddGold;
	UILabelAtlas* pNextAddScore;
	UILabelAtlas* pNeedGold;
	UILoadingBar* pBar[3];
	UIButton* pUpdate;
	int needGold;

	//taskBoard com
	UIButton* pGet;
	UILoadingBar* pTaskPro;
	UIImageView* pContentTask;

	//mainBoard com
	UILabelAtlas* pShopPrice[3];
	UILabelAtlas* pShopCount[3];
	UILabelAtlas* pEquPrice[3];
	UILabelAtlas* pScore[3];
	UIButton* pShopBuy[3];
	UIButton* pEquBuy[3];
	UIImageView* pEquCheck[3];
	int myShopPrice[3];
	int myEquPrice[3];
public:

	bool init();
	void initCom();
	void callBack(CCObject* pSender,TouchEventType type);
	void update(float dt);

	void addStartShow();

	void startCallBack();

	void check(int index);

	void onExit();

	void addLightToArray(float dt);
	void addStartLight(float dt);
	void addSnowLight(float dt);	
	void checkCom();
	void hideMainBoard(CCNode* pShowNode);
	void unHideMainBoard();

	CREATE_FUNC(MainBoard);
};

class MoreBlood : public MenuBoard{

public:

	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(MoreBlood);
};

class SecondMoreBlood : public PopupLayer{
public:
	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(SecondMoreBlood);
};

class MoreGold : public MenuBoard{

public:

	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(MoreGold);
};

class SecondMoreGold : public  PopupLayer{
public:
	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(SecondMoreGold);
};

class MoreMoney : public MenuBoard{

public:

	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(MoreMoney);
};

class SecondMoreMoney : public  PopupLayer{
public:
	bool init();
	void doSure();
	void doCancel(){};
	CREATE_FUNC(SecondMoreMoney);
};

class SecondLordSure : public  PopupLayer{
public:
	bool init();
	void doSure();
	void doCancel();
	CREATE_FUNC(SecondLordSure);
};

static const int shopPrice[] = {1500,2000,2000};
static const int equPrice[] = {500,500,700};