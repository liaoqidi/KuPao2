#include "TaskScene.h"
#include "MainScene.h"
#include "ChooseScene.h"
#include "GameScene.h"
#include "AppDelegate.h"

bool TaskLayer::init(){

	UILayer::init();

	CCSprite* pBg = CCSprite::create("signScene/background.jpg");
	pBg->setAnchorPoint(ccp(0,0));
	pBg->setZOrder(-1);
	addChild(pBg);

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/taskScene.json"));

	initCom();	

	return true;
}

void TaskLayer::initCom(){

	pTaskNum = dynamic_cast<UILabelAtlas*>(getWidgetByName("LabelAtlas_383"));

	pBoard = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_365"));
	pTaskImg = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_381"));

	pReturn = dynamic_cast<UIButton*>(getWidgetByName("Button_340_Copy1"));
	pReturn->addTouchEventListener(this,toucheventselector(TaskLayer::callBack));

	pGet = dynamic_cast<UIButton*>(getWidgetByName("Button_382"));
	pGet->addTouchEventListener(this,toucheventselector(TaskLayer::callBack));

	pTaskPower = dynamic_cast<UILoadingBar*>(getWidgetByName("LoadingBar_548"));

	float sleepTime = 0;

	pBoard->setPositionX(pBoard->getPositionX() - 300);
	pBoard->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(300,0))),NULL));

	pReturn->setPositionX(pReturn->getPositionX() + 140);
	pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));

	checkTask();
}

void TaskLayer::hideCom(){

	float sleepTime = 0;

	playSound("music/page.ogg");

	pBoard->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(-300,0))),NULL));

	pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),
		CCCallFunc::create(this,callfunc_selector(TaskLayer::done)),NULL));
}

void TaskLayer::done(){

	CCDirector::sharedDirector()->replaceScene(getSceneWithLayer(ChooseLayer::create()));
}

void TaskLayer::callBack(CCObject* pSender,TouchEventType type){

	if(type != TOUCH_EVENT_ENDED)	return;

	if(pSender == pReturn){
		
		hideCom();
	}else{

		if(!pGet->isBright())	return;

		CCUserDefault* pSava= CCUserDefault::sharedUserDefault();
		pSava->setIntegerForKey("contentGold",pSava->getIntegerForKey("contentGold") + 2000);
		pSava->setBoolForKey("isGetTask",true);
		
		pGet->setBright(false);
		pGet->loadTextureDisabled("UI/getDisable2.png");
	}
}

void TaskLayer::checkTask(){

	GameScene::checkEveryDayTaskData();

	CCUserDefault* pSava= CCUserDefault::sharedUserDefault();

	int taskType = pSava->getIntegerForKey("taskType");
	int taskNum = pSava->getIntegerForKey(CCString::createWithFormat
		("task%d",taskType)->getCString());

	bool isGet = pSava->getBoolForKey("isGetTask");;

	if(taskNum > taskMax[taskType])
		taskNum = taskMax[taskType];

	pTaskNum->setStringValue(CCString::createWithFormat("%d",taskNum)->getCString());
	pTaskImg->loadTexture(CCString::createWithFormat("UI/task%d.png",taskType)->getCString());	

	if(taskNum < taskMax[taskType]){

		pGet->setBright(false);		
		int percent = taskNum / taskMax[taskType] * 100;
		pTaskPower->setPercent(percent);
	}else if(isGet){

		pGet->setBright(false);
		pGet->loadTextureDisabled("UI/getDisable2.png");
	}
}