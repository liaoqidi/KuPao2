#include "SetScene.h"
#include "MainScene.h"
#include "ChooseScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#define ONX 32
#define OFFX 53

using namespace CocosDenshion;

bool SetLayer::init(){

	UILayer::init();

	CCSprite* pBg = CCSprite::create("signScene/background.jpg");
	pBg->setAnchorPoint(ccp(0,0));
	pBg->setZOrder(-1);
	addChild(pBg);

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/setScene.json"));

	initCom();

	return true;
}

void SetLayer::initCom(){

	pBoard = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_338"));

	pSoundState = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_446"));
	pSoundPoint = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_452"));
	pMusicState = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_447"));
	pMusicPoint = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_452_Copy0"));

	pReturn = dynamic_cast<UIButton*>(getWidgetByName("Button_340_Copy0"));
	pReturn->addTouchEventListener(this,toucheventselector(SetLayer::callBack));

	pSound = dynamic_cast<UIButton*>(getWidgetByName("Button_450"));
	pSound->addTouchEventListener(this,toucheventselector(SetLayer::callBack));

	pMusic = dynamic_cast<UIButton*>(getWidgetByName("Button_450_Copy0"));
	pMusic->addTouchEventListener(this,toucheventselector(SetLayer::callBack));

	float sleepTime = 0;

	pBoard->setPositionY(pBoard->getPositionY() - 400);
	pBoard->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,400))),NULL));

	pReturn->setPositionX(pReturn->getPositionX() + 140);
	pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(-140,0))),NULL));

	checkState();
}

void SetLayer::hideCom(){

	float sleepTime = 0;

	playSound("music/page.ogg");

	pBoard->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(0,-400))),NULL));

	pReturn->runAction(CCSequence::create(CCDelayTime::create(sleepTime += 0.05),CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(140,0))),
		CCCallFunc::create(this,callfunc_selector(SetLayer::done)),NULL));
}

void SetLayer::done(){

	CCDirector::sharedDirector()->replaceScene(getSceneWithLayer(ChooseLayer::create()));
}

void SetLayer::checkState(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	bool soundState = pSava->getBoolForKey("isSound");
	bool musicState = pSava->getBoolForKey("isMusic");

	char* onImg = "UI/on.png";
	char* offImg = "UI/off.png";
	char* soundStateFlie = soundState ? onImg : offImg;
	char* musicStateFlie = musicState ? onImg : offImg;

	pSoundState->loadTexture(soundStateFlie);
	pMusicState->loadTexture(musicStateFlie);

	pSoundPoint->stopAllActions();
	pMusicPoint->stopAllActions();

	pSoundPoint->runAction(CCMoveTo::create(0.15,ccp(soundState ? ONX : OFFX,112)));
	pMusicPoint->runAction(CCMoveTo::create(0.15,ccp(musicState ? ONX : OFFX,62)));
}

void SetLayer::callBack(CCObject* pSender,TouchEventType type){

	if(type == TOUCH_EVENT_ENDED){

		if(pSender == pReturn){

			hideCom();
		}
	}else if(type == TOUCH_EVENT_BEGAN){

		if(pSender == pReturn)	return;

		CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

		bool soundState = pSava->getBoolForKey("isSound");
		bool musicState = pSava->getBoolForKey("isMusic");

		if(pSender == pSound){

			pSava->setBoolForKey("isSound",!soundState);		
		}else if(pSender == pMusic){

			pSava->setBoolForKey("isMusic",!musicState);

			if(musicState == true){

				SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			}else{

				playMusic("music/background1.ogg");
			}
		}

		checkState();
	}
}