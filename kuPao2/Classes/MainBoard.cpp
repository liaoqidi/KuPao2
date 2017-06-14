#include "MainBoard.h"
#include "MainScene.h"
#include "BossChooseScene.h"
#include "BloodTool.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "TaskScene.h"
#include "GamePay.h"
#include "SDKManager.h"

using namespace CocosDenshion;

#define ONX -20
#define OFFX 20

bool MainBoard::init(){

	UILayer::init();
	if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		playMusic("music/main.mp3");
	}
	

	pLightTask = CCArray::create();
	pLightTask->retain();

	isHideMainBoard = false;
	pShowNode = NULL;

	initCom();

	check(0);

	scheduleUpdate();
	schedule(schedule_selector(MainBoard::addLightToArray),0.5);
	//schedule(schedule_selector(MainBoard::addStartLight),2);
	schedule(schedule_selector(MainBoard::addSnowLight),0.2);
	
	return true;
}

void MainBoard::onExit(){

	UILayer::onExit();
	pLightTask->release();
}

void MainBoard::unHideMainBoard(){

	if(!isHideMainBoard)	return;

	isHideMainBoard = false;

	pMainBoard->runAction(CCMoveBy::create(0.15,ccp(800,0)));
	pShowNode->runAction(CCMoveBy::create(0.15,ccp(0,-480)));	
	
	pReturn->runAction(CCMoveBy::create(0.15,ccp(300,0)));

	pShowNode = NULL;
}

void MainBoard::hideMainBoard(CCNode* pShowNode){

	if(isHideMainBoard)	return;

	isHideMainBoard = true;

	this->pShowNode = pShowNode;
	pMainBoard->runAction(CCMoveBy::create(0.15,ccp(-800,0)));
	pShowNode->runAction(CCMoveBy::create(0.15,ccp(0,480)));	

	pReturn->runAction(CCMoveBy::create(0.15,ccp(-300,0)));
}

void MainBoard::addSnowLight(float dt){

	UIImageView* pSnow = UIImageView::create();
	pSnow->loadTexture("GameUI/snow.png");
	pSnow->setPosition(ccp(rand() % 800,rand() % 480));
	pSnow->setAnchorPoint(ccp(0.5,0.5));
	pSnow->setOpacity(0);
	pBackground->addChild(pSnow);

	pSnow->runAction(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL));
	pSnow->runAction(CCSequence::create(CCMoveBy::create(1,ccp(-100,-100)),CCRemoveSelf::create(),NULL));
	pSnow->runAction(CCRotateBy::create(1,360));
}

void MainBoard::addStartLight(float dt){

	for(int i = 0;i < 2;i++){

		UIImageView* pLight = UIImageView::create();
		pLight->loadTexture(i == 0 ? "GameUI/startLight2.png" : "GameUI/startLight.png");
		pLight->setAnchorPoint(ccp(0.5,0.5));
		pLight->setPosition(ccp(675,70));

		addChild(pLight);

		pLight->runAction(CCSequence::create(CCDelayTime::create(i * 0.05),
			CCEaseOut::create(CCScaleTo::create(0.5,1.3),5),CCScaleTo::create(0.3,1.7),CCRemoveSelf::create(),NULL));
		pLight->runAction(CCSequence::create(CCDelayTime::create(i * 0.05),
			CCEaseOut::create(CCFadeOut::create(0.7),3),NULL));
	}
}

void MainBoard::addLightToArray(float dt){

	//for(int i = 0;i < pLightTask->count();i++){
	{

		int i = rand() % pLightTask->count();

		CCNode* pNode = (CCNode*)pLightTask->objectAtIndex(i);

		UIImageView* pLight = UIImageView::create();
		pLight->loadTexture("imgs/light.png");
		pLight->setAnchorPoint(ccp(1,1));
		pLight->setPosition(ccp(rand() % (int)pNode->getContentSize().width,rand() % (int)pNode->getContentSize().height));
		
		pLight->setOpacity(0);

		pLight->runAction(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),CCRemoveSelf::create(),NULL));

		pNode->addChild(pLight);
	}
}

