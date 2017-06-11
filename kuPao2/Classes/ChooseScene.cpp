#include "chooseScene.h"
#include "GameScene.h"
#include "BossChooseScene.h"
#include "MainScene.h"
#include "SetScene.h"
#include "TaskScene.h"
#include "AppDelegate.h"

bool ChooseLayer::init(){

	UILayer::init();	

	pToScene = NULL;

	CCSprite* pBg = CCSprite::create("signScene/background.jpg");
	pBg->setAnchorPoint(ccp(0,0));
	pBg->setZOrder(-1);
	addChild(pBg);

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/chooseScene.json"));

	initCom();

	return true;
}

void ChooseLayer::initCom(){

	pBoard1 = dynamic_cast<UIButton*>(getWidgetByName("Button_329"));
	pBoard2 = dynamic_cast<UIButton*>(getWidgetByName("Button_330"));
	pSet = dynamic_cast<UIButton*>(getWidgetByName("Button_332"));
	pStart = dynamic_cast<UIButton*>(getWidgetByName("Button_331"));
	pTask = dynamic_cast<UIButton*>(getWidgetByName("Button_333"));

	pBoard1Image = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_409"));
	pBoard2Image = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_410"));
	pTan = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_550"));

	pBoard1->addTouchEventListener(this,toucheventselector(ChooseLayer::callBack));
	pBoard2->addTouchEventListener(this,toucheventselector(ChooseLayer::callBack));
	pSet->addTouchEventListener(this,toucheventselector(ChooseLayer::callBack));
	pStart->addTouchEventListener(this,toucheventselector(ChooseLayer::callBack));
	pTask->addTouchEventListener(this,toucheventselector(ChooseLayer::callBack));

	float sleepTime = 0;

	pBoard1->setPositionY(pBoard1->getPositionY() + 400);
	pBoard1->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-400))),NULL));

	pBoard2->setPositionY(pBoard2->getPositionY() + 400);
	pBoard2->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-400))),NULL));

	pTask->setPositionX(pTask->getPositionX() + 140);
	pTask->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));

	pSet->setPositionX(pSet->getPositionX() + 140);
	pSet->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));

	pStart->setPositionX(pStart->getPositionX() + 140);
	pStart->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));	

	pStart->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(-10,0)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(10,0)),2),NULL)));

	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isChooseOnCar")){

		pBoard1->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,-20)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,20)),2),NULL)));
		pBoard1Image->setVisible(false);
		pBoard2Image->setVisible(true);
	}else{
		
		pBoard2->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,-20)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,20)),2),NULL)));
		pBoard2Image->setVisible(false);
		pBoard1Image->setVisible(true);
	}

	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isGetTask") == false)
		pTan->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1),CCJumpBy::create(0.3,ccp(0,0),20,2),NULL)));
	else
		pTan->setVisible(false);
}

void ChooseLayer::hideComToChangeScene(){

	float sleepTime = 0;

	pBoard1->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(0,400))),NULL));

	pBoard2->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(0,400))),NULL));
	
	pTask->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),NULL));
	
	pSet->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),NULL));
	
	pStart->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),
		CCCallFunc::create(this,callfunc_selector(ChooseLayer::actionDone)),NULL));
}

void ChooseLayer::actionDone(){

	CCDirector::sharedDirector()->replaceScene(pToScene);
	pToScene->release();
}

void ChooseLayer::chooseBoard(CCObject* pSender){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	pBoard1->stopAllActions();
	pBoard2->stopAllActions();
	pBoard1->setPositionY(290);
	pBoard2->setPositionY(290);
	if(pSender == pBoard1){
		
		//choose one
		pBoard1->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,-20)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,20)),2),NULL)));
		pBoard1Image->setVisible(false);
		pBoard2Image->setVisible(true);

		pSava->setBoolForKey("isChooseOnCar",false);
	}else if(pSender == pBoard2){

		pBoard2->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,-20)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,20)),2),NULL)));
		pBoard1Image->setVisible(true);
		pBoard2Image->setVisible(false);
	
		pSava->setBoolForKey("isChooseOnCar",true);
	}
}

void ChooseLayer::callBack(CCObject* pSender,TouchEventType type){

	if(type != TOUCH_EVENT_ENDED)	return;

	if(MenuBoard::shareContentMenu() != NULL)
		return;

	if(pToScene != NULL) return;

	if(pSender == pBoard1){

		chooseBoard(pSender);
	}else if(pSender == pBoard2){

		if(CCUserDefault::sharedUserDefault()->getBoolForKey("isCar") == false)
			addChild(MotoBoard::create());
		else
			chooseBoard(pSender);
	}else if(pSender == pSet){

		playSound("music/page.ogg");
		pToScene = getSceneWithLayer(SetLayer::create());
		pToScene->retain();
		hideComToChangeScene();
	}else if(pSender == pTask){

		playSound("music/page.ogg");
		pToScene = getSceneWithLayer(TaskLayer::create());
		pToScene->retain();
		hideComToChangeScene();
	}else if(pSender == pStart){

		playSound("music/bow.ogg");
		pToScene = getSceneWithLayer(BossChooseLayer::create());
		pToScene->retain();
		hideComToChangeScene();
	}
}

bool MotoBoard::init(){

	MenuBoard::init();

	addBackgroundSprite("UI/motoBoard.png");

	return true;
}

void MotoBoard::doSure(){

	CCUserDefault::sharedUserDefault()->setBoolForKey("isCar",true);
}

void MotoBoard::doCancel(){


}