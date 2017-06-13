#include "MainScene.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "ChooseScene.h"

static MenuBoard* menuInstace = NULL;

CCScene* getSceneWithLayer(CCLayer* pLayer){

	CCScene* pScene = CCScene::create();
	pScene->addChild(pLayer);

	return pScene;
}

bool MainLayer::init(){

	CCLayer::init();

	//playMusic("music/background1.ogg");

	CCSprite* pBackground = CCSprite::create("signScene/background.jpg");
	pBackground->setAnchorPoint(ccp(0,0));
	addChild(pBackground);

	CCSprite* pLufei1 = CCSprite::create("mainScene/lufei1.png");
	pLufei1->setAnchorPoint(ccp(0.5,0));
	pLufei1->setPosition(ccp(400,0));
	addChild(pLufei1);

	CCSprite* pLufei2 = CCSprite::create("mainScene/lufei2.png");
	pLufei2->setAnchorPoint(ccp(0.5,0));
	pLufei2->setPosition(ccp(400,0));
	addChild(pLufei2);

	CCSprite* pTitle = CCSprite::create("mainScene/title.png");
	pTitle->setAnchorPoint(ccp(0.5,0.5));
	pTitle->setPosition(ccp(400,50));
	addChild(pTitle);

	pLufei2->runAction(CCSequence::create(CCDelayTime::create(1),CCFadeOut::create(1),CCRemoveSelf::create(),NULL));
	pLufei1->runAction(CCSequence::create(CCDelayTime::create(2.2),CCSpawn::create(CCMoveBy::create(0.3,ccp(200,0)),NULL),NULL));
	pTitle->runAction(CCSequence::create(CCFadeOut::create(0),CCDelayTime::create(2.2),CCFadeIn::create(0.3),NULL));
	pTitle->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(0,-10)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(0,10)),2),NULL)));
	//pBackground->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(0,-10)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(0,10)),2),NULL)));

	setTouchEnabled(true);

	return true;
}

void MainLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){

	playSound("music/bow.ogg");
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.2,getSceneWithLayer(LoadLayer::create())));
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.2,getSceneWithLayer(ChooseLayer::create())));
}

CCScene* MainLayer::createMainScene(){

	CCScene* pScene = CCScene::create();
	pScene->addChild(MainLayer::create());

	return pScene;
}

bool LoadLayer::init(){

	CCLayer::init();

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("run/man10.png","run/man10.plist","run/man1.ExportJson");
	pBody = CCArmature::create("man1");
	pBody->getAnimation()->playByIndex(0);
	pBody->setScale(0.6);	
	pBody->setPosition(ccp(700,50));
	addChild(pBody,2);

	CCSprite* pTips = CCSprite::create(CCString::createWithFormat("mainScene/tips%d.png",rand () % 2 + 1)->getCString());
	pTips->setAnchorPoint(ccp(0,0));
	pTips->setPositionY(20);
	addChild(pTips);

	runAction(CCSequence::create(CCDelayTime::create(3),CCCallFunc::create(this,callfunc_selector(LoadLayer::callBack)),NULL));

	return true;
}

void LoadLayer::callBack(){

	playMusic("music/game.mp3");
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,GameScene::create()));
}

void MenuBoard::addBackgroundSprite(char* pBackgroundName){

	pReliveBoard = CCSprite::create(pBackgroundName);
	pReliveBoard->setAnchorPoint(ccp(0.5, 0.5));
	pReliveBoard->setPosition(ccp(400, 290));
	pReliveBoard->setZOrder(99);
	addChild(pReliveBoard);

	pReliveBoard->addChild(pMenu);
}


bool MenuBoard::init(){

	CCLayer::init();

	pMenu = CCMenu::create();
	pMenu->setZOrder(1);
	pMenu->setPosition(ccp(0, 0));

	pCancer = CCMenuItemImage::create("GameUI/menuClose.png", "GameUI/menuCloseLock.png", this, menu_selector(MenuBoard::call_back));
	pCancer->setAnchorPoint(ccp(0.5, 0.5));
	pCancer->setPosition(ccp(523, 336));
	pMenu->addChild(pCancer);

	pSure = CCMenuItemImage::create("GameUI/menuSure.png", "GameUI/menuSureLock.png", this, menu_selector(MenuBoard::call_back));
	pSure->setAnchorPoint(ccp(0.5, 0.5));
	pSure->setPosition(ccp(260,-58));
	pMenu->addChild(pSure);

	setZOrder(99981);

	setScale(0.3);

	setVisible(false);

	menuInstace = this;
	runAction(CCSequence::create(CCDelayTime::create(0),CCShow::create(),CCEaseBackOut::create(CCScaleTo::create(0.2, 1)),NULL));

	pLight = CCSprite::create("GameUI/canSee.png");
	pLight->setAnchorPoint(ccp(0.5,0.5));
	pLight->setScale(10);
	addChild(pLight);

	return true;
}

void MenuBoard::onExit(){

	CCLayer::onExit();

	menuInstace = NULL;
}

void MenuBoard::call_back(CCObject* pSender){

	playSound("music/menu.mp3");

	if (pSender == pSure){

		doSure();
	}else{
		runAction(CCSequence::create(CCEaseBackIn::create(CCScaleTo::create(0.1, 0.7)), CCRemoveSelf::create(), NULL));
		doCancel();
	}
	//pMenu->setEnabled(false);

	//runAction(CCSequence::create(CCEaseBackIn::create(CCScaleTo::create(0.1, 0.7)), CCRemoveSelf::create(), NULL));
	//pLight->setVisible(false);
}

MenuBoard* MenuBoard::shareContentMenu(){

	if (menuInstace != NULL)
		menuInstace->runAction(CCRepeat::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.1, ccp(-13, 0)), 2),
		CCEaseInOut::create(CCMoveBy::create(0.1, ccp(13, 0)), 2), NULL), 3));

	return menuInstace;
}

bool MoreGoldBoard::init(){

	MenuBoard::init();

	addBackgroundSprite("UI/goldBoard.png");

	return true;
}

void MoreGoldBoard::doSure(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int contentGold = pSava->getIntegerForKey("contentGold");

	pSava->setIntegerForKey("contentGold",contentGold + 2000);
}