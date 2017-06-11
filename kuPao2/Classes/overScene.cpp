#include "overScene.h"
#include "MainScene.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "ChooseScene.h"
#include "MainBoard.h"
#include "algorithm.h"

bool com(int num1,int num2){

	return num1 > num2;
}

bool OverLayer::init(){

	CCLayer::init();

	playMusic("music/over.ogg",false);

	CCSprite* pBackground = CCSprite::create("GameUI/background.jpg");
	pBackground->setAnchorPoint(ccp(0,0));
	addChild(pBackground);

	pOverBoard = CCSprite::create("overScene/overBoard.png");
	pOverBoard->setAnchorPoint(ccp(0.5,0.5));
	pOverBoard->setPosition(ccp(400,240 + 480));
	pOverBoard->runAction(CCSequence::create(CCDelayTime::create(0.5),
		CCEaseBackOut::create(CCMoveBy::create(0.5,ccp(0,-480))),CCCallFunc::create(this,callfunc_selector(OverLayer::scheduleUpdate)),NULL));
	addChild(pOverBoard);

	pContentMetle = CCLabelAtlas::create("0","GameUI/num1.png",28,46,'0');
	pContentMetle->setAnchorPoint(ccp(0,0.5));
	pContentMetle->setPosition(ccp(200,195));
	pContentMetle->setScale(0.8);
	pOverBoard->addChild(pContentMetle);

	pContentScore = CCLabelAtlas::create("0","GameUI/num1.png",28,46,'0');
	pContentScore->setAnchorPoint(ccp(0,0.5));
	pContentScore->setPosition(ccp(200,132));
	pContentScore->setScale(0.8);
	pOverBoard->addChild(pContentScore);

	pContentGold = CCLabelAtlas::create("0","GameUI/num1.png",28,46,'0');
	pContentGold->setAnchorPoint(ccp(0.5,0.5));
	pContentGold->setPosition(ccp(238,63));
	pContentGold->setScale(1);
	pOverBoard->addChild(pContentGold);

	pBestMetle = CCLabelAtlas::create("0","GameUI/num2.png",28,46,'0');
	pBestMetle->setAnchorPoint(ccp(0.5,0.5));
	pBestMetle->setPosition(ccp(239,264));
	pBestMetle->setScale(1);
	pOverBoard->addChild(pBestMetle);

	pBestSocre = CCLabelAtlas::create("0","GameUI/num1.png",28,46,'0');
	pBestSocre->setAnchorPoint(ccp(0,0.5));
	pBestSocre->setPosition(ccp(255,310));
	pBestSocre->setScale(0.5);
	pOverBoard->addChild(pBestSocre);

	CCMenu* pMenu = CCMenu::create();
	pMenu->setPosition(ccp(0,0));
	pOverBoard->addChild(pMenu);

	pSure = CCMenuItemImage::create("overScene/sure.png","overScene/sureLock.png",this,menu_selector(OverLayer::callBack));
	pSure->setAnchorPoint(ccp(0.5,0.5));
	pSure->setPosition(ccp(600,35));
	pSure->setScale(0);
	pContentGold->setScale(0);
	pMenu->addChild(pSure);

	giftGOld = 0;
	score1 = 0;
	gold1 = 0;
	isShowGold = false;

	schedule(schedule_selector(OverLayer::addStartLight),1);

	return true;
}

void OverLayer::update(float delta){

	if(score1 < contentScore){

		if(contentScore - score1 < 10){

			score1 ++;
		}else{

			score1 += (contentScore - score1) / 20 + 1;
		}
	}else{

		if(isShowGold == false){

			isShowGold = true;
			pSure->getParent()->getParent()->runAction(CCMoveTo::create(0.2,ccp(278,240)));
			pSure->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.2,1)),NULL));
			pContentGold->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.2,1)),NULL));
			pContentGold->setString(CCString::createWithFormat("%d",giftGOld)->getCString());		
		}
	}

	pBestMetle->setString(CCString::createWithFormat("%d",score1)->getCString());
}

void OverLayer::setNum(int metle,int score){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int bestScore = pSava->getIntegerForKey("bestScore");
	int bestLenth = pSava->getIntegerForKey("bestLenth");

	pContentMetle->setString(CCString::createWithFormat("%d",metle)->getCString());
	pContentScore->setString(CCString::createWithFormat("%d",score)->getCString());
	
	score = metle + score * 10;

	if(score > bestScore){

		bestScore = score;
		pSava->setIntegerForKey("bestScore",bestScore);
	}

	if(metle > bestLenth){

		bestLenth = metle;
		pSava->setIntegerForKey("bestLenth",metle);
	}

	pBestSocre->setString(CCString::createWithFormat("%d",bestScore)->getCString());
	pBestMetle->setString(CCString::createWithFormat("%d",0)->getCString());

	giftGOld = metle / 10 + score / 10;
	contentScore = score;
	pContentGold->setString(CCString::
		createWithFormat("%d",0)->getCString());

	int num[4];
	for(int i = 0;i < 3;i++){

		num[i] = pSava->getIntegerForKey(CCString::createWithFormat("score%d",i + 1)->getCString());		
	}
	num[3] = contentScore;
	sort(num,num + 4,com);

	for(int i = 0;i < 3;i++){

		pSava->setIntegerForKey(CCString::createWithFormat("score%d",i + 1)->getCString(),num[i]);		
	}
}

void OverLayer::callBack(CCObject* pSender){

	playSound("music/menu.mp3");

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	int gold = pSava->getIntegerForKey("contentGold");
	pSava->setIntegerForKey("contentGold",gold + giftGOld);

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.2,getSceneWithLayer(MainBoard::create())));
}

void OverLayer::addStartLight(float dt){

	for(int i = 0;i < 2;i++){

		CCSprite* pLight = CCSprite::create(i == 0 ? "overScene/overLight2.png" : "overScene/overLight.png");		
		pLight->setAnchorPoint(ccp(0.5,0.5));
		pLight->setPosition(ccp(239,370));

		pOverBoard->addChild(pLight);

		pLight->runAction(CCSequence::create(CCDelayTime::create(i * 0.05),
			CCEaseOut::create(CCScaleTo::create(0.5,1.3),5),CCScaleTo::create(0.3,1.7),CCRemoveSelf::create(),NULL));
		pLight->runAction(CCSequence::create(CCDelayTime::create(i * 0.05),
			CCEaseOut::create(CCFadeOut::create(0.7),3),NULL));
	}
}