void MainBoard::initCom(){

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("GameUI/main.json"));

	pStart = dynamic_cast<UIButton*>(getWidgetByName("Button_2"));
	pStart->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pStartMode2 = dynamic_cast<UIButton*>(getWidgetByName("Button_3"));
	pStartMode2->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pReturn = dynamic_cast<UIButton*>(getWidgetByName("Button_52"));
	pReturn->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pLordSure = dynamic_cast<UIButton*>(getWidgetByName("Button_55"));
	pLordSure->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pBloodBoard = dynamic_cast<UIButton*>(getWidgetByName("Button_5"));
	pBloodBoard->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pGoldBoard = dynamic_cast<UIButton*>(getWidgetByName("Button_4"));
	pGoldBoard->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pMoneyBoard = dynamic_cast<UIButton*>(getWidgetByName("Button_6"));
	pMoneyBoard->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pUpdate = dynamic_cast<UIButton*>(getWidgetByName("Button_34"));
	pUpdate->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

	pGet = dynamic_cast<UIButton*>(getWidgetByName("Button_19"));
	pGet->addTouchEventListener(this,toucheventselector(MainBoard::callBack));	

	pMin = dynamic_cast<UILabelAtlas*>(getWidgetByName("min"));
	pSec = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_7_0_1"));
	pGoldNum = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_7_0"));
	pMoneyNum = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_7"));
	pPoint = dynamic_cast<UIImageView*>(getWidgetByName("Image_14"));
	pMax = dynamic_cast<UIImageView*>(getWidgetByName("Image_23"));	
	pBackground = dynamic_cast<UIImageView*>(getWidgetByName("Image_1"));	
	pMainBoard = dynamic_cast<UIImageView*>(getWidgetByName("Image_22"));	
	pLordBoard = dynamic_cast<UIImageView*>(getWidgetByName("Image_46"));	
	pSetBoard = dynamic_cast<UIImageView*>(getWidgetByName("Image_8"));	
	pUpdateBoard = dynamic_cast<UIImageView*>(getWidgetByName("Update"));
	pTaskBoard = dynamic_cast<UIImageView*>(getWidgetByName("taskBoard"));
	pContentTask = dynamic_cast<UIImageView*>(getWidgetByName("contentTask"));

	pContentLevel = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_18"));
	pAddGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_23"));
	pAddScore = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_23_0"));
	pNextAddGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_23_0_1"));
	pNextAddScore = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_23_0_1_2"));
	pNeedGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("AtlasLabel_8"));
	pTaskPro = dynamic_cast<UILoadingBar*>(getWidgetByName("ProgressBar_17")); 

	for(int i = 0;i < 3;i++){

		pCheck[i] = dynamic_cast<UIButton*>(getWidgetByName(CCString::createWithFormat("Button_%d",36 + i)->getCString()));
		pCheck[i]->addTouchEventListener(this,toucheventselector(MainBoard::callBack));
		
		pShow[i] = dynamic_cast<UIScrollView*>(getWidgetByName(CCString::createWithFormat("ScrollView_13_%d",i)->getCString()));
		pShow[i]->setVisible(true);			

		pSet[i] = dynamic_cast<UIButton*>(getWidgetByName(CCString::createWithFormat("set%d",i + 1)->getCString()));
		pSet[i]->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

		pShopBuy[i] = dynamic_cast<UIButton*>(getWidgetByName(CCString::createWithFormat("shopBuy%d",i + 1)->getCString()));
		pShopBuy[i]->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

		pEquBuy[i] = dynamic_cast<UIButton*>(getWidgetByName(CCString::createWithFormat("equBuy%d",i + 1)->getCString()));
		pEquBuy[i]->addTouchEventListener(this,toucheventselector(MainBoard::callBack));

		pEquCheck[i] = dynamic_cast<UIImageView*>(getWidgetByName(CCString::createWithFormat("equCheck%d",i + 1)->getCString()));

		pScore[i] = dynamic_cast<UILabelAtlas*>(getWidgetByName(CCString::createWithFormat("score%d",i + 1)->getCString()));
		pShopPrice[i] = dynamic_cast<UILabelAtlas*>(getWidgetByName(CCString::createWithFormat("shopPrice%d",i + 1)->getCString()));
		pShopCount[i] = dynamic_cast<UILabelAtlas*>(getWidgetByName(CCString::createWithFormat("shopCount%d",i + 1)->getCString()));
		pEquPrice[i] = dynamic_cast<UILabelAtlas*>(getWidgetByName(CCString::createWithFormat("equPrice%d",i + 1)->getCString()));

		char* tempName[] = {"ProgressBar_37","ProgressBar_37_0","ProgressBar_37_1"};
		for(int i = 0;i < 3;i++){

			pBar[i] = dynamic_cast<UILoadingBar*>(getWidgetByName(tempName[i]));
		}

		pSetMenu[i] = dynamic_cast<UIImageView*>(pSet[i]->getChildByTag(11));
	}

	for(int i = 0;i < 5;i++){

		pBlood[i] = dynamic_cast<UIImageView*>(getWidgetByName(CCString::createWithFormat("Image_15_%d",i)->getCString()));
		pMenu[i] = dynamic_cast<UIButton*>(getWidgetByName(CCString::createWithFormat("Button_%d",i + 67)->getCString()));
		pMenu[i]->addTouchEventListener(this,toucheventselector(MainBoard::callBack));	
		pLightTask->addObject(pBlood[i]);
	}

	pLightTask->addObject(pGoldBoard);
	pLightTask->addObject(pMoneyBoard);
	pLightTask->addObject(pStart);
	pLightTask->addObject(pStartMode2);
	
	checkCom();
}

