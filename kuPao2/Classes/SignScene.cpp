#include "SignScene.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "MainBoard.h" 
#include "GameScene.h"
#include "SDKManager.h"
#include "GamePay.h"

bool SignLayer::init(){ 
	playMusic("music/main.mp3");
	if(CCLayer::init() == false)	return false;

	pSava = CCUserDefault::sharedUserDefault();

	CCSprite* pBackground = CCSprite::create("GameUI/background.jpg");
	pBackground->setAnchorPoint(ccp(0,0));
	addChild(pBackground);

	pBackground1 = CCSprite::create("signScene/board.png");
	pBackground1->setAnchorPoint(ccp(0.5,0.5));
	pBackground1->setPosition(ccp(400,300));
	pBackground1->setZOrder(0);
	addChild(pBackground1);

	pBackground2 = CCSprite::create("signScene/board2.png");
	pBackground2->setAnchorPoint(ccp(0.5,0.5));
	pBackground2->setPosition(ccp(400,300));
	pBackground2->setZOrder(2);
	addChild(pBackground2);

	pSign = CCMenuItemImage::create("signScene/sign.png","signScene/signLock.png","signScene/signDisable.png",
		this,menu_selector(SignLayer::sign));
	pSign->setAnchorPoint(ccp(0.5,0.5));
	pSign->setPosition(ccp(326,-65));
	pSign->setScale(1.3);
	
	//增加关闭按钮
	pSignClose = CCMenuItemImage::create("GameUI/menuClose.png", "GameUI/menuCloseLock.png", "GameUI/menuCloseLock.png",
		this, menu_selector(SignLayer::signClose));
	pSignClose->setAnchorPoint(ccp(0.5, 0.5));
	pSignClose->setPosition(ccp(680, 305));
	pSignClose->setScale(1.3);

	pMenu = CCMenu::create(pSignClose,pSign, NULL);
	pMenu->setAnchorPoint(ccp(0,0));
	pMenu->setPosition(ccp(0,0));
	pBackground1->addChild(pMenu);

	pNode = CCNode::create();
	pNode->setAnchorPoint(ccp(0,0));
	pNode->setPosition(ccp(0,0));
	pBackground1->addChild(pNode);

	//showSign();

	return true;
}

bool SignLayer::showSign(){

	pNode->removeAllChildren();

	int preDay = pSava->getIntegerForKey("preDay",0);
	keepDay = pSava->getIntegerForKey("keepDay",0);

	for(int i = 0;i < keepDay;i++){

		CCSprite* pSignSign = CCSprite::create("signScene/signSign.png");
		pSignSign->setPosition(ccp(signXY[i][0] + 40,signXY[i][1] - 42));
		pSignSign->setAnchorPoint(ccp(0.5,0.5));
		pSignSign->setZOrder(1);
		pNode->addChild(pSignSign);
	}

	//get system time
	struct tm *tm; 
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);
#else  
	struct cc_timeval now;  
	CCTime::gettimeofdayCocos2d(&now, NULL);  
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep); 
	//contentDay = tm->tm_yday;
	contentDay = pSava->getIntegerForKey("preDay", 0) + 1;
	 
	//end

	//contentDay = preDay + 1;
	if(contentDay == preDay + 1){

		CCSprite* pContentDay = CCSprite::create("signScene/signContentDay.png");
		pContentDay->setPosition(ccp(signXY[keepDay][0],signXY[keepDay][1]));
		pContentDay->setAnchorPoint(ccp(0.5,0.5));
		pContentDay->setZOrder(1);
		//pNode->addChild(pContentDay);
		pSign->setEnabled(true);
		/*pMainScene->pMainLayer->pPauseMenu->setEnabled(false);
		pMainScene->pMainLayer->setKeypadEnabled(false);*/
		runAction(CCMoveTo::create(0.2,ccp(0,0)));

		return true;
	}else if(contentDay == preDay){

		CCSprite* pContentDay = CCSprite::create("signScene/signContentDay.png");
		pContentDay->setPosition(ccp(signXY[keepDay - 1][0],signXY[keepDay - 1][1]));
		pContentDay->setAnchorPoint(ccp(0.5,0.5));
		pContentDay->setZOrder(1);
		//pNode->addChild(pContentDay);	
		pSign->setEnabled(false);

		return false;
	}else{

		keepDay = 0;

		pNode->removeAllChildren();

		CCSprite* pContentDay = CCSprite::create("signScene/signContentDay.png");
		pContentDay->setPosition(ccp(signXY[keepDay][0],signXY[keepDay][1]));
		pContentDay->setAnchorPoint(ccp(0.5,0.5));
		pContentDay->setZOrder(1);
		//pNode->addChild(pContentDay);

		pSign->setEnabled(true);
		runAction(CCMoveTo::create(0.2,ccp(0,0)));

		return true;
	}
}

void SignLayer::sign(CCObject* pSender){
	playSound("music/menu.mp3");

	addChild(SecondRevgift::create());
	
}

void SignLayer::revGift(){
	CCSprite* pSignSign = CCSprite::create("signScene/signSign.png");
	pSignSign->setPosition(ccp(signXY[keepDay][0] + 40, signXY[keepDay][1] - 40));
	pSignSign->setAnchorPoint(ccp(0.5, 0.5));
	pSignSign->setZOrder(1);
	pNode->addChild(pSignSign);

	pSign->setEnabled(false);
	pSava->setIntegerForKey("preDay", contentDay);
	if (keepDay >= 6){
		pSava->setIntegerForKey("keepDay", 6);
	}
	else{
		pSava->setIntegerForKey("keepDay", (keepDay + 1) % 7);
	}

	int contentJewelNum = pSava->getIntegerForKey("contentGold");
	int contentMoneyNum = pSava->getIntegerForKey("contentMoney");

	switch (keepDay)
	{
	case 1:contentJewelNum += 5000;  contentMoneyNum += 400; break;
	case 2:contentJewelNum += 10000; contentMoneyNum += 500; break;
	case 3:contentJewelNum += 15000; contentMoneyNum += 600; break;
	case 4:contentJewelNum += 20000; contentMoneyNum += 700; break;
	case 5:contentJewelNum += 30000; contentMoneyNum += 800; break;
	case 6:contentJewelNum += 50000; contentMoneyNum += 1000; break;
	case 0:contentJewelNum += 3000;  contentMoneyNum += 300; break;
	}

	pSava->setIntegerForKey("contentGold", contentJewelNum);
	pSava->setIntegerForKey("contentMoney", contentMoneyNum);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, getSceneWithLayer(MainBoard::create())));
}


void SignLayer::signClose(CCObject* pSender)
{
	playSound("music/menu.mp3");
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, getSceneWithLayer(MainBoard::create())));
}

bool SecondRevgift::init(){
	PopupLayer::init();

	return true;
}

void SecondRevgift::doSure(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager aaa;
	aaa.pay(0);
#endif
}