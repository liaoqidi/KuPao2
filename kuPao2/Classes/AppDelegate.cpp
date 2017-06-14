#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "MainScene.h"
#include "MainBoard.h"
#include "SignScene.h"
#include "overScene.h"
#include "SDKManager.h"
#include "GamePay.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
//������Ϸ������Ϣ
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SDKManager::getinstance()->init();
#endif
	remove(CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str());
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	if(!pSava->getBoolForKey("isFirstPlay",false)){

		pSava->setBoolForKey("isFirstPlay",true);
		
		//�Ƿ񼤻�Ħ�б��
		pSava->setBoolForKey("isCar",true);
		//�Ƿ񼤻��Ь���
		pSava->setBoolForKey("isShoes",false);
		//�Ƿ񼤻Ŀ�����
		pSava->setBoolForKey("isGlass",false);
		//�Ƿ񼤻��������
		pSava->setBoolForKey("isMoney",false);

		//�Ƿ������ֱ��
		pSava->setBoolForKey("isMusic",true);
		//�Ƿ�����Ч���
		pSava->setBoolForKey("isSound",true);

		//��ǰ�����
		pSava->setIntegerForKey("contentGold",0);
		//��ǰ��Ѿ�����
		pSava->setIntegerForKey("bestLenth",0);
		//��ǰ��ѷ�����
		pSava->setIntegerForKey("bestScore",0);

		//ÿ�������¼֮ǰ������  ���ɸ���
		pSava->setIntegerForKey("preDays",0);

		//��¼�Ƿ���ѡ�˽���ѡ��Ħ�У�������һ����ʾ
		pSava->setBoolForKey("isChooseOnCar",false);

		//��¼�����Ƿ���������
		pSava->setBoolForKey("isGetTask",false);

		//��¼�Ƿ�Ϊ����
		pSava->setBoolForKey("isLord",false);

		pSava->setIntegerForKey("bloodNum",5);

		pSava->setBoolForKey("isShowButton",true);

		pSava->setIntegerForKey("contentLevel",1);

		for(int i = 0;i < 3;i++){

			pSava->setIntegerForKey(CCString::createWithFormat("shopCount%d",i + 1)->getCString(),0);
			pSava->setBoolForKey(CCString::createWithFormat("equBuy%d",i + 1)->getCString(),false);
			pSava->setIntegerForKey(CCString::createWithFormat("score%d",i + 1)->getCString(),0);
		}

		//3�ŵ�ͼ��Կ��
		for(int i = 2;i <= 4;i++){

			pSava->setIntegerForKey(CCString::createWithFormat("boss%dKey",i)->getCString(),0);
		}
	}

	//pSava->setIntegerForKey("bestLenth",300);

	CCEGLView::sharedOpenGLView()->
		setDesignResolutionSize(800, 480, kResolutionExactFit);

	// turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);	
	
    // run
	//pDirector->runWithScene(MainLayer::createMainScene());
    //pDirector->runWithScene(GameScene::create());
	//pDirector->runWithScene(getSceneWithLayer(MainSceneSignLayer::create()));
	if(false){
		
		pDirector->runWithScene(CCTransitionFade::create(0.2,getSceneWithLayer(OverLayer::create())));
	}else{

		SignLayer* pSignLayer = SignLayer::create();
		GamePay::getInstance()->signLayer = pSignLayer;

		if(pSignLayer->showSign()){

			pDirector->runWithScene(CCTransitionFade::create(0.2,getSceneWithLayer(pSignLayer)));
		}else{

			//pDirector->runWithScene(MainLayer::createMainScene());
			pDirector->runWithScene(CCTransitionFade::create(0.2,getSceneWithLayer(MainBoard::create())));
		}
	}

	


	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void playMusic(const char* pName,bool state){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	if(pSava->getBoolForKey("isMusic")){

		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pName,state);
	}
}

void playSound(const char* pName){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	if(pSava->getBoolForKey("isSound")){

		SimpleAudioEngine::sharedEngine()->playEffect(pName);
	}
}

void saveAndAdd(const char* pName, int num){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	int contentNum = pSava->getIntegerForKey(pName);
	pSava->setIntegerForKey(pName, contentNum + num);
}