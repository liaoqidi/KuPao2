#include "BossChooseScene.h"
#include "MainScene.h"
#include "ChooseScene.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#define MOVEX 800
#define HALFMOVEX (MOVEX / 2)
#define BOSSCONTENTX 400

using namespace CocosDenshion;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

float abs(float num){

	if(num < 0)
		return -num;

	return num;
}

#endif

bool BossChooseLayer::init(){

	UILayer::init();

	mapIndex = 0;

	contentBossIndexX = 0;

	pBg = CCSprite::create("signScene/background.jpg");
	pBg->setAnchorPoint(ccp(0,0));
	pBg->setZOrder(-11);
	addChild(pBg);

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/bossChoose.json"));

	initCom();

	isTouching = false;

	scheduleUpdate();

	moveTimes = 0;

	playSound("music/page.ogg");

	return true;
}

void BossChooseLayer::initCom(){
	
	/*pReturn = dynamic_cast<UIButton*>(getWidgetByName("Button_340_Copy1"));
	pReturn->addTouchEventListener(this,toucheventselector(BossChooseLayer::callBack));

	pStart = dynamic_cast<UIButton*>(getWidgetByName("Button_331_Copy1"));
	pStart->addTouchEventListener(this,toucheventselector(BossChooseLayer::callBack));*/

	float sleepTime = 0;

	/*pReturn->setPositionX(pReturn->getPositionX() + 140);
	pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));

	pStart->setPositionX(pStart->getPositionX() + 140);
	pStart->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));*/

	for(int i = 0;i < 4;i++){

		//pBoss[i] = CCSprite::create(CCString::createWithFormat("UI/boss%d.png", i + 1)->getCString());
		
		pBoss[i] = dynamic_cast<UIButton*>
			(getWidgetByName(CCString::createWithFormat("boss%d", i + 1)->getCString()));

		if(i > 0){

			pBossNum[i - 1] = dynamic_cast<UILabelAtlas*>
				(getWidgetByName(CCString::createWithFormat("boss%dNum", i + 1)->getCString()));
		}

		pBoss[i]->setAnchorPoint(ccp(0.5,0.5));
		pBoss[i]->setPositionY(248);
		pBoss[i]->addTouchEventListener(this,toucheventselector(BossChooseLayer::callBack));

		pBossBackground1[i] = CCSprite::create(CCString::createWithFormat("imgs/background%d.jpg", i + 1)->getCString());
		pBossBackground1[i]->setAnchorPoint(ccp(0,0));
		pBossBackground1[i]->setZOrder(-1);
		addChild(pBossBackground1[i]);
	
		pBossBackground2[i] = CCSprite::create(CCString::createWithFormat("imgs/background%d.png", i + 1)->getCString());
		pBossBackground2[i]->setAnchorPoint(ccp(0,0));
		pBossBackground2[i]->setZOrder(-1);
		addChild(pBossBackground2[i]);
	}

	checkBossKey();

	bossChange(-MOVEX / 4);

	for(int i = 0;i < 4;i++){

		/*const CCPoint endPoint = pBoss[i]->getPosition();
		pBoss[i]->setPosition(ccp(i < 2 ? 115 : 659,19));
		pBoss[i]->runAction(CCMoveTo::create(0.15,endPoint));*/
	
	
		/*pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime),CCFadeTo::create(0.15,220),NULL));*/

		pBoss[i]->setOpacity(230);
		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-480))),NULL));		
		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(i % 2 == 0 ? 0 : 0.5),CCRepeat::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(0,-10)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(0,10)),2),NULL),9999),NULL));		
		
		pBossBackground1[i]->setOpacity(0);
		pBossBackground2[i]->setPositionY(-480);
		pBoss[i]->setPositionY(pBoss[i]->getPositionY() + 480);
	}

	pBg->runAction(CCFadeOut::create(0.15));
	pBossBackground1[0]->runAction(CCFadeIn::create(0.15));
	pBossBackground2[0]->runAction(CCMoveTo::create(0.4,ccp(0,0)));
}

void BossChooseLayer::checkBossKey(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	for(int i = 0;i < 3;i++){

		int contentBossKey = pSava->getIntegerForKey(CCString::createWithFormat("boss%dKey",i + 2)->getCString());
		pBossNum[i]->setStringValue(CCString::createWithFormat("%d",contentBossKey)->getCString());
	}
}

void BossChooseLayer::showNode(){

	checkBossKey();

	float sleepTime = 0;

	for(int i = 0;i < 4;i++){

		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-480))),NULL));	
	}
}

void BossChooseLayer::hideNode(){

	playSound("music/page.ogg");

	float sleepTime = 0;

	for(int i = 0;i < 4;i++){

		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(0,480))),NULL));	
	}
}

void BossChooseLayer::hideCom(){

	float sleepTime = 0;

	playSound("music/page.ogg");

	for(int i = 0;i < 4;i++){

		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCScaleTo::create(0.15,3),NULL));
		pBoss[i]->runAction(CCSequence::create(CCDelayTime::create(sleepTime),CCFadeOut::create(0.15),NULL));
	
		pBg->runAction(CCFadeIn::create(0.25));

		if(pBossBackground1[i]->getOpacity() > 100)
			pBossBackground1[i]->runAction(CCFadeOut::create(0.3));
		pBossBackground2[i]->runAction(CCMoveTo::create(0.3,ccp(0,-480)));
	}

	/*pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),
		CCCallFunc::create(this,callfunc_selector(BossChooseLayer::done)),NULL));

	pStart->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),
		CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),NULL));*/
}

void BossChooseLayer::done(){

	CCDirector::sharedDirector()->replaceScene(getSceneWithLayer(ChooseLayer::create()));
}

void BossChooseLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){


	UILayer::ccTouchEnded(pTouch,pEvent);

	CCLog("end %d",moveTimes);

	if(moveTimes > 5){

		for(int i = 0;i < 4;i++){

			pBoss[i]->setFocused(false);
		}
	}

	isTouching = false;

	return;
}

bool BossChooseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	UILayer::ccTouchBegan(pTouch,pEvent);

	isTouching = true;

	moveTimes = 0;

	return true;
}

void BossChooseLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

	UILayer::ccTouchMoved(pTouch,pEvent);

	moveTimes ++;

	CCLog("move %d",moveTimes);

	if(abs(pTouch->getStartLocation().x - pTouch->getLocation().x) > 10){

		for(int i = 0;i < 4;i++){

			pBoss[i]->setFocused(false);
		}
	}

	bossChange(pTouch->getLocation().x - pTouch->getPreviousLocation().x);

	return;
}

void BossChooseLayer::bossChange(float moveX){

	int bossX;

	contentBossIndexX += moveX;

	if(contentBossIndexX < 0){

		contentBossIndexX += MOVEX;
	}

	for(int i = 0;i < 4;i++){

		int tempBossX = contentBossIndexX + i * (MOVEX / 4);

		bool isGone = false;

		if(tempBossX / HALFMOVEX % 2 == 0){

			bossX = HALFMOVEX - tempBossX % HALFMOVEX;
			isGone = true;
		}else{

			bossX = tempBossX % HALFMOVEX;
		}
		
		float test = (HALFMOVEX / 2) / (float)(abs((HALFMOVEX / 2) - bossX) + (HALFMOVEX / 2));

		if(isGone == true){

			test = 1 - test;
		}

		pBoss[i]->setPositionX(bossX + BOSSCONTENTX - HALFMOVEX / 2);
		pBoss[i]->setScale(test < 0.3 ? 0.3 : test);
		pBoss[i]->setZOrder(test * 100);
		/*pBossBackground1[i]->setZOrder(test);*/

		float testFloat = 0.65;

		if(test > testFloat){
		
			pBossBackground1[i]->setOpacity((test - testFloat) / 
				(1 - testFloat) * 255);
			pBossBackground2[i]->setPositionY((test - testFloat) / 
				(1 - testFloat) * 420 - 420);
		}
	}
}

void BossChooseLayer::update(float delta){

	if(isTouching == false){

		if(contentBossIndexX < 0){

			contentBossIndexX += MOVEX;
		}

		float p = contentBossIndexX % (MOVEX / 4);

		if(p <= MOVEX / 8 && p > 3){

			contentBossIndexX -= 6;
			bossChange(0);
		}else if(p > MOVEX / 8 && p < MOVEX / 4 - 3){

			contentBossIndexX += 6;
			bossChange(0);
		}
	}
}

void MapOpenBoard::setParentNode(BossChooseLayer* pNode){

	pParentNode = pNode;
}

bool MapOpenBoard::init(){

	CCLayer::init();

	pParentNode = NULL;

	CCSprite* pReliveBoard = CCSprite::create("UI/mapOpen.png");
	pReliveBoard->setAnchorPoint(ccp(0.5, 0.5));
	pReliveBoard->setPosition(ccp(400, 200));
	addChild(pReliveBoard);

	pMenu = CCMenu::create();
	pMenu->setPosition(ccp(0, 0));
	pReliveBoard->addChild(pMenu);

	pCancer = CCMenuItemImage::create("UI/reliveCancer.png", "UI/reliveCancerLock.png", this, menu_selector(MapOpenBoard::call_back));
	pCancer->setAnchorPoint(ccp(0, 0));
	pCancer->setPosition(ccp(61, 68));
	pMenu->addChild(pCancer);

	pSure = CCMenuItemImage::create("UI/reliveSure.png", "UI/reliveSureLock.png", this, menu_selector(MapOpenBoard::call_back));
	pSure->setAnchorPoint(ccp(0, 0));
	pSure->setPosition(ccp(61 + 185, 68));
	pMenu->addChild(pSure);

	setZOrder(99981);

	setScale(0.3);

	setVisible(false);

	runAction(CCSequence::create(CCDelayTime::create(0.3),CCShow::create(),CCEaseBackOut::create(CCScaleTo::create(0.2, 1)),NULL));

	runAction(CCSequence::create(CCDelayTime::create(0),CCRepeat::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(0,-10)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(0,10)),2),NULL),9999),NULL));		

	return true;
}

void MapOpenBoard::call_back(CCObject* pSender){

	playSound("music/menu.mp3");

	if (pSender == pSure){

		//3ÕÅµØÍ¼µÄÔ¿³×
		for(int i = 2;i <= 4;i++){

			CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("boss%dKey",i)->getCString(),10);
		}	
	}else{


	}


	pParentNode->showNode();

	pMenu->setEnabled(false);
	runAction(CCSequence::create(CCScaleTo::create(0.2, 0.3), CCRemoveSelf::create(), NULL));
}

void BossChooseLayer::callBack(CCObject* pSender,TouchEventType type){

	if(type == TOUCH_EVENT_ENDED){

		//playSound("music/bow.ogg");

		int mapIndex = 0;

		for(int i = 0;i < 4;i++){

			if(pSender == pBoss[i]){

				mapIndex = i + 1;
				break;
			}
		}
	
		if(mapIndex != 1 && CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("boss%dKey",mapIndex)->getCString()) < 10){

			hideNode();

			MapOpenBoard* pNode = MapOpenBoard::create();
			pNode->setParentNode(this);
			addChild(pNode);
		}else{

			hideNode();

			CCUserDefault::sharedUserDefault()->setIntegerForKey("mapIndex",mapIndex);
			
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8,getSceneWithLayer(LoadLayer::create())));
		}
	}
}