void MainBoard::callBack(CCObject* pSender,TouchEventType type){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	
	bool soundState = pSava->getBoolForKey("isSound");
	bool musicState = pSava->getBoolForKey("isMusic");
	bool showState = pSava->getBoolForKey("isShowButton");
	int contentGold = pSava->getIntegerForKey("contentGold");
	int contentMoney = pSava->getIntegerForKey("contentMoney");

	if(type == TOUCH_EVENT_ENDED){

		playSound("music/menu.mp3");

		if(MenuBoard::shareContentMenu() != NULL){

			return;
		}

		if(pSender == pStart){

			CCUserDefault::sharedUserDefault()->setBoolForKey("isGameMode1",true);

			if(BloodTool::deleteBlood()){

				addStartShow();
				//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,getSceneWithLayer(BossChooseLayer::create())));			
			}else{

				addChild(MoreBlood::create());
			}
		}else if(pSender == pStartMode2){

			CCUserDefault::sharedUserDefault()->setBoolForKey("isGameMode1",false);

			if(BloodTool::deleteBlood()){

				addStartShow();
				//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,getSceneWithLayer(BossChooseLayer::create())));			
			}else{

				addChild(MoreBlood::create());
			}
		}else if(pSender == pBloodBoard){

			addChild(MoreBlood::create());
		}else if(pSender == pGoldBoard){

			addChild(MoreGold::create());
		}else if(pSender == pMoneyBoard){

			addChild(MoreMoney::create());
		}else if(pSender == pCheck[0]){

			check(0);
		}else if(pSender == pCheck[1]){

			check(1);
		}else if(pSender == pCheck[2]){

			check(2);
		}else if(pSender == pMenu[4]){			
						
			hideMainBoard(pLordBoard);
		}else if(pSender == pMenu[1]){

			hideMainBoard(pSetBoard);
		}else if(pSender == pReturn){

			unHideMainBoard();
		}else if(pSender == pLordSure){
			if (pLordSure->isBright() == false)	return;
			//增加二次弹窗
			if (GamePay::getInstance()->getIsSecond())
			{
				addChild(SecondLordSure::create());
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				SDKManager::getinstance()->pay(PAY_VIP);
#endif
			}
		}else if(pSender == pSet[0]){

			pSava->setBoolForKey("isSound",!soundState);

			checkCom();
		}else if(pSender == pSet[1]){

			pSava->setBoolForKey("isMusic",!musicState);

			if(musicState == true){

				SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			}else{

				playMusic("music/main.mp3");
			}

			checkCom();
		}else if(pSender == pSet[2]){

			pSava->setBoolForKey("isShowButton",!showState);			

			checkCom();
		}else if(pSender == pMenu[3]){
	
			hideMainBoard(pTaskBoard);
		}else if(pSender == pMenu[0]){

			CCDirector::sharedDirector()->end();
		}else if(pSender == pMenu[2]){

			hideMainBoard(pUpdateBoard);
		}else if(pSender == pUpdate){

			if(contentGold < needGold){

				addChild(MoreGold::create());
			}else{

				saveAndAdd("contentGold",-needGold);
				saveAndAdd("contentLevel",1);
				checkCom();

				CCSprite* pSprite = CCSprite::create("GameUI/UpdateTitle.png");
				pSprite->setAnchorPoint(ccp(0.5,0));
				pSprite->setPosition(ccp(105,50));
				pSprite->runAction(CCSequence::create(CCJumpBy::create(0.5,ccp(0,0),30,3),CCDelayTime::create(0.2),CCRemoveSelf::create(),NULL));
				addChild(pSprite);
			}
		}else if(pSender == pGet){

			if(!pGet->isBright())	return;

			pSava->setIntegerForKey("contentMoney",
				pSava->getIntegerForKey("contentMoney") + 20);
			pSava->setBoolForKey("isGetTask",true);

			pGet->setBright(false);
			pGet->loadTextureDisabled("GameUI/taskDis.png");
		}else if(pSender == pShopBuy[0]){
			
			if(contentGold < myShopPrice[0]){

				addChild(MoreGold::create());
			}else{

				saveAndAdd("contentGold",-myShopPrice[0]);
				saveAndAdd("shopCount1",1);
				checkCom();
			}			
		}else if(pSender == pShopBuy[1]){

			if(contentGold < myShopPrice[1]){

				addChild(MoreGold::create());
			}else{

				saveAndAdd("contentGold",-myShopPrice[1]);
				saveAndAdd("shopCount2",1);
				checkCom();
			}		
		}else if(pSender == pShopBuy[2]){

			if(contentGold < myShopPrice[2]){

				addChild(MoreGold::create());
			}else{

				saveAndAdd("contentGold",-myShopPrice[2]);
				saveAndAdd("shopCount3",1);
				checkCom();
			}		
		}else if(pSender == pEquBuy[0]){

			if(pEquBuy[0]->isBright() == false)	return;

			if(contentMoney < myEquPrice[0]){

				addChild(MoreMoney::create());
			}else{

				saveAndAdd("contentMoney",-myEquPrice[0]);
				pSava->setBoolForKey("equBuy1",true);
				checkCom();
			}
		}else if(pSender == pEquBuy[1]){

			if(pEquBuy[1]->isBright() == false)	return;

			if(contentMoney < myEquPrice[1]){

				addChild(MoreMoney::create());
			}else{

				saveAndAdd("contentMoney",-myEquPrice[1]);
				pSava->setBoolForKey("equBuy2",true);
				checkCom();
			}
		}else if(pSender == pEquBuy[2]){

			if(pEquBuy[2]->isBright() == false)	return;

			if(contentMoney < myEquPrice[2]){

				addChild(MoreMoney::create());
			}else{

				saveAndAdd("contentMoney",-myEquPrice[2]);
				pSava->setBoolForKey("equBuy3",true);
				checkCom();
			}
		}
	}
}

void MainBoard::checkCom(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	
	//lordBoard
	bool isLord = pSava->getBoolForKey("isLord");
	if(isLord){

		pLordSure->setBright(false);
	}

	//Set com
	char* pFlieNum[3];
	bool tag[3];

	tag[0] = pSava->getBoolForKey("isSound");
	tag[1] = pSava->getBoolForKey("isMusic");
	tag[2] = pSava->getBoolForKey("isShowButton");

	char* onImg = "GameUI/SetOn.png";
	char* offImg = "GameUI/SetOff.png";
	
	pFlieNum[0] = tag[0] ? onImg : offImg;
	pFlieNum[1] = tag[1] ? onImg : offImg;
	pFlieNum[2] = tag[2] ? onImg : offImg;


	for(int i = 0;i < 3;i++){

		pSet[i]->loadTextureNormal(pFlieNum[i]);
		pSet[i]->loadTexturePressed(pFlieNum[i]);
	
		pSetMenu[i]->stopAllActions();
		pSetMenu[i]->runAction(CCMoveTo::create(0.15,ccp(tag[i] ? ONX : OFFX,pSetMenu[i]->getPositionY())));
	}

	//UpdateCom
	int contentLevel = pSava->getIntegerForKey("contentLevel");
	needGold = (contentLevel + 1) * 100;
	pContentLevel->setStringValue(CCString::createWithFormat("%d",contentLevel)->getCString());
	pAddGold->setStringValue(CCString::createWithFormat("%d",contentLevel)->getCString());
	pAddScore->setStringValue(CCString::createWithFormat("%d",contentLevel)->getCString());
	pNextAddGold->setStringValue(CCString::createWithFormat("%d",contentLevel + 1)->getCString());
	pNextAddScore->setStringValue(CCString::createWithFormat("%d",contentLevel + 1)->getCString());
	pNeedGold->setStringValue(CCString::createWithFormat("%d",needGold)->getCString());
	for(int i = 0;i < 3;i++){

		pBar[i]->setPercent(contentLevel + 1);
	}

	//taskCom
	GameScene::checkEveryDayTaskData();

	int taskType = pSava->getIntegerForKey("taskType");
	int taskNum = pSava->getIntegerForKey(CCString::createWithFormat
		("task%d",taskType)->getCString());

	bool isGet = pSava->getBoolForKey("isGetTask");;

	if(taskNum > taskMax[taskType])
		taskNum = taskMax[taskType];

	//移动任务标签位置
	pContentTask->setPositionX(pContentTask->getPositionX()-80);
	pContentTask->loadTexture(CCString::createWithFormat("GameUI/task%d.png",
		taskType)->getCString());	
	
	if(taskNum < taskMax[taskType]){

		pGet->setBright(false);		
		int percent = taskNum / taskMax[taskType] * 100;
		pTaskPro->setPercent(percent);
	}else if(isGet){

		pGet->setBright(false);
		pGet->loadTextureDisabled("GameUI/taskDis.png");
	}

	//mainBoard com

	for(int i = 0;i < 3;i++){

		myShopPrice[i] = shopPrice[i] * (isLord ? 0.5 : 1);
		myEquPrice[i] = equPrice[i] * (isLord ? 0.7 : 1);

		pShopPrice[i]->setStringValue(CCString::createWithFormat("%d",myShopPrice[i])->getCString());
		pEquPrice[i]->setStringValue(CCString::createWithFormat("%d",myEquPrice[i])->getCString());
	
		int shopCount = pSava->getIntegerForKey(CCString::createWithFormat("shopCount%d",i + 1)->getCString());
		if(shopCount != 0){

			pShopCount[i]->getParent()->setVisible(true);
			pShopCount[i]->setStringValue(CCString::createWithFormat("%d",shopCount)->getCString());
		}else{

			pShopCount[i]->getParent()->setVisible(false);
		}

		if(isLord){

			{
				UIImageView* pVip = UIImageView::create();
				pVip->loadTexture("GameUI/vip.png");
				pVip->setAnchorPoint(ccp(0.5,0.5));
				pVip->setPosition(ccp(55,2));
				pVip->setScale(2);				

				pShopPrice[i]->addChild(pVip);
			}

			{
				UIImageView* pVip = UIImageView::create();
				pVip->loadTexture("GameUI/vip.png");
				pVip->setAnchorPoint(ccp(0.5,0.5));
				pVip->setPosition(ccp(55,2));
				pVip->setScale(2);

				pEquPrice[i]->addChild(pVip);
			}
		}

		bool isEquBuy = pSava->getBoolForKey(CCString::createWithFormat("equBuy%d",i + 1)->getCString());
		if(!isEquBuy){

			pEquCheck[i]->setVisible(false);
			pEquBuy[i]->setBright(true);
		}else{

			pEquCheck[i]->setVisible(true);
			pEquBuy[i]->setBright(false);
		}

		int score = pSava->getIntegerForKey(CCString::createWithFormat("score%d",i + 1)->getCString());	
		pScore[i]->setStringValue(CCString::createWithFormat("%d",score)->getCString());
	}
}

void MainBoard::update(float dt){

	BloodData data = BloodTool::getContentBlood();

	for(int i = 0;i < 5;i++){

		pBlood[i]->setVisible(i < data.contentBlood);
	}

	if(data.nextTime.min == -1){
		
		pMin->setVisible(false);
		pSec->setVisible(false);
		pPoint->setVisible(false);
		pMax->setVisible(true);
	}else{

		pMin->setStringValue(CCString::createWithFormat("%d",data.nextTime.min)->getCString());
		pSec->setStringValue(CCString::createWithFormat("%d",data.nextTime.sec)->getCString());
	
		pMin->setVisible(true);
		pSec->setVisible(true);
		pPoint->setVisible(true);
		pMax->setVisible(false);
	}	

	pGoldNum->setStringValue(CCString::createWithFormat("%d",CCUserDefault::sharedUserDefault()->getIntegerForKey("contentGold"))->getCString());
	pMoneyNum->setStringValue(CCString::createWithFormat("%d",CCUserDefault::sharedUserDefault()->getIntegerForKey("contentMoney"))->getCString());
}

void MainBoard::addStartShow(){

	CCSprite* pBloodShow = CCSprite::create("GameUI/blood.png");
	pBloodShow->setContentSize(ccp(36,33));
	pBloodShow->setAnchorPoint(ccp(0.5,0.5));
	pBloodShow->setPosition(ccp(53 + BloodTool::getContentBlood().contentBlood * 39,447));
	
	ccBezierConfig config;
	config.endPosition = CCUserDefault::sharedUserDefault()->getBoolForKey("isGameMode1") ? pStart->getPosition() : pStartMode2->getPosition();
	config.controlPoint_1 = ccp(500,400);
	config.controlPoint_2 = ccp(600,320);

	pBloodShow->runAction(CCSequence::create(CCEaseInOut::create(CCBezierTo::create(1,config),2),CCScaleTo::create(0.1,1.5),
		CCScaleTo::create(0.1,1),CCCallFunc::create(this,callfunc_selector(MainBoard::startCallBack)),CCRemoveSelf::create(),NULL));
	pBloodShow->setZOrder(9999);
	addChild(pBloodShow);
}

void MainBoard::startCallBack(){

	CCUserDefault::sharedUserDefault()->setIntegerForKey("mapIndex",1 + rand() % 2);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,getSceneWithLayer(LoadLayer::create())));
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,getSceneWithLayer(BossChooseLayer::create())));
}

bool MoreBlood::init(){

	MenuBoard::init();

	addBackgroundSprite("GameUI/bloodBuy.png");

	return true;
}

bool MoreGold::init(){

	MenuBoard::init();

	addBackgroundSprite("GameUI/goldBuy.png");

	return true;
}

bool MoreMoney::init(){

	MenuBoard::init();

	addBackgroundSprite("GameUI/moneyBuy.png");

	return true;
}

void MainBoard::check(int index){

	for(int i = 0;i < 3;i++){

		pCheck[i]->setBright(i != index);
		pShow[i]->setScale(i == index ? 1 : 0);
	}
}

void MoreMoney::doSure(){
	if (GamePay::getInstance()->getIsSecond())
	{
		addChild(SecondMoreMoney::create());
	}
	else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SDKManager::getinstance()->pay(PAY_MONEY);
#endif
	}
}

bool SecondMoreMoney::init(){
	PopupLayer::init();
	return true;
}

void SecondMoreMoney::doSure(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager::getinstance()->pay(PAY_MONEY);
#endif
}

void MoreGold::doSure(){
	if (GamePay::getInstance()->getIsSecond())
	{
		addChild(SecondMoreGold::create());
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SDKManager::getinstance()->pay(PAY_GOLD);
#endif
	}
}

bool SecondMoreGold::init(){
	PopupLayer::init();
	return true;
}

void SecondMoreGold::doSure(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager::getinstance()->pay(PAY_GOLD);
#endif
}

void MoreBlood::doSure(){
	if (GamePay::getInstance()->getIsSecond())
	{
		addChild(SecondMoreBlood::create());
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SDKManager::getinstance()->pay(PAY_BLOOD);
#endif
	}
}

bool SecondMoreBlood::init(){
	PopupLayer::init();

	return true;
}

void SecondMoreBlood::doSure(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager::getinstance()->pay(PAY_BLOOD);
#endif
}


bool SecondLordSure::init(){
	PopupLayer::init();

	return true;
}

void SecondLordSure::doSure(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager::getinstance()->pay(PAY_VIP);
#endif
}

void SecondLordSure::doCancel(){
	
}