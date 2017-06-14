#include "GameScene.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "MainBoard.h"
#include "overScene.h"
#include "ChooseScene.h"
#include "SimpleAudioEngine.h"
#include "Box2D/Box2D.h"

using namespace CocosDenshion;

#define RUNINDEX(ISONCAR) (ISONCAR ? 7 : 0)
#define JUMP1INDEX(ISONCAR) (ISONCAR ? 8 : 1)
#define JUMP2INDEX(ISONCAR) (ISONCAR ? 9 : 3)
#define ATTINDEX(ISONCAR) (ISONCAR ? 10 : 2)
#define DOWNINDEX 4
#define FLYINDEX(ISONCAR) (ISONCAR ? 11 : 5)
#define DIEINDEX 1

#define MONEYTIME 5

#define MAPWIDTH 1600

#define SPRITEWIDTH 70
#define SPRITEHEIGHT 70

#define PLAYERWIDTH (55 * (Player::sharePlayer()->isInBig() ? 1.7 : 1))
#define PLAYERHEIGHT (95 * (Player::sharePlayer()->isInBig() ? 1.7 : 1))
#define PLAYERSCALE 0.7
#define PLAYERSTARTX 250.0
#define beginX 510

#define MAPSPEED 7 * mapMoveTime * (baseMapMoveTime * 0.05 + 1)

#define BACKGROUNDSPEED 1

#define TMXTAG 1024
#define GOLDTMXTAG 1025

#define G 1
#define JUMP1POWER -18
#define JUMP2POWER -15
#define UPJUMPPOWER -15

#define GIFTRANDOMNUM 5

#define ISDRAW false

#define FLYTIME 5

#define GOLDTAG 0
#define POWERTAG 1
#define FLYGOLDTAG 2
#define PROTECTTAG 3
#define KEY2TAG 12
#define KEY3TAG 13
#define KEY4TAG 14
#define GIFT1TAG 21
#define GIFT2TAG 22

static GameScene* pGameSceneInstance = NULL;
static Player* pPlayerInstance = NULL;
static CCArray* pEnemyArray = NULL;
static CCArray* pUpBoxArray = NULL;

float mapMoveTime = 1;
float baseMapMoveTime = 1;
int test = 1;
bool isStart;
static int taskNum[4];
int mapIndex = 0;
bool isGameMode1 = false;

CCAnimate* getAnimate(char* pFirstNameStr,int animationFrameCount,float dt){

	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCAnimation* pAnimation = CCAnimation::create();

	for(int i = 0;i < animationFrameCount;i++)
		pAnimation->addSpriteFrame(pCache->spriteFrameByName(CCString::createWithFormat(pFirstNameStr,i + 1)->getCString()));
	pAnimation->setDelayPerUnit(dt);

	return CCAnimate::create(pAnimation);
}

CCAnimate* getAnimate(char* pFirstNameStr,int firstIndex,int lastIndex,float dt){

	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCAnimation* pAnimation = CCAnimation::create();

	for(int i = firstIndex;i <= lastIndex;i++)
		pAnimation->addSpriteFrame(pCache->spriteFrameByName(CCString::createWithFormat(pFirstNameStr,i)->getCString()));
	pAnimation->setDelayPerUnit(dt);

	return CCAnimate::create(pAnimation);
}


int getDay(){

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

	return tm->tm_yday;
}

void GameScene::gameOver(){

	OverLayer* pOverLayer = OverLayer::create();
	pOverLayer->setNum(GameScene::shareUILayer()->getMetle(),
		GameScene::shareUILayer()->getGold());
	CCDirector::sharedDirector()->replaceScene(
		CCTransitionFade::create(0.2,getSceneWithLayer(pOverLayer)));	

	GameScene::saveEveryDayTaskData();
}

GameLayer* GameScene::shareGameLayer(){

	return pGameSceneInstance->pGameLayer;
}

GameUILayer* GameScene::shareUILayer(){

	return pGameSceneInstance->pGameUILayer;
}

BackgroundLayer* GameScene::shareBackgroundLayer(){

	return pGameSceneInstance->pBackgroundLayer;
}

void GameScene::checkEveryDayTaskData(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	
	int preDay = pSava->getIntegerForKey("preDays");
	int contentDay = getDay();
	
	if(preDay == contentDay){

	}else{

		pSava->setIntegerForKey("taskType",rand () % 2 + 1);
		pSava->setIntegerForKey("task1",0);
		pSava->setIntegerForKey("task2",0);
		pSava->setIntegerForKey("task3",0);
		pSava->setIntegerForKey("task4",0);
		pSava->setIntegerForKey("preDays",contentDay);

		pSava->setBoolForKey("isGetTask",false);
	}

	for(int i = 0;i < 4;i++){

		taskNum[i] = 0;
	}
}

void GameScene::saveEveryDayTaskData(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	//sava taskNum by 3 kind of task
	taskNum[0] = GameScene::shareUILayer()->getMetle();	

	//保存数据 距离
	int task1 = pSava->getIntegerForKey("task1");
	pSava->setIntegerForKey("task1",task1 + taskNum[0]);

	//杀敌
	int task2 = pSava->getIntegerForKey("task2");
	pSava->setIntegerForKey("task2",task2 + taskNum[1]);

	int task3 = pSava->getIntegerForKey("task3");
	pSava->setIntegerForKey("task3",task3 + taskNum[2]);
}

GameScene* GameScene::shareGameScene(){

	return pGameSceneInstance;
}

bool GameScene::init(){

	if(!CCScene::init())	return false;

	mapIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey("mapIndex");
	isGameMode1 = CCUserDefault::sharedUserDefault()->getBoolForKey("isGameMode1",true);

	if(pEnemyArray == NULL){

		pEnemyArray = CCArray::create();
		pEnemyArray->retain();
	}

	if(pUpBoxArray == NULL){

		pUpBoxArray = CCArray::create();
		pUpBoxArray->retain();
	}

	pGameSceneInstance = this;

	pBackgroundLayer = BackgroundLayer::create();
	addChild(pBackgroundLayer);

	pGameLayer = GameLayer::create();
	addChild(pGameLayer);

	pGameUILayer = GameUILayer::create();
	addChild(pGameUILayer);

	checkEveryDayTaskData();

	return true;
};

bool BackgroundLayer::init(){

	CCLayer::init();

	CCSprite* pRoot = CCSprite::create(CCString::createWithFormat("imgs/background%d.jpg",mapIndex)->getCString());
	pRoot->setAnchorPoint(ccp(0,0));
	addChild(pRoot);

	pRoot = CCSprite::create(CCString::createWithFormat("imgs/background%d.png", mapIndex)->getCString());
	pRoot->setAnchorPoint(ccp(0,0));
	pRoot->setPositionX(pRoot->getContentSize().width);
	addChild(pRoot);

	for(int i = 0;i < 2;i++){

		pBackground[i] = CCNode::create();
		addChild(pBackground[i]);
	}

	pBackground[1]->setVisible(false);

	for(int i = 0;i < 4;i++){

		for(int j = 0;j < 2;j++){

			CCSprite* pSprite =CCSprite::create(CCString::createWithFormat("imgs/background%d.png", mapIndex)->getCString());
			pSprite->setAnchorPoint(ccp(0.5,0));
			bgWidth = pSprite->getContentSize().width;
			pSprite->setPosition(ccp(i * bgWidth,0));		

			if(i % 2 != 0)
				pSprite->setScaleX(-1);

			pBackground[j]->addChild(pSprite);
		}
	}

	for(int i = 0;i < 2;i++)
		pBackground[i]->setPositionX(-bgWidth * 1.5);

	pBackground[0]->setPositionY(-50);

	return true;
}

void BackgroundLayer::move(){

	for(int i = 0;i < 2;i++){

		pBackground[i]->setPositionX(pBackground[i]->getPositionX() - BACKGROUNDSPEED / (i == 0 ? 1.2 : 1));

		if(pBackground[i]->getPositionX() < -bgWidth * 2){

			pBackground[i]->setPositionX(pBackground[i]->getPositionX() + bgWidth * 2);
		}
	}
}

bool GameLayer::init(){

	if(!CCLayer::init())	return false;

	isInGoldScene = false;
	mapCount = 0;
	pFirstMap = NULL;
	pPlayerNext = NULL;
	isStart = true;
	test = 0;
	baseMapMoveTime = 1;
	mapMoveTime = 1;

	pCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	pCache->addSpriteFramesWithFile("plist/jump_light.plist");
	pCache->addSpriteFramesWithFile("plist/fly.plist");
	pCache->addSpriteFramesWithFile("plist/eat.plist");
	pCache->addSpriteFramesWithFile("plist/root.plist");
	pCache->addSpriteFramesWithFile("plist/enemy bow.plist");
	pCache->addSpriteFramesWithFile("plist/bigLoad.plist");
	pCache->addSpriteFramesWithFile("plist/gift.plist");

	pMapNode = CCNode::create();
	pMapNode->setTag(TMXTAG);
	addChild(pMapNode);

	pGoldScene = CCNode::create();
	pGoldScene->setTag(GOLDTMXTAG);
	pGoldScene->setVisible(false);
	pMapNode->addChild(pGoldScene);

	//addChild(DrawLayer::create());

	pPlayer = Player::create();
	pPlayer->setZOrder(1);
	pMapNode->addChild(pPlayer);

	pPlayer->setFoucs();	

	pCloud = CCSprite::create();
	pCloud->setAnchorPoint(ccp(0, 0));
	pCloud->setPosition(ccp(400, 0));
	pCloud->setZOrder(8000);
	pCloud->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1, ccp(-10, 0)), 2), CCEaseInOut::create(CCMoveBy::create(1, ccp(10, 0)), 2), NULL)));
	
	pCloud->setVisible(false);

	addChild(pCloud);

	createMap();
	createMap();

	createGoldMap();

	setTouchEnabled(true);

	scheduleUpdate();
	schedule(schedule_selector(GameLayer::addSpeedLine),0.5);

	schedule(schedule_selector(GameLayer::createFire),7);

	pHurtLight = CCSprite::create("imgs/hurt.png");
	pHurtLight->setAnchorPoint(ccp(0, 0));
	addChild(pHurtLight);

	pHurtLight->setOpacity(0);

	pHurtLight->setZOrder(9999);

	pBoss = Boss::create();
	pBoss->setVisible(false);
	addChild(pBoss);

	return true;
}

void GameLayer::update(float delta){

	if(pPlayer->isInDie())	return;

	int contentMetle = GameScene::shareUILayer()->getMetle();

	if(contentMetle > 10000){

		CCDirector::sharedDirector()->end();
	}

	createRect();
	
	//check is goldScene out of screen
	if(isInGoldScene){


		if(pGoldScene->getPositionX() + pMapNode->getPositionX() < -7 * MAPWIDTH){

			changeOutGold();
		}		
	}

	//addPlayer
	if(contentMetle / 100 % 5 == 0){

	//	addNextPlayer(1);		
	}

	//move background
	GameScene::shareBackgroundLayer()->move();

	//move map and player
	pMapNode->setPositionX(pMapNode->getPositionX() - MAPSPEED);

	if(isStart)
		GameScene::shareUILayer()->addScore(MAPSPEED);	

	//check out if map out screen
	if(pMapNode->getPositionX() < -((mapCount - 1)* MAPWIDTH) + (!isStart ? -1 * beginX : 0)){

		createMap();
		pMapNode->removeChild(pFirstMap);
	}

	float test = GameScene::shareUILayer()->getMetle();

	//check out if it's need to add spped
	if(baseMapMoveTime <= 10 && 
		GameScene::shareUILayer()->getMetle() / 1000 >= baseMapMoveTime){

		baseMapMoveTime += 1;

		CCSprite* pTitle = CCSprite::create("GameUI/addSpeedTitle.png");
		pTitle->setAnchorPoint(ccp(0,0));
		pTitle->setPosition(ccp(800,300));
		pTitle->runAction(CCSequence::create(CCMoveBy::create(1,ccp((pTitle->getContentSize().width + 800) * -1,0)),CCRemoveSelf::create(),NULL));
		pTitle->setZOrder(9999);		
		addChild(pTitle);
	}

	setCloudAlpha(pPlayer->isInGlass());
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){

	return;

	CCTouch* pTouch = (CCTouch*)(*pTouches->begin());
	if(pTouch->getLocation().x < 400){

		pPlayer->doAtt();
	}else{

		pPlayer->doJump();
	}
}

void GameLayer::doShaky(){

	pMapNode->runAction(CCRepeat::create(CCSequence::create(CCMoveBy::create(0.03,ccp(0,-20)),CCMoveBy::create(0.03,ccp(0,20)),CCMoveBy::create(0,ccp(0,-getPositionY())),NULL),5));
}

void GameLayer::doLight(float time){

	CCSprite* pLight = CCSprite::create("GameUI/light.jpg");
	pLight->setAnchorPoint(ccp(0.5, 0.5));
	pLight->setScale(100);
	addChild(pLight);

	pLight->setZOrder(9999);
	pLight->setOpacity(0);
	pLight->runAction(CCSequence::create(CCFadeIn::create(0.3),
		CCDelayTime::create(time),CCFadeOut::create(0.5),CCRemoveSelf::create(),NULL));

	clearEnemyInScreen();
}

void GameLayer::clearEnemyInScreen(){

	for(int i = 0;i < pEnemyArray->count();i++){

		Enemy* pEnemy = ((Enemy*)pEnemyArray->objectAtIndex(i));

		//getPositionX() - 61 / 2 + getParent()->getPositionX()
		float x = pEnemy->getPositionX() - 61 / 2 + pEnemy->getParent()->getPositionX() + pEnemy->getParent()->getParent()->getPositionX();
		if(x > 0 && x < 800){

			pEnemy->die();
		}
	}
}

void GameLayer::addHurtLight(){

	pHurtLight->runAction(CCSequence::create(CCFadeIn::create(0.15),CCFadeOut::create(0.15),NULL));
}

vector<CCRect> GameLayer::getRectVector(){

	/*CCRect test(0,-5,100,100);
	vector<CCRect> ret;
	ret.push_back(test);*/
	return rectVector;
}

vector<CCRect> GameLayer::getMoveRectVector(){

	return moveRectVector;
};

CCNode* GameLayer::getMap(){

	return pMapNode;
}

bool Human::isNeedJump(){

	if(dropSpeed < 0)
		return false;

	if(getPositionY() < 70 && dropSpeed > 2)
		return true;

	CCRect contentRect = getRect();

	vector<CCRect> mapRect = GameScene::shareGameLayer()->getRectVector();		

	for(vector<CCRect>::iterator it = mapRect.begin();it != mapRect.end();it++){

		CCRect collRect = (CCRect)*it;

		{

			float maxX = collRect.getMaxX();
			float maxY = collRect.getMaxY();
			collRect.setRect(maxX - SPRITEWIDTH / 2,maxY - SPRITEHEIGHT / 2,SPRITEWIDTH,SPRITEHEIGHT);
		}

		if(collRect.intersectsRect(contentRect)){ 
			
			return true;
		}
	}

	return false;
}

void Human::updateMove(float speed){

	CCRect contentRect = getRect();
	float temp = getPositionX() - contentRect.getMaxX();
	
	contentRect.setRect(contentRect.getMinX() + speed,contentRect.getMinY() + dropSpeed,
		contentRect.getMaxX() - contentRect.getMinX(),contentRect.getMaxY() - contentRect.getMinY());

	vector<CCRect> mapRect = GameScene::shareGameLayer()->getMoveRectVector();		

	if(!GameScene::shareGameLayer()->getIsInGoldScene())
	{
		for(int i = 0;i < pEnemyArray->count();i++){

			Enemy* pEnemy = ((Enemy*)pEnemyArray->objectAtIndex(i));
			
			if(!pEnemy->isInDie())
				mapRect.push_back(pEnemy->getMoveRect());
		}
	
		for(int i = 0;i < pUpBoxArray->count();i++){

			UpBox* pEnemy = ((UpBox*)pUpBoxArray->objectAtIndex(i));
			
			mapRect.push_back(pEnemy->getRect());
		}
	}

	float isHaveNextX = -1;

	for(vector<CCRect>::iterator it = mapRect.begin();it != mapRect.end();it++){

		CCRect collRect = (CCRect)*it;

		int natureX = collRect.getMinX();

		float minX = collRect.getMinX();
		float minY = collRect.getMinY();
		float maxX = collRect.getMaxX() - minX;
		float maxY = collRect.getMaxY() - minY;
		collRect.setRect(minX,minY,maxX + speed,maxY);
	
		if(collRect.intersectsRect(contentRect) && collRect.getMinX() + speed >= contentRect.getMaxX()){

			//有碰撞
			float temp = natureX;
			if(isHaveNextX < temp){

				isHaveNextX = temp;
			}
		}
	}

	if(isHaveNextX == -1){

		setPositionX(contentRect.getMaxX() + temp);			
	}else{

		setPositionX(isHaveNextX + temp);
	}
}

void Human::updateDrop(float dt){

	bool isByRect = true;
	
	if(isByRect){
		
		CCRect contentRect = getRect();
		float temp = getPositionY() - contentRect.getMinY();
		contentRect.setRect(contentRect.getMinX(),contentRect.getMinY() - dropSpeed,
			contentRect.getMaxX() - contentRect.getMinX(),2);

		vector<CCRect> mapRect = GameScene::shareGameLayer()->getRectVector();

		if(!GameScene::shareGameLayer()->getIsInGoldScene())
		{
			if(this == Player::sharePlayer())
			for(int i = 0;i < pUpBoxArray->count();i++){

				UpBox* pEnemy = ((UpBox*)pUpBoxArray->objectAtIndex(i));

				mapRect.push_back(pEnemy->getDropRect());
			}
		}

		float isHaveNextY = -1;

		for(vector<CCRect>::iterator it = mapRect.begin();it != mapRect.end();it++){

			CCRect collRect = (CCRect)*it;

			int natureY = collRect.getMaxY();

			if(dropSpeed > 0){

				float minX = collRect.getMinX();
				float minY = collRect.getMinY();
				float maxX = collRect.getMaxX() - minX;
				float maxY = collRect.getMaxY() - minY;
				collRect.setRect(minX,minY,maxX,maxY + dropSpeed);
			}

			if(collRect.intersectsRect(contentRect) && dropSpeed > 0){

				//有碰撞
				float temp = natureY;
				if(isHaveNextY < temp){

					isHaveNextY = temp;
				}
			}
		}

		if(isHaveNextY == -1){

			setPositionY(contentRect.getMinY() + temp);			
		}else{

			float tempData = dropSpeed;

			if(isCanSqu){

				dropSpeed = -dropSpeed / 4.0;
			}else{

				dropSpeed = 0;
			}

			setPositionY(isHaveNextY + temp);
			
			if(tempData > G)
				onLoad();
		}

		if(isCanAddSpeed)
			dropSpeed += G;
	}else{

		CCPoint contentPoint = getStandPosition();
		float temp = getPositionY() - contentPoint.y;
		contentPoint.y -= dropSpeed;

		vector<CCRect> mapRect = GameScene::shareGameLayer()->getRectVector();

		float isHaveNextY = -1;

		for(vector<CCRect>::iterator it = mapRect.begin();it != mapRect.end();it++){

			CCRect collRect = (CCRect)*it;

			int natureY = collRect.getMaxY();

			if(dropSpeed > 0){

				float minX = collRect.getMinX();
				float minY = collRect.getMinY();
				float maxX = collRect.getMaxX() - minX;
				float maxY = collRect.getMaxY() - minY;
				collRect.setRect(minX,minY,maxX,maxY + dropSpeed);
			}

			if(collRect.containsPoint(contentPoint) && dropSpeed > 0){

				//有碰撞
				float temp = natureY;
				if(isHaveNextY < temp){

					isHaveNextY = temp;
				}
			}
		}

		if(isHaveNextY == -1){

			setPositionY(contentPoint.y + temp);			
		}else{

			if(isCanSqu){

				dropSpeed = -dropSpeed / 4.0;
			}else{

				dropSpeed = 0;
			}

			setPositionY(isHaveNextY + temp);
			onLoad();			
		}

		if(isCanAddSpeed)
			dropSpeed += G;	
	}
}

bool Human::init(){

	if(CCSprite::init() == false)	return false;

	dropSpeed = 0;
	isCanSqu = false;
	isCanAddSpeed = true;
	isCanReJump = false;

	return true;
}

CCPoint Human::getStandPosition(){

	return getPosition();
}

void Human::setDropSpeed(float speed){

	dropSpeed = speed;
}

void Human::setIsCanAddSpeed(bool isCanAddSpeed){

	this->isCanAddSpeed = isCanAddSpeed;
}

void Human::onLoad(){

	//do no thing
	if(!isCanReJump)
		unscheduleUpdate();
}

void GameLayer::createGoldMap(){

	for(int i = 1;i <= 8;i++){

		CCTMXTiledMap* pMap = CCTMXTiledMap::create(CCString::createWithFormat("maps/gold (%d).tmx",i)->getCString());
		pMap->setTag(TMXTAG);
		pMap->setZOrder(-1);
		pMap->setPositionX((i - 1) * MAPWIDTH);
		pGoldScene->addChild(pMap);
		
		createGold(pMap);
	}
}

void GameLayer::resetGoldSceneGold(bool isStart){

	CCArray* pChildren = pGoldScene->getChildren();
	
	for(int i = (isStart ? 0 : 4);i < pChildren->count();i++){

		if(isStart){

			if(i == 4)
				return;
		}

		CCTMXTiledMap* pMap = (CCTMXTiledMap*)pChildren->objectAtIndex(i);
		pMap->removeChildByTag(3);
		createGold(pMap);
	}
}

void GameLayer::changeInGold(){

	isInGoldScene = true;	
	
	pGoldScene->setPositionX(pMapNode->getPositionX() * -1);
	
	pGoldScene->runAction(CCSequence::create(CCDelayTime::create(0.5),CCShow::create(),NULL));
	
	if(!isGameMode1){
	
		pBoss->stay();
		pBoss->runAction(CCSequence::create(CCDelayTime::create(0.5),CCShow::create(),NULL));
	}

	pPlayer->doFly(2);
	
	doLight(1);

	resetGoldSceneGold();
}

void GameLayer::changeOutGold(){

	isInGoldScene = false;	
	pGoldScene->runAction(CCSequence::create(CCDelayTime::create(0.5),CCHide::create(),NULL));

	if(!isGameMode1)
		pBoss->runAction(CCSequence::create(CCDelayTime::create(0.5),CCHide::create(),NULL));

	pPlayer->doFly(2);
	doLight(1);

	resetGoldSceneGold(false);
}

void GameLayer::createMap(){


	//CCTMXTiledMap* pMap = CCTMXTiledMap::create(CCString::createWithFormat("maps/map (%d).tmx",!isStart ? 0 : test ++)->getCString());
	//CCTMXTiledMap* pMap = CCTMXTiledMap::create(CCString::createWithFormat
	//	("maps/map1.tmx")->getCString());
	
	CCTMXTiledMap* pMap = CCTMXTiledMap::create(CCString::createWithFormat("map%d/map%d.tmx",mapIndex,!isStart ? 0 : test ++)->getCString());

	if(!pMap){
		
		test = 1;
		createMap();

		return;
	}

	pMap->setTag(TMXTAG);
	pMap->setZOrder(-1);
	pMap->setPositionX(mapCount * MAPWIDTH);
	pMapNode->addChild(pMap);

	createEnemy(pMap);
	createUp(pMap);
	createGold(pMap);

	//pMap->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(0,-10)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(0,10)),2),NULL)));

	mapCount ++;
}

void GameLayer::createUp(CCTMXTiledMap* pMap){

	CCTMXObjectGroup* pRectLayer = pMap->objectGroupNamed("up");

	if(pRectLayer != NULL){

		CCArray* pArray = pRectLayer->getObjects();
		int x,y,width,height;
		for(int i = 0;i < pArray->count();i++){

			CCDictionary* pDictionary = (CCDictionary*)pArray->objectAtIndex(i);

			x = pDictionary->valueForKey("x")->intValue() + 60;
			y = pDictionary->valueForKey("y")->intValue() + 40;

			UpBox* pEnemy = UpBox::create();
			pEnemy->setPosition(ccp(x,y));			
			pEnemy->setZOrder(1023);
			pMap->addChild(pEnemy);
		}
	}
}

void GameLayer::createEnemy(CCTMXTiledMap* pMap){

	CCTMXObjectGroup* pRectLayer = pMap->objectGroupNamed("enemy");

	if(pRectLayer != NULL){

		CCArray* pArray = pRectLayer->getObjects();
		int x,y,width,height;
		for(int i = 0;i < pArray->count();i++){

			CCDictionary* pDictionary = (CCDictionary*)pArray->objectAtIndex(i);
			
			x = pDictionary->valueForKey("x")->intValue() + 60;
			y = pDictionary->valueForKey("y")->intValue() + 40;

			int type = pDictionary->valueForKey("type")->intValue();

			if(type == 0){

				Enemy* pEnemy = Enemy::create();
				pEnemy->setPosition(ccp(x,y));			
				pEnemy->setZOrder(1023);
				pMap->addChild(pEnemy);
			}else{

				UpBox* pEnemy = UpBox::create();
				pEnemy->setPosition(ccp(x,y));			
				pEnemy->setZOrder(1023);
				pMap->addChild(pEnemy);
			}
		}
	}
}

//int test1 = 0;
void GameLayer::createGold(CCTMXTiledMap* pMap){

	CCTMXObjectGroup* pRectLayer = pMap->objectGroupNamed("items");

	CCSpriteBatchNode* pBatch = CCSpriteBatchNode::create("plist/gift.png");
	/*CCNode* pBatch = CCNode::create();*/
	pBatch->setZOrder(999);
	pBatch->setTag(3);
	pMap->addChild(pBatch);

	if(pRectLayer != NULL){

		CCArray* pArray = pRectLayer->getObjects();
		int x,y,width,height;
		for(int i = 0;i < pArray->count();i++){

			CCDictionary* pDictionary = (CCDictionary*)pArray->objectAtIndex(i);
			x = pDictionary->valueForKey("x")->intValue() + 20;
			y = pDictionary->valueForKey("y")->intValue() + 40;
		
			Gift* pGift = Gift::create();
			pGift->setPosition(ccp(x,y));			
			pBatch->addChild(pGift);
		}
	}
}

//void GameLayer::draw(){
//
//	CCLayer::draw();
//
//	ccDrawColor4B(0, 0, 255, 255);
//	glLineWidth(5);
//	
//	ccDrawRect(ccp(0,0),ccp (100,100));
//}

void GameLayer::low(){

	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->setTimeScale(0.7f);//实现减速效果

	//CCDirector::sharedDirector()->setAnimationInterval(1 / 45.0);
	mapMoveTime = 0.7;

	runAction(CCSequence::create(CCScaleTo::create(0.1,1.3),CCDelayTime::create(0.2),CCCallFunc::create(this,callfunc_selector(GameLayer::up)),NULL));
}

void GameLayer::up(){

	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->setTimeScale(1);//实现减速效果

	//CCDirector::sharedDirector()->setAnimationInterval(1 / 60.0);
	mapMoveTime = 1;

	runAction(CCSequence::create(CCScaleTo::create(0.1,1),NULL));
}

void GameLayer::setCloudAlpha(bool isInGlass){

	pCloud->setOpacity(isInGlass ? 100 : 254);
}

void GameLayer::addSpeedLine(float dt){

	if(baseMapMoveTime == 1)	return;

	CCSprite* pTitle = CCSprite::create("GameUI/speedLine.png");
	pTitle->setAnchorPoint(ccp(0,0));
	pTitle->setPosition(ccp(800,rand () % 750));
	pTitle->runAction(CCSequence::create(CCMoveBy::create(0.5,ccp((pTitle->getContentSize().width + 800) * -1,0)),CCRemoveSelf::create(),NULL));
	pTitle->setZOrder(9999);		
	pTitle->setScaleY(0.5);
	addChild(pTitle);
}

void GameLayer::createFire(float dt){

	if(pPlayer->isInDie() || GameScene::shareUILayer()->getMetle() < 500)	return;

	addChild(Fire::create());
}

void GameLayer::createRect(){

	rectVector.clear();
	moveRectVector.clear();
	
	if(pPlayer->isOnDieFly())	return;
	
	int minX = INT_MAX;
	pFirstMap = NULL;

	CCArray* pChildren;

	if(isInGoldScene){
	
		pChildren = pGoldScene->getChildren();
	
		{

			CCArray* pChildren = pMapNode->getChildren();

			for(int i = 0;i < pChildren->count();i++){

				CCNode* pTemp = (CCNode*)pChildren->objectAtIndex(i);
				if(pTemp->getTag() == TMXTAG){

					CCTMXTiledMap* pMap = (CCTMXTiledMap*)pTemp;
					if(pMap->getPositionX() < minX){

						minX = pMap->getPositionX();
						pFirstMap = pMap;
					}
				}
			}
		}
	}else
		pChildren = pMapNode->getChildren();
	
	for(int i = 0;i < pChildren->count();i++){

		CCNode* pTemp = (CCNode*)pChildren->objectAtIndex(i);
		if(pTemp->getTag() == TMXTAG){

			CCTMXTiledMap* pMap = (CCTMXTiledMap*)pTemp;
			char* name[] = {"collision","collision1"};

			for(int i = 0;i < 2;i++){

				CCTMXObjectGroup* pRectLayer = pMap->objectGroupNamed(name[i]);

				if(!isInGoldScene){

					if(pMap->getPositionX() < minX){

						minX = pMap->getPositionX();
						pFirstMap = pMap;
					}
				}		

				if(pRectLayer != NULL){

					CCArray* pArray = pRectLayer->getObjects();
					int x,y,width,height;
					for(int i = 0;i < pArray->count();i++){

						CCDictionary* pDictionary = (CCDictionary*)pArray->objectAtIndex(i);
						x = pDictionary->valueForKey("x")->intValue() + pMap->getPositionX() + (isInGoldScene ? pGoldScene->getPositionX() : 0);
						y = pDictionary->valueForKey("y")->intValue() + pMap->getPositionY();
						width = pDictionary->valueForKey("width")->intValue();
						height = pDictionary->valueForKey("height")->intValue();
						
						if(y + height > 400)
							continue;
						
						moveRectVector.insert(moveRectVector.begin(),CCRect(x,y,width,height));
						y = y + height - 5;
						height = 5;
						rectVector.insert(rectVector.begin(),CCRect(x,y,width,height));
					}
				}
			}
		}
	}
}

void Player::createRoot(){

	return;

	CCSprite* pRoot = CCSprite::create();
	pRoot->setPosition(getPosition());
	pRoot->runAction(CCSequence::create(getAnimate("root%d.png",4,0.07),CCRemoveSelf::create(),NULL));
	getParent()->addChild(pRoot);
}

void Player::flyDone(){ 
	
	if(pFly != NULL){

		removeChild(pFly,true);
		pFly = NULL;		
	}

	toolDoneAndStart();

	setDropSpeed(0);

	state = RUN; 
	mapMoveTime = 1; 
	doJump(); 
};

void Player::draw(){

	if(!ISDRAW)
		return;

	CCSprite::draw();

	ccDrawColor4B(0, 0, 255, 255);
	glLineWidth(5);

	ccDrawRect(ccp(-PLAYERWIDTH  * (1 / 4.0),0), ccp (-PLAYERWIDTH  * (1 / 4.0) + PLAYERWIDTH,PLAYERHEIGHT));
	ccDrawRect(ccp(-PLAYERWIDTH  * (1 / 4.0),0), ccp (-PLAYERWIDTH  * (1 / 4.0) + PLAYERWIDTH * 3,PLAYERHEIGHT / 2));
}

bool Player::init(){

	Human::init();

	mIsOnCar = false;
	isCanReJump = true;
	mIsDieFly = false;

	pFly = NULL;
	pProtect = NULL;
	pBody = NULL;

	state = RUN;
	isDie = false;

	isGlass = false;
	isShoes = false;

	mIsOnDieFly = false;

	mIsOnProtect = false;

	mMoneyTimes = 1;
	jumpTimes = 0;
	bloodNum = 1;

	pLightNode = CCNode::create();
	pLightNode->setZOrder(8888);
	addChild(pLightNode);

	initBody();

	checkBody();

	setPosition(ccp(250,250));

	scheduleUpdate();

	//doBig(100);

	//doInProtect();

	if(!isGameMode1){

		CCSprite* pAttLight = CCSprite::create("imgs/attLight.png");
		pAttLight->setAnchorPoint(ccp(0.5,0));
		pAttLight->setPosition(ccp(200,0));
		addChild(pAttLight);
	}

	return true;
}

void Player::doDie(){

	if(isDie || Player::sharePlayer() != this)
		return;

	if(GameScene::shareGameLayer()->changePlayer()){

	}else{

		if(false){

			GameScene::gameOver();
		}else{

			playSound("music/hurt.ogg");
			playSound("music/bow.ogg");

			isDie = true;
			mapMoveTime = 0;		

			doBow();
			GameScene::shareGameLayer()->doShaky();

			if(GameScene::shareUILayer()->endCheck()){


			}else{

				GameScene::shareGameLayer()->addChild(ReliveBoard::create());
			}
			//runAction(CCSequence::create(CCDelayTime::create(0.9),CCCallFunc::create(this,callfunc_selector(Player::doBow)),NULL));
		}
	}
}


void Player::doBow(){

	pBody->setVisible(false);

	CCSprite* pSprite = CCSprite::create();
	pSprite->runAction(CCSequence::create(getAnimate("enemy bow (%d).png", 17, 0.07), CCRemoveSelf::create(), NULL));
	pSprite->setAnchorPoint(ccp(0.5, 0.1));
	pSprite->setScale(1.5);
	pSprite->setZOrder(1024);
	pSprite->setPosition(getPosition());
	getParent()->addChild(pSprite);
}

void Player::hurt(){

	if(isInProtect()){

		GameScene::shareUILayer()->protectBrken();

		playSound("music/protect1.ogg");

		mIsOnProtect = false;
		pProtect->runAction(CCSequence::create(CCScaleTo::create(0.2,10),CCRemoveSelf::create(),NULL));
		GameScene::shareGameLayer()->clearEnemyInScreen();
	}else{

		GameScene::shareGameLayer()->addHurtLight();

		playSound("music/hurt.ogg");

		bloodNum --;

		if(bloodNum <= 0){

			doDie();
		}
	}
}

void Player::doRelive(){

	if (!isDie)
		return;

	mIsOnDieFly = false;
	bloodNum = 1;
	isDie = false;
	mapMoveTime = 1;
	pBody->setVisible(true);

	pBody->getAnimation()->resume();

	doFly(FLYTIME);

	/*runAction(CCMoveBy::create(0.3,ccp(250,0)));*/

	GameScene::shareGameLayer()->doLight();
}

void Player::doInProtect(){

	if(isInProtect())	return;

	playSound("music/protect1.ogg");

	mIsOnProtect = true;
	pProtect = CCSprite::create("imgs/protect.png");
	pProtect->setZOrder(99);
	addChild(pProtect);

	pProtect->setScale(0);

	float scale = isOnCar() ? 1 : 0.8;

	pProtect->setAnchorPoint(ccp(0.5,isOnCar() ? 0.25 : 0.23));

	pProtect->runAction(CCEaseBackOut::create(CCScaleTo::create(0.15,scale)));

	pProtect->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCScaleTo::create(0.3,scale),2),CCEaseInOut::create(CCScaleTo::create(0.3,scale + 0.1),2),NULL)));
}

void Player::doTakeGlass(){
	

	return;

	isGlass = true;
	pBody->getBone("Layer2")->changeDisplayByIndex(1,true);
	//GameScene::shareGameLayer()->setCloudAlpha(true);

	addTakeLight();
}

void Player::doUnTakeGlass(){

	isGlass = false;
	pBody->getBone("Layer2")->changeDisplayByIndex(0,true);

	addTakeLight();
}

void Player::doTakeShoes(){

	return;

	isShoes = true;
	pBody->getBone("Layer63")->changeDisplayByIndex(1,true);
	pBody->getBone("Layer64")->changeDisplayByIndex(1,true);

	addTakeLight();
}

void Player::doUnTakeShoes(){


	return;

	isShoes = false;
	pBody->getBone("Layer63")->changeDisplayByIndex(0,true);
	pBody->getBone("Layer64")->changeDisplayByIndex(0,true);

	addTakeLight();
}

void Player::doOnCar(){

	pBody->getAnimation()->stop();

	mIsOnCar = true;

	if(state == RUN){

		doRun();
	}

	addTakeLight();
}

void Player::doInMoney(){

	isMoney = true;
}

void Player::checkBody(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	bool isShoes = pSava->getBoolForKey("isShoes");
	bool isGlass = pSava->getBoolForKey("isGlass");
	bool isMoney = pSava->getBoolForKey("equBuy1");
	bool isCar = pSava->getBoolForKey("isCar");
	bool isChooseCar = pSava->getBoolForKey("isChooseOnCar");

	if(isMoney){
		
		doInMoney();
	}else{

		doUnInMoney();
	}

	return;

	if(isGlass){

		doTakeGlass();
	}else{

		doUnTakeGlass();
	}

	if(isShoes){

		doTakeShoes();
	}else{

		doUnTakeShoes();
	}

	if(isChooseCar){

		doOnCar();
	}else{

		doUnOnCar();
	}
}

void Player::doUnInMoney(){

	isMoney = false;
}

void Player::doUnOnCar(){

	/*pBody->getAnimation()->stop();*/

	mIsOnCar = false;

	if(state == RUN)
		doRun();
}

void Player::addTakeLight(){

	/*pLightNode->removeAllChildren();

	CCSprite* pLight = CCSprite::create();
	pLight->setAnchorPoint(ccp(0.5,0.19));
	pLight->setZOrder(9999);
	pLight->setScale(0.7);
	pLight->runAction(CCSequence::create(getAnimate("take light (%d).png",12,0.06),CCRemoveSelf::create(),NULL));
	pLightNode->addChild(pLight);*/
}

void Player::initBody(){

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("run/man10.png","run/man10.plist","run/man1.ExportJson");
	pBody = CCArmature::create("man1");
	pBody->setScale(PLAYERSCALE);
	pBody->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(Player::armatureCallBack));
	pBody->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Player::frameEventCallBack));
	addChild(pBody,2);

	doRun();

	return;

	CCSprite* pFootLeft = CCSprite::create("body/foot.png");
	pFootLeft->setPosition(ccp(60,10));
	pFootLeft->setRotation(-49.63);
	pBody->getBone("Layer64")->addDisplay(pFootLeft,0);

	CCSprite* pFootRight = CCSprite::create("body/foot.png");
	pFootRight->setPosition(ccp(54,12));
	pFootRight->setRotation(-50);
	pBody->getBone("Layer63")->addDisplay(pFootRight,0);

	CCSprite* pHand = CCSprite::create("body/hand.png");
	pHand->setPosition(ccp(48,7));
	pHand->setRotation(-30);
	pBody->getBone("Layer14")->addDisplay(pHand,0);
}

void GameLayer::removeNextPlayer(){

	pPlayerNext->runAction(CCRemoveSelf::create());
	pPlayerNext = NULL;	
}

void GameLayer::addNextPlayer(int personIndex){

	if(pPlayerNext != NULL)
		return;

	pPlayerNext = Player::create();
	pPlayerNext->setPositionX(-pMapNode->getPositionX());
	pPlayerNext->setZOrder(1);
	pMapNode->addChild(pPlayerNext);
}

bool GameLayer::changePlayer(){

	if(pPlayerNext != NULL){

		pPlayer->removeFromParent();

		pPlayerNext->setFoucs();
		pPlayer = pPlayerNext;	
		pPlayerNext = NULL;
		
		float needMoveX = pPlayer->getPositionX() + pPlayer->getParent()->getPositionX() - PLAYERSTARTX;
		pMapNode->runAction(CCMoveBy::create(0.2,ccp(-needMoveX,0)));

		return true;
	}else{

		return false;
	}
}

void Player::setFoucs(){

	pPlayerInstance = this;
}

bool Player::isFoucs(){

	return pPlayerInstance == this;
}

Player* Player::sharePlayer(){
	
	return pPlayerInstance;
}

CCRect Player::getRect(){

	return CCRect(getPositionX() - PLAYERWIDTH * (1 / 4.0),getPositionY(),PLAYERWIDTH,PLAYERHEIGHT * (isInDown() ? 0.5 : 1));
}

CCRect Player::getFightRect(){

	return CCRect(getPositionX() - PLAYERWIDTH * (1 / 4.0),getPositionY() - PLAYERHEIGHT,PLAYERWIDTH * 4,PLAYERHEIGHT * 3);
}

void Player::doRun(){

	state = RUN;
	pBody->getAnimation()->playByIndex(RUNINDEX(mIsOnCar));
}

void Player::doAtt(){

	if(state == FLY || isDie)
		return;

	if(CCDirector::sharedDirector()->isPaused())	return;

	playSound("music/att.ogg");
	//fightEnemys();

	state = ATT;
	pBody->getAnimation()->playByIndex(ATTINDEX(mIsOnCar));

	CCSprite* pRoot = CCSprite::create();
	pRoot->setAnchorPoint(ccp(0.5,0));
	pRoot->setPosition(getPosition());
	pRoot->runAction(CCSequence::create(getAnimate("bigLoad%d.png",4,0.11),CCRemoveSelf::create(),NULL));
	getParent()->addChild(pRoot);

	GameScene::shareGameLayer()->getBoss()->hurt();

	fightEnemys();
}

void Player::doDown(){

	if(state == FLY || isDie)
		return;

	if(CCDirector::sharedDirector()->isPaused())	return;

	state = DOWN;

	pBody->getAnimation()->playByIndex(DOWNINDEX);
}

void Player::doBig(int bigTime){

	if(isInBig())	return;

	playSound("music/big.ogg");

	toolDoneAndStart();

	runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.2,1.7)),CCDelayTime::create(bigTime),CCCallFunc::create(this,callfunc_selector(Player::toolDoneAndStart)),CCEaseBackOut::create(CCScaleTo::create(0.2,1)),NULL));
	pBody->runAction(CCRepeat::create(CCSequence::create(CCTintTo::create(0.25,200,0,0),CCTintTo::create(0.25,255,255,255),CCDelayTime::create(0.5),NULL),bigTime));
}

void Player::toolDoneAndStart(){

	/*if(!isInFly())	
		GameScene::shareGameLayer()->doLight(0.2);*/

	if(isInBig())
		playSound("music/big.ogg");

	GameScene::shareGameLayer()->clearEnemyInScreen();
}

void Player::doFly(int flyTime){

	if(state == FLY)
		return;

	playSound("music/fly.ogg");

	/*CCParticleFire* pFlyFire = CCParticleFire::create();
	pFlyFire->setAnchorPoint(ccp(0,0));
	pFlyFire->setPosition(10,75 + PLAYERHEIGHT);
	pFlyFire->setZOrder(1024);
	addChild(pFlyFire);

	pFlyFire->setSourcePosition(ccp(-100,0));
	pFlyFire->setSpeed(pFlyFire->getSpeed() * 2);
	pFlyFire->setRotation(-90);*/

	mapMoveTime = 3;

	pFly = CCSprite::create();
	pFly->setAnchorPoint(ccp(0.5,0.5));
	pFly->setPosition(ccp(-30,30));
	pFly->runAction(CCScaleTo::create(0.2,1.5));
	pFly->setZOrder(1024);
	pFly->setRotation(-90);
	addChild(pFly);

	if(isStart){

		pFly->runAction(CCRepeatForever::create(CCSequence::create(getAnimate("fly (%d).png",15,0.07)->reverse(),NULL)));
		pFly->runAction(CCSequence::create(CCDelayTime::create(flyTime),CCCallFunc::create(this,callfunc_selector(Player::flyDone)),NULL));
		runAction(CCMoveBy::create(0.3,ccp(0,300 - getPositionY())));
		runAction(CCSequence::create(CCMoveBy::create(flyTime / 2.0,ccp(100,0)),CCMoveBy::create(flyTime / 2.0,ccp(-100,0)),NULL));
	}else{

		pFly->runAction(CCRepeatForever::create(CCSequence::create(getAnimate("fly (%d).png",15,0.07)->reverse(),NULL)));		
		runAction(CCMoveBy::create(0.3,ccp(0,200 - getPositionY())));		
	}

	state = FLY;
	pBody->getAnimation()->playByIndex(FLYINDEX(mIsOnCar));
}

/*CCBone *bone;
const char *frameEventName;
int originFrameIndex;
int currentFrameIndex;*/
void Player::frameEventCallBack(CCBone *bone, const char *name, int originFrameIndex, int currentFrameIndex){

	if(strcmp(name,"step") == 0){

		createRoot();
	}else if(strcmp(name,"att1") == 0){

		fightEnemys();
		//GameScene::shareGameLayer()->doShaky();
	}
}

void Player::fightEnemys(){
	CCSprite* pWarnning = CCSprite::create("imgs/warning.png");
	pWarnning->setAnchorPoint(ccp(0, 0.16));
	pWarnning->setPosition(ccp(pBody->getPositionX()+pBody->getContentSize().width/3, pBody->getPositionY() + pBody->getContentSize().height / 3));
	pWarnning->setVisible(false);
	addChild(pWarnning);
	pWarnning->runAction(CCSequence::create(CCBlink::create(0.1, 1), CCHide::create(), CCRemoveSelf::create(), NULL));
	for(int i = 0;i < pEnemyArray->count();i++){

		Enemy* pEnemy = (Enemy*)pEnemyArray->objectAtIndex(i);
		if(getFightRect().intersectsRect(pEnemy->getRect())){

			GameScene::shareGameLayer()->low();
			pEnemy->die();
			//pEnemy->runAction(CCRemoveSelf::create());
		}
	}
}

void Player::fightEnemysOnCar(){

	if(jumpTimes == 0 || dropSpeed <= 1 || isOnCar() == false)
		return;

	for(int i = 0;i < pEnemyArray->count();i++){

		Enemy* pEnemy = (Enemy*)pEnemyArray->objectAtIndex(i);
		if(pEnemy->getRect().intersectsRect(getRect())){
			
			pEnemy->bow();

			jumpTimes = 0;
			doJump();
			//pEnemy->runAction(CCRemoveSelf::create());
		}
	}
}

bool Player::checkIsCanUpJump(){

	if(isInUpJump() || GameScene::shareGameLayer()->getIsInGoldScene())
		return false;

	for(int i = 0;i < pUpBoxArray->count();i++){

		UpBox* pEnemy = (UpBox*)pUpBoxArray->objectAtIndex(i);
		if(pEnemy->getRect().intersectsRect(getRect())){
		
			setPositionY(pEnemy->getPositionY() + 200);

			return true;
		}
	}

	return false;
}

bool Player::isCanBeHurt(){

	if(mIsOnCar)
		if(!(jumpTimes == 0 || dropSpeed <= 1))
			return false;

	if(isInFly() || isInatt())	return false;

	return true;
}

void Player::armatureCallBack(CCArmature* pArmature, MovementEventType type, const char* str){


	switch (type)
	{
	case cocos2d::extension::START:
		break;
	case cocos2d::extension::COMPLETE:
		
		break;
	case cocos2d::extension::LOOP_COMPLETE:
				
		if(strcmp(str,"att") == 0 || strcmp(str,"carAtt") == 0){

			doRun();
		}
		break;
	default:
		break;
	}
}

void Enemy::armatureCallBack(CCArmature* pArmature, MovementEventType type, const char* str){


	switch (type)
	{
	case cocos2d::extension::START:
		break;
	case cocos2d::extension::COMPLETE:

		break;
	case cocos2d::extension::LOOP_COMPLETE:

		if(strcmp(str,"die") == 0){

			bow();
		}
		break;
	default:
		break;
	}
}

void Player::doUpJump(){

	playSound("music/up.ogg");

	pBody->getAnimation()->playByIndex(JUMP2INDEX(mIsOnCar));

	jumpTimes = 99;
	mapMoveTime = 1.8;

	CCSprite* pLight = CCSprite::create();
	pLight->setPosition(getPosition());
	pLight->runAction(CCSequence::create(getAnimate("jump_light%d.png",7,0.1),CCRemoveSelf::create(),NULL));	
	GameScene::shareGameLayer()->getMap()->addChild(pLight);

	setDropSpeed(UPJUMPPOWER);
}

void Player::doJump(){

	if(state == FLY || isDie)
		return;

	if(CCDirector::sharedDirector()->isPaused())	return;

	jumpTimes ++;	

	if(jumpTimes > getMaxJumpTimes() && isFoucs())
		return;

	CCSprite* pLight = CCSprite::create();
	pLight->setPosition(getPosition());
	pLight->runAction(CCSequence::create(getAnimate("jump_light%d.png",7,0.1),CCRemoveSelf::create(),NULL));
	GameScene::shareGameLayer()->getMap()->addChild(pLight);

	if(jumpTimes == 1){

		pBody->getAnimation()->playByIndex(JUMP1INDEX(mIsOnCar));
		setDropSpeed(JUMP1POWER);
		playSound("music/jump.mp3");

		pLight->setVisible(false);
	}else{

		pBody->getAnimation()->playByIndex(JUMP2INDEX(mIsOnCar));
		setDropSpeed(JUMP2POWER);
		playSound("music/jump.mp3");	
	}
}

void Player::update(float delta){

	if(isDie)	return;

	if(!isFoucs()){

		if(isNeedJump())
			doJump();

		if(getPositionX() + getParent()->getPositionX() > 800){

			GameScene::shareGameLayer()->removeNextPlayer();
			return;
		}
	}

	//if(mIsOnCar)
	fightEnemysOnCar();

	if(state != FLY){

		Human::updateDrop(delta);
		
		if(isFoucs()){

			Human::updateMove(getPositionX() + getParent()->getPositionX() < 
				PLAYERSTARTX ? MAPSPEED * 1.5 : MAPSPEED);
		}else{

			setPositionX(getPositionX() + MAPSPEED * 1.5);
		}

		//isDie
		float contentX = getPositionX() + getParent()->getPositionX();
		if ((contentX < 0 || getPositionY() < -PLAYERHEIGHT)){

			doDie();
		}
	}else{

		setPositionX(getPositionX() + MAPSPEED);
		/*setPositionX(getPositionX() + MAPSPEED * 2);
		GameScene::shareGameLayer()->getMap()->setPositionX(GameScene::shareGameLayer()->getMap()->getPositionX() - MAPSPEED * 2);
	
		GameScene::shareUILayer()->addScore(MAPSPEED * 2);*/
	
		for (int i = 0; i < pEnemyArray->count(); i++){

			Enemy* pEnemy = (Enemy*)pEnemyArray->objectAtIndex(i);
			if (pEnemy->getRect().getMinX() < getRect().getMinX()){

				pEnemy->bow();
				//pEnemy->runAction(CCRemoveSelf::create());
			}
		}
	}
}

void Player::onLoad(){

	if(!isFoucs()){

		jumpTimes = 0;
		doRun();	
	}else{

		if(checkIsCanUpJump()){

			doUpJump();
		}else{
	
			if(jumpTimes > 0){
	
				if(!isInDown())
					doRun();

				if(isInUpJump())
					mapMoveTime = 1;

				jumpTimes = 0;
	
				if(isInBig() && GameScene::shareGameLayer()->getIsInGoldScene() == false){

					playSound("music/bigOnLoad.ogg");
					GameScene::shareGameLayer()->getMap()->runAction(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.08,ccp(0,-30)),2),CCEaseInOut::create(CCMoveBy::create(0.08,ccp(0,30)),2),NULL));
				}
			}
		}
	}
}

bool Gift::init(){

	char* pImgFile;
	int tag = 0;

	if(rand () % 200 < 199){

		pImgFile = "gold.png";
		tag = GOLDTAG;
	}else if(rand () % 2 == 0){

		pImgFile = "power.png";
		tag = POWERTAG;
	}else{

		pImgFile = "protect.png";
		tag = PROTECTTAG;
	}

	CCSprite::initWithSpriteFrameName(pImgFile);

	isFollowPlayer = false;
	isOrbit = false;

	setTag(tag);

	scheduleUpdate();

	setScale(0.7);

	return true;
}

bool Gift::initWithFile(const char *pszFilename){

	CCSprite::initWithFile(pszFilename);

	isOrbit = false;

	scheduleUpdate();

	return true;
}

void Gift::update(float delta){

	if(!isFollowPlayer){

		if(getRect().intersectsRect(Player::sharePlayer()->getRect())){

			beEat();

			return;
		}

		
		if(getParent()->getTag() == 3){  //来自tag为3的batch结点

			bool isInGoldScene = getParent()->getParent()->getParent()->getTag() != TMXTAG;
			int xInScreen = getPositionX() + getParent()->getParent()->getPositionX() -
				GameScene::shareGameLayer()->getMap()->getPositionX() * -1;

			if(xInScreen < 800 && !isOrbit){

				isOrbit = true;
				//runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,20)),2),CCEaseInOut::create(CCMoveBy::create(0.7,ccp(0,-20)),2),NULL)));
			}

			if(xInScreen < 600 && isInGoldScene == false){

				if(Player::sharePlayer()->isInMoney()){

					//stopAllActions();
					runAction(CCSequence::create(CCMoveTo::create(0.2,
						ccp(Player::sharePlayer()->getPosition().x - getParent()->getParent()->getPositionX(),Player::sharePlayer()->getPositionY() + PLAYERHEIGHT / 2)),
						CCCallFunc::create(this,callfunc_selector(Gift::beEat)),NULL));
					isFollowPlayer = true;
					prePlayerPoint = Player::sharePlayer()->getPosition();
				}
			}
			/*runAction(CCOrbitCamera::create(5,1, 0, 0, 180 * 5, 0, 0));*/
		}
	}else{

		CCPoint contentPoint = Player::sharePlayer()->getPosition();
		setPositionX(getPositionX() + (contentPoint.x - prePlayerPoint.x));
		setPositionY(getPositionY() + (contentPoint.y - prePlayerPoint.y) / 1.5);
		prePlayerPoint = contentPoint;
	}
}

void Gift::beEat(){

	if(getParent()->getTag() == 3)
		if((getParent()->getParent()->getParent()->getTag() == TMXTAG) == GameScene::shareGameLayer()->getIsInGoldScene()){
		
			runAction(CCRemoveSelf::create());
			return;
		}

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	CCSprite* pSprite = CCSprite::create();
	pSprite->runAction(CCSequence::create(getAnimate("eat%d.png",8,0.07),CCRemoveSelf::create(),NULL));
	pSprite->setPosition(getPosition());
	pSprite->setPositionX(getPositionX() + getParent()->getParent()->getPositionX() + (getParent()->getTag() == 3 && GameScene::shareGameLayer()->getIsInGoldScene() ? getParent()->getParent()->getParent()->getPositionX() : 0));
	pSprite->setZOrder(123);
	GameScene::shareGameLayer()->getMap()->addChild(pSprite);

	int tag = getTag();

	if(tag == GOLDTAG){

		playSound("music/gold.ogg");
		GameScene::shareUILayer()->addGold(1);
	}else if(tag == POWERTAG){

		playSound("music/gift.ogg");
		Player::sharePlayer()->doBig(10);
	}else if(tag == PROTECTTAG){

		Player::sharePlayer()->doInProtect();
	}else if(tag == FLYGOLDTAG){

		/*Player::sharePlayer()->doInMoney();*/

		/*Player::sharePlayer()->doFly();*/

		playSound("music/flyGold.ogg");

		addScoreShow("imgs/score20.png");
		GameScene::shareUILayer()->addGold(20);
	}else if(tag == KEY2TAG){

		playSound("music/flyGold.ogg");
		addGiftShow("imgs/key2Title.png");
		int contentKeyNum = pSava->getIntegerForKey("boss2Key");
		pSava->setIntegerForKey("boss2Key",contentKeyNum + 1);
	}else if(tag == KEY3TAG){

		playSound("music/flyGold.ogg");
		addGiftShow("imgs/key3Title.png");
		int contentKeyNum = pSava->getIntegerForKey("boss3Key");
		pSava->setIntegerForKey("boss3Key",contentKeyNum + 1);
	}else if(tag == KEY4TAG){

		playSound("music/flyGold.ogg");
		addGiftShow("imgs/key4Title.png");
		int contentKeyNum = pSava->getIntegerForKey("boss4Key");
		pSava->setIntegerForKey("boss4Key",contentKeyNum + 1);
	}else if(tag == GIFT1TAG){

		playSound("music/flyGold.ogg");
		addGiftShow("imgs/gift1Title.png");
		pSava->setBoolForKey("isGlass",true);
		Player::sharePlayer()->checkBody();
	}else if(tag == GIFT2TAG){

		playSound("music/flyGold.ogg");
		addGiftShow("imgs/gift2Title.png");
		pSava->setBoolForKey("isMoney",true);
		Player::sharePlayer()->checkBody();
	}

	removeFromParent();
}

void Gift::addScoreShow(char* fileName){

	CCSprite* pScore20 = CCSprite::create(fileName);
	pScore20->setAnchorPoint(ccp(0,0.5));
	pScore20->setPosition(getPosition());
	pScore20->setZOrder(1024);
	pScore20->runAction(CCSequence::create(CCMoveBy::create(1,ccp(50,30)),CCRemoveSelf::create(),NULL));
	pScore20->runAction(CCFadeOut::create(1));

	GameScene::shareGameLayer()->addChild(pScore20);
}

void Gift::addGiftShow(char* fileName){

	CCSprite* pGiftBoard = CCSprite::create("imgs/giftBoard.png");
	pGiftBoard->setAnchorPoint(ccp(0.5,0));
	pGiftBoard->setPosition(ccp(583,370));
	GameScene::shareGameLayer()->addChild(pGiftBoard);

	pGiftBoard->setPositionY(pGiftBoard->getPositionY() + 110);
	pGiftBoard->runAction(CCSequence::create(CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-110))),CCDelayTime::create(2),
		CCEaseBackIn::create(CCMoveBy::create(0.3,ccp(0,110))),CCRemoveSelf::create(),NULL));	

	CCSprite* pTitle = CCSprite::create(fileName);
	pTitle->setAnchorPoint(ccp(0,0));
	pGiftBoard->addChild(pTitle);
}

CCRect Gift::getRect(){

	if(getParent()->getTag() == 3){

		bool isInGoldScene = getParent()->getParent()->getParent()->getTag() != TMXTAG;
		return CCRect(getPositionX() + getParent()->getParent()->getPositionX() + (isInGoldScene ? getParent()->getParent()->getParent()->getPositionX() : 0),getPositionY(),getContentSize().width,getContentSize().height);
	}else{
		//is fly

		return CCRect(getPositionX() - GameScene::shareGameLayer()->getMap()->getPositionX(),getPositionY(),getContentSize().width,getContentSize().height);
	}
}

bool DrawLayer::init(){

	if(!CCLayer::init())	return false;

	return true;
}


void DrawLayer::draw(){

	if(!ISDRAW)
		return;

	CCLayer::draw();

	ccDrawColor4B(0, 0, 255, 255);
	glLineWidth(5);

	float mapX = GameScene::shareGameLayer()->getMap()->getPositionX();
	vector<CCRect> mapRect = GameScene::shareGameLayer()->getMoveRectVector();

	for(vector<CCRect>::iterator it = mapRect.begin();it != mapRect.end();it++){

		CCRect collRect = (CCRect)*it;
		ccDrawRect(ccp(collRect.getMinX() + mapX,collRect.getMinY()),ccp (collRect.getMaxX() + mapX,collRect.getMaxY()));	
	}
}

bool Enemy::init(){

	isAppear = false;
	isDie = false;
	isHaveAtt = false;
	isBow = false;
	isCanReJump = true;
	needStayY = -1;

	Human::init();	

	if(isGameMode1){

		pDoor = CCSprite::create("imgs/door.png");
		pDoor->setAnchorPoint(ccp(0.5,0));
		addChild(pDoor);
	}else{

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("run/Enemy40.png","run/Enemy40.plist","run/Enemy4.ExportJson");
		pBody = CCArmature::create("Enemy4");
		pBody->getAnimation()->playByIndex(1);
		pBody->setScale(0.3);
		pBody->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(Enemy::armatureCallBack));
		this->addChild(pBody,2);
	}

	pEnemyArray->addObject(this);

	scheduleUpdate();

	return true;
}

void Enemy::update(float delta){

	if(GameScene::shareGameLayer()->getIsInGoldScene())	return;

	float moveX = getPositionX() - 61 / 2 + getParent()->getPositionX() + getParent()->getParent()->getPositionX() - PLAYERSTARTX - 100;

	if(!isGameMode1 || (isGameMode1 && needStayY == -1))
		updateDrop(delta);
	else{
		
		if(moveX > 0){

			setPositionY(needStayY + moveX / 2);	
		}		
	}

	if(!isGameMode1){

		if(moveX < PLAYERSTARTX + 200 && isAppear == false){

			setVisible(true);
			isAppear = true;
			setPositionY(needStayY - 150);
			setDropSpeed(-20);
		}

		if(isDie)
			setPositionX(getPositionX() + 10);
		else if(isHaveAtt == false && getRect().intersectsRect(Player::sharePlayer()->getRect())){

			isHaveAtt = true;
			att();
		}
	}else{

		if(getMoveRect().intersectsRect(Player::sharePlayer()->getRect()) && 
			Player::sharePlayer()->isInBig()){

			die();
		}
	}
}

void Enemy::att(){

	pBody->getAnimation()->playByIndex(2);

	if(Player::sharePlayer()->isInBig() || Player::sharePlayer()->isInUpJump()){

		die();
	}else if(Player::sharePlayer()->isCanBeHurt()){

		Player::sharePlayer()->hurt();
		GameScene::shareGameLayer()->doShaky();
	}
}

void Enemy::onExit(){

	pEnemyArray->removeObject(this);
	Human::onExit();
}

void Enemy::die(){

	if(isDie)	return;

	if(isGameMode1){

		bow();
	}else{

		playSound("music/die.mp3");

		isDie = true;
		setDropSpeed(-12 - rand () % 4);
		pBody->getAnimation()->playByIndex(3);
	}
}

void Enemy::onLoad(){

	if(needStayY == -1){
	
		if(!isGameMode1){
			
			setVisible(false);
		}
		
		needStayY = getPositionY();
	}

	if(isDie)
		bow();
}

CCRect Enemy::getMoveRect(){

	if(isGameMode1){

		return CCRect(getPositionX() - 61 / 2 + getParent()->getPositionX(),
			getPositionY() + PLAYERHEIGHT * 0.6,61,1000);
	
	}else{

		return CCRect(getPositionX() - 61 / 2 + getParent()->getPositionX(),
			getPositionY(),61,104);
	}
}

CCRect Enemy::getRect(){

	return CCRect(getPositionX() - 61 / 2 + getParent()->getPositionX(),
		getPositionY(),61,104);
}

void Enemy::bow(){

	if(isBow)	return;

	taskNum[1] ++;

	playSound("music/bow.ogg");

	isBow = true;

	CCSprite* pSprite = CCSprite::create();
	pSprite->runAction(CCSequence::create(getAnimate("enemy bow (%d).png",17,0.07),CCRemoveSelf::create(),NULL));
	pSprite->setAnchorPoint(ccp(0.5,0.1));
	pSprite->setScale(1.5);
	pSprite->setZOrder(1024);
	pSprite->setPosition(getPosition());
	getParent()->addChild(pSprite);

	Gift* pGift = Gift::randomCreateFlyGift();
	
	pGift->setAnchorPoint(ccp(0,0));
	pGift->setPosition(getPosition());
	pGift->setPositionX(pGift->getPositionX() + getParent()->getPositionX() + getParent()->getParent()->getPositionX() + 50);
	pGift->setZOrder(1024);
	
	pGift->setRotation(-15);

	pGift->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCRotateBy::create(0.3,30),2),CCEaseInOut::create(CCRotateBy::create(0.3,-30),2),NULL)));
	pGift->runAction(CCSequence::create(CCEaseOut::create(CCMoveBy::create(1,ccp(400 + rand () % 300,0)),2),CCMoveBy::create(3,ccp(-1500,0)),CCRemoveSelf::create(),NULL));
	pGift->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.5,ccp(0,300)),2),CCEaseInOut::create(CCMoveBy::create(0.5,ccp(0,-300)),2),NULL)));

	GameScene::shareGameLayer()->addChild(pGift);

	runAction(CCRemoveSelf::create());
}

Gift* Gift::randomCreateFlyGift(){

	Gift* pGift = NULL;

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	if(mapIndex == 1){

		{

			pGift = Gift::createWithSpriteFrame("flyGold.png");
			pGift->setTag(FLYGOLDTAG);		
		}
	}else if(mapIndex == 2){

		{
			pGift = Gift::createWithSpriteFrame("flyGold.png");
			pGift->setTag(FLYGOLDTAG);		
		}
	}else if(mapIndex == 3){

		{
			pGift = Gift::createWithSpriteFrame("flyGold.png");
			pGift->setTag(FLYGOLDTAG);		
		}
	}else{

		{
			pGift = Gift::createWithSpriteFrame("flyGold.png");
			pGift->setTag(FLYGOLDTAG);		
		}
	}

	pGift->setScale(1.1);

	return pGift;
}

bool GameUILayer::init(){

	UILayer::init();

	contentScore = 0;
	contentGold = 0;
	contentPower = 0;
	isHaveUseDieFly = false;

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("GameUI/game.json"));

	pJump = dynamic_cast<UIButton*>(getWidgetByName("Button_27"));
	pJump->addTouchEventListener(this,toucheventselector(GameUILayer::callBack));

	pAtt = dynamic_cast<UIButton*>(getWidgetByName("Button_28"));
	pAtt->addTouchEventListener(this,toucheventselector(GameUILayer::callBack));

	pPause = dynamic_cast<UIButton*>(getWidgetByName("Button_157"));
	pPause->addTouchEventListener(this,toucheventselector(GameUILayer::callBack));

	pPower = dynamic_cast<UILoadingBar*>(getWidgetByName("LoadingBar_273"));
	pPowerNum = dynamic_cast<UILabelAtlas*>(getWidgetByName("LabelAtlas_275"));

	for(int i = 0;i < 3;i++){

		pShopShow[i] = dynamic_cast<UIImageView*>(getWidgetByName(CCString::createWithFormat("shopShow%d",i + 1)->getCString()));
		pShopShow[i]->addTouchEventListener(this,toucheventselector(GameUILayer::callBack));
		pShopShowPro[i] = dynamic_cast<UILoadingBar*>(pShopShow[i]->getChildByTag(1));
	}

	for(int i = 0;i < 4;i++){

		pIcon[i] = dynamic_cast<UIImageView*>(getWidgetByName(CCString::createWithFormat("icon%d",i + 1)->getCString()));
		pIcon[i]->setVisible(false);
	}

	pScore = dynamic_cast<UILabelAtlas*>(getWidgetByName("LabelAtlas_24"));

	pGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("gold"));

	if(!isGameMode1){

		pAtt->loadTextureNormal("GameUI/att.png");
		pAtt->loadTexturePressed("GameUI/attLock.png");
	}

	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isShowButton")){

		pJump->setVisible(false);
		pJump->setScale(2);
	
		pAtt->setVisible(false);
		pAtt->setScale(2);
	}

	startCheck();

	addChild(OldScoreBoard::create());
	schedule(schedule_selector(GameUILayer::deleteShowPro),1/30.0);

	return true;
}


bool StartShow::init(){

	MenuBoard::init();

	addBackgroundSprite("UI/startShow.png");

	return true;
}

void StartShow::doCancel(){

	pNode->startCheck();
}

bool StartUILayer::init(){

	UILayer::init();

	CCSprite* pBackground = CCSprite::create("signScene/background.jpg");
	pBackground->setAnchorPoint(ccp(0,0));
	pBackground->setZOrder(-1);
	addChild(pBackground);

	addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/start.json"));

	initUI();

	pPreButton = NULL;

	contentNeedGold = -1;

	isBuyFly = false;
	isBuyDieFly = false;
	isBuyTempMoney = false;

	checkBuyIcon();

	scheduleUpdate();

	callBack(pShoesBuy,TOUCH_EVENT_ENDED);
	Player::sharePlayer()->checkBody();

	return true;
}

void StartUILayer::initUI(){

	pStart = dynamic_cast<UIButton*>(getWidgetByName("Button_95"));
	pStart->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	//pCarBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy6"));
	//pCarBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pShoesBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97"));
	pShoesBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pGlassBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy0"));
	pGlassBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pMoneyBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy3"));
	pMoneyBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pGoldBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy1"));
	pGoldBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pFlyBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy2"));
	pFlyBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pDieFlyBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy4"));
	pDieFlyBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pTempMoneyBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_97_Copy5"));
	pTempMoneyBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pBuy = dynamic_cast<UIButton*>(getWidgetByName("Button_113"));
	pBuy->addTouchEventListener(this,toucheventselector(StartUILayer::callBack));

	pContentGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("LabelAtlas_114"));
	pNeedGold = dynamic_cast<UILabelAtlas*>(getWidgetByName("LabelAtlas_114_Copy0"));

	pShoesIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133"));
	pGlassIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133_Copy0"));
	pMoneyIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133_Copy1"));
	pFlyIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133_Copy2"));
	pDieFlyIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133_Copy3"));
	pTempMoneyIcon = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_133_Copy4"));

	pNeedType1 = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_155"));
	pNeedType2 = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_156"));

	UIImageView* pImage = dynamic_cast<UIImageView*>(getWidgetByName("ImageView_94"));

	pShoesIcon->setVisible(false);
	pGlassIcon->setVisible(false);
	pMoneyIcon->setVisible(false);
	pFlyIcon->setVisible(false);
	pDieFlyIcon->setVisible(false);
	pTempMoneyIcon->setVisible(false);

	pImage->setPositionY(pImage->getPositionY() + 480);
	pImage->runAction(CCSequence::create(CCDelayTime::create(0.5),CCEaseBackOut::create(CCMoveBy::create(0.3,ccp(0,-480))),NULL));

	pBuy->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.05,0.78),CCScaleTo::create(0.05,0.88),CCScaleTo::create(0.05,0.78),CCScaleTo::create(0.05,0.83),CCDelayTime::create(0.5),NULL)));
}

void GameUILayer::callBack(CCObject* pSender,TouchEventType type){

	if(type == TOUCH_EVENT_BEGAN){

		if(pSender == pJump){

			Player::sharePlayer()->doJump();
		}else if(pSender == pAtt){

			if(isGameMode1){

				Player::sharePlayer()->doDown();
			}else{

				Player::sharePlayer()->doAtt();
			}
		}else{

			for(int i = 0;i < 3;i++){

				if(pSender == pShopShow[i]){

					pShopShow[i]->runAction(CCScaleTo::create(0.1,0.9));
				}
			}
		}
	}else if(type == TOUCH_EVENT_ENDED){


		if(pSender == pPause){


			playSound("music/menu.mp3");

			if(CCDirector::sharedDirector()->isPaused())	return;

			GameScene::shareGameLayer()->addChild(PauseBoard::create());
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}else if(pSender == pShopShow[0]){


			playSound("music/menu.mp3");

			pShopShow[0]->stopAllActions();
			pShopShow[0]->setScale(0);
			
			Player::sharePlayer()->doFly(10);
			saveAndAdd("shopCount3",-1);

			pIcon[1]->setVisible(true);
			pIcon[1]->runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL),10));
		}else if(pSender == pShopShow[1]){


			playSound("music/menu.mp3");

			pShopShow[1]->stopAllActions();
			pShopShow[1]->setScale(0);

			Player::sharePlayer()->doInProtect();
			saveAndAdd("shopCount1",-1);

			pIcon[2]->setVisible(true);
			pIcon[2]->runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL),10000));
		}else if(pSender == pShopShow[2]){


			playSound("music/menu.mp3");

			pShopShow[2]->stopAllActions();
			pShopShow[2]->setScale(0);

			Player::sharePlayer()->doDieFly();
			saveAndAdd("shopCount2",-1);

			pIcon[3]->setVisible(true);
			pIcon[3]->runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL),10));
		}
	}
}

void GameUILayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

	UILayer::ccTouchEnded(pTouch,pEvent);

	if(Player::sharePlayer()->isInDown())	
		Player::sharePlayer()->doRun();
}

void GameUILayer::startCheck(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	
	char* pTempName[] = {"shopCount3","shopCount1"};
	for(int i = 0;i < 2;i++){

		int shopCount = pSava->getIntegerForKey(pTempName[i]);
	
		if(shopCount > 0){

			pShopShow[i]->setScale(1);
			pShopShowPro[i]->setPercent(100);			
		}else{

			pShopShow[i]->setScale(0);
		}
	}

	pShopShow[2]->setScale(0);

	if(pSava->getBoolForKey("equBuy1")){

		pIcon[0]->setVisible(true);
		pIcon[0]->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL)));
	}
}

bool GameUILayer::endCheck(){

	if(isHaveUseDieFly)	return false;

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int shopCount = pSava->getIntegerForKey("shopCount2");

	if(shopCount > 0){

		isHaveUseDieFly = true;

		pShopShow[2]->setScale(1);
		pShopShowPro[2]->setPercent(100);

		return true;
	}else{

		pShopShow[2]->setScale(0);

		return false;
	}
}

void GameUILayer::deleteShowPro(float dt){

	for(int i = 0;i < 3;i++){

		if(pShopShow[i]->getScale() > 0){

			int pro = pShopShowPro[i]->getPercent();
			if(pro > 0){

				pro --;
				pShopShowPro[i]->setPercent(pro);
			}else{

				pShopShow[i]->setScale(0);
			
				if(i == 2){

					GameScene::shareGameLayer()->addChild(ReliveBoard::create());
				}
			}
		}
	}
}

void StartUILayer::doBuy(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int contentGold = pSava->getIntegerForKey("contentGold");

	if(pPreButton == pCarBuy){

		pSava->setBoolForKey("isCar",true);
	}else if(pPreButton == pShoesBuy){

		pSava->setBoolForKey("isShoes",true);
	}else if(pPreButton == pMoneyBuy){

		pSava->setBoolForKey("isMoney",true);
	}else if(pPreButton == pGlassBuy){

		pSava->setBoolForKey("isGlass",true);
	}else if(pPreButton == pGoldBuy){

		pSava->setIntegerForKey("contentGold",contentGold + 2000);
	}else if(pPreButton == pFlyBuy && !isBuyFly){

		if(contentNeedGold <= contentGold){
			
			isBuyFly = true;
			pSava->setIntegerForKey("contentGold",contentGold - contentNeedGold);
		}else{

			addChild(MoreGoldBoard::create());
		}
	}else if(pPreButton == pDieFlyBuy && !isBuyDieFly){

		if(contentNeedGold <= contentGold){

			isBuyDieFly = true;
			pSava->setIntegerForKey("contentGold",contentGold - contentNeedGold);
		}else{

			addChild(MoreGoldBoard::create());
		}
	}else if(pPreButton == pTempMoneyBuy && !isBuyTempMoney){

		if(contentNeedGold <= contentGold){

			isBuyTempMoney = true;
			pSava->setIntegerForKey("contentGold",contentGold - contentNeedGold);
		}else{

			addChild(MoreGoldBoard::create());
		}
	}

	Player::sharePlayer()->checkBody();
	checkBuyIcon();
}

void StartUILayer::startCheck(){

	pStart->setEnabled(false);
	runAction(CCEaseBackIn::create(CCScaleTo::create(0.3,0)));

	Player* pPlayer = Player::sharePlayer();

	pPlayer->checkBody();

	if(pPlayer->isInFly()){

		pPlayer->flyDone();
	}

	if(isBuyFly){

		pPlayer->doFly(10);
	}

	if(isBuyDieFly){

		pPlayer->setIsDieFly(true);
	}

	if(isBuyTempMoney){

		pPlayer->setMoneyTimes(2);
	}
}

void StartUILayer::checkBuyIcon(){

	playSound("music/doBuy.mp3");

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	
	//if(pSava->getBoolForKey("isCar") && !pCarBuy->getChildByTag(1024)->isVisible()){

	//	//pCarBuy->getChildByTag(1025)->setVisible(false);
	//	//pCarBuy->getChildByTag(1024)->setVisible(true);
	//}

	if(pSava->getBoolForKey("isShoes") && !pShoesIcon->isVisible()){

		pShoesIcon->setVisible(true);
		pShoesIcon->setScale(0);
		pShoesIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));
		
		pShoesBuy->getChildByTag(1025)->setVisible(false);
		pShoesBuy->getChildByTag(1024)->setVisible(true);
	}
	
	if(pSava->getBoolForKey("isMoney") && !pMoneyIcon->isVisible()){

		pMoneyIcon->setVisible(true);
		pMoneyIcon->setScale(0);
		pMoneyIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));
		
		pMoneyBuy->getChildByTag(1025)->setVisible(false);
		pMoneyBuy->getChildByTag(1024)->setVisible(true);
	}
	
	if(pSava->getBoolForKey("isGlass") && !pGlassIcon->isVisible()){

		pGlassIcon->setVisible(true);	
		pGlassIcon->setScale(0);
		pGlassIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));

		pGlassBuy->getChildByTag(1025)->setVisible(false);
		pGlassBuy->getChildByTag(1024)->setVisible(true);
	}

	if(isBuyFly && !pFlyIcon->isVisible()){

		pFlyIcon->setVisible(true);
		pFlyIcon->setScale(0);
		pFlyIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));

		pFlyBuy->getChildByTag(1024)->setVisible(true);
	}

	if(isBuyDieFly && !pDieFlyIcon->isVisible()){

		pDieFlyIcon->setVisible(true);
		pDieFlyIcon->setScale(0);
		pDieFlyIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));

		pDieFlyBuy->getChildByTag(1024)->setVisible(true);
	}

	if(isBuyTempMoney && !pTempMoneyIcon->isVisible()){

		pTempMoneyIcon->setVisible(true);
		pTempMoneyIcon->setScale(0);
		pTempMoneyIcon->runAction(CCEaseBackOut::create(CCScaleTo::create(0.3,1)));

		pTempMoneyBuy->getChildByTag(1024)->setVisible(true);
	}
}

void StartUILayer::update(float dt){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int contentGoldNum = pSava->getIntegerForKey("contentGold");
	pContentGold->setStringValue(CCString::createWithFormat("%d",contentGoldNum)->getCString());
}

void StartUILayer::callBack(CCObject* pSender,TouchEventType type){

	Player* pPlayer = Player::sharePlayer();

	if(type == TOUCH_EVENT_ENDED){

		if(MenuBoard::shareContentMenu() != NULL)
			return;

		//Player::sharePlayer()->checkBody();

		if(pPreButton != NULL)
			pPreButton->setBright(true);

		if(pSender == pStart){

			/*runAction(CCMoveBy::create(0.5,ccp(-800,0)));*/
			
			if(isBuyFly || isBuyDieFly || isBuyTempMoney){

				startCheck();
			}else{

				StartShow* pShow = StartShow::create();
				pShow->setStartUILayer(this);

				addChild(pShow);
			}
		}else if(pSender == pSet){
			
		}else if(pSender == pCarBuy){

			if(pPlayer->isOnCar()){

				pPlayer->doUnOnCar();
			}else{

				pPlayer->doOnCar();
			}

			pPreButton = pCarBuy;
			contentNeedGold = 2;
		}else if(pSender == pShoesBuy){

			if(pPlayer->isInShoes())
				pPlayer->doUnTakeShoes();
			else	
				pPlayer->doTakeShoes();
			
			pPreButton = pShoesBuy;
			contentNeedGold = 2;
		}else if(pSender == pMoneyBuy){

			if(pPlayer->isInMoney())
				pPlayer->doUnInMoney();
			else
				pPlayer->doInMoney();

			pPreButton = pMoneyBuy;
			contentNeedGold = 2;
		}else if(pSender == pGlassBuy){

			if(pPlayer->isInGlass())
				pPlayer->doUnTakeGlass();
			else
				pPlayer->doTakeGlass();

			pPreButton = pGlassBuy;
			contentNeedGold = 2;
		}else if(pSender == pGoldBuy){

			pPreButton = pGoldBuy;
			contentNeedGold = 2;
		}else if(pSender == pFlyBuy){

			pPreButton = pFlyBuy;
			contentNeedGold = 1000;

			if(!pPlayer->isInFly())
				pPlayer->doFly(10);
			else
				pPlayer->flyDone();
		}else if(pSender == pDieFlyBuy){
			
			pPreButton = pDieFlyBuy;
			contentNeedGold = 1000;
		}else if(pSender == pTempMoneyBuy){

			pPreButton = pTempMoneyBuy;
			contentNeedGold = 1000;
		}else if(pSender == pBuy){

			doBuy();
		}

		if(contentNeedGold != -1)
			pNeedGold->setStringValue(CCString::createWithFormat("%d",contentNeedGold)->getCString());
		if(pPreButton != NULL)
			pPreButton->setBright(false);
		
		if(contentNeedGold < 100){

			pNeedType1->setVisible(true);
			pNeedType2->setVisible(false);
		}else{

			pNeedType1->setVisible(false);
			pNeedType2->setVisible(true);
		}		
	}else if(type == TOUCH_EVENT_BEGAN){

		if(pSender != pStart)
			playSound("music/menu.mp3");
		else
			playSound("music/bow.ogg");
	}
}

void GameUILayer::addScore(int score){

	contentScore += score;
	pScore->setStringValue(CCString::createWithFormat("%d",getMetle())->getCString());
}

void GameUILayer::addGold(int gold){

	contentGold += gold * Player::sharePlayer()->getMoneyTimes();
	pGold->setStringValue(CCString::createWithFormat("%d",contentGold)->getCString());

	addPower(1 * 3);
}

void GameUILayer::addPower(int gold){

	if(GameScene::shareGameLayer()->getIsInGoldScene())	return;

	contentPower += gold / 5.0;	

	if(contentPower >= 100){

		contentPower = 0;
		/*Player::sharePlayer()->doFly(10);
		GameScene::shareGameLayer()->doLight();*/
		GameScene::shareGameLayer()->changeInGold();
	}

	pPower->setPercent(contentPower);

	pPowerNum->setStringValue(CCString::createWithFormat("%d",(int)contentPower)->getCString());
}

bool OldScoreBoard::init(){

	CCSprite::initWithFile("imgs/newScore.png");

	isHaveStartAction = false;

	pScore = CCLabelAtlas::create("0","GameUI/num1.png",28,46,'0');
	pScore->setAnchorPoint(ccp(0.5,0.5));
	pScore->setPosition(ccp(129,152));
	//pScore->setScale();
	addChild(pScore);

	setAnchorPoint(ccp(0.5,0.5));
	setPosition(ccp(742,302));

	bestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("bestLenth");

	setScale(0.5);
	scheduleUpdate();

	runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(1,ccp(-20,0)),2),CCEaseInOut::create(CCMoveBy::create(1,ccp(20,0)),2),NULL)));

	setOpacity(150);
	pScore->setOpacity(150);

	return true;
}

void OldScoreBoard::update(float dt){

	int contentScore = GameScene::shareUILayer()->getMetle();
	int nextScore = bestScore - contentScore;
	if(nextScore >= 0)
		pScore->setString(CCString::createWithFormat("%d",nextScore)->getCString());
	else{

		pScore->setString(CCString::createWithFormat("%d",0)->getCString());
		setPositionX(getPositionX() - MAPSPEED);
		
		if(!isHaveStartAction){

			isHaveStartAction = true;
			runAction(CCRotateBy::create(0.2,360));
			runAction(CCScaleTo::create(0.2,1));
			taskNum[2] ++;
		}
	}
}

bool ReliveBoard::init(){

	CCLayer::init();

	CCSprite* pReliveBoard = CCSprite::create("GameUI/relive.png");
	pReliveBoard->setAnchorPoint(ccp(0.5, 0.5));
	pReliveBoard->setPosition(ccp(400, 240));
	addChild(pReliveBoard);

	pMenu = CCMenu::create();
	pMenu->setPosition(ccp(0, 0));
	pReliveBoard->addChild(pMenu);

	pCancer = CCMenuItemImage::create("GameUI/menuClose.png", "GameUI/menuCloseLock.png", this, menu_selector(ReliveBoard::call_back));
	pCancer->setAnchorPoint(ccp(0.5, 0.5));
	pCancer->setPosition(ccp(525, 345));
	pMenu->addChild(pCancer);

	pSure = CCMenuItemImage::create("GameUI/menuSure.png", "GameUI/menuSureLock.png", this, menu_selector(ReliveBoard::call_back));
	pSure->setAnchorPoint(ccp(0.5, 0.5));
	pSure->setPosition(ccp(270, 40));
	pSure->setScale(0.8f);
	pMenu->addChild(pSure);

	setZOrder(99981);

	setScale(0.7);
	/*setVisible(false);
	runAction(CCSequence::create(CCHide::create(),CCDelayTime::create(1),CCShow::create(),CCEaseBackOut::create(CCScaleTo::create(0.15, 0.7)),NULL));*/

	return true;
}

void ReliveBoard::call_back(CCObject* pSender){

	playSound("music/menu.mp3");

	if (pSender == pSure){

		if(CCUserDefault::sharedUserDefault()->getIntegerForKey("contentMoney") < 20){

			addChild(MoreMoney::create());
		}else{


			saveAndAdd("contentMoney",-20);
			Player::sharePlayer()->doRelive();
		
			pMenu->setEnabled(false);
			runAction(CCSequence::create(CCScaleTo::create(0.2, 0.3), CCRemoveSelf::create(), NULL));
		}
	}else{
	
		//true over
		if(Player::sharePlayer()->isDieFly()){

			/*GameScene::gameOver();*/			
			Player::sharePlayer()->doDieFly();
		}else{

			GameScene::gameOver();
		}

		pMenu->setEnabled(false);
		runAction(CCSequence::create(CCScaleTo::create(0.2, 0.3), CCRemoveSelf::create(), NULL));
	}
}

//购买二次确认
bool PopupLayer::init()
{
	CCLayer::init();

	CCDirector* pDirector = CCDirector::sharedDirector();
	CCSize s = pDirector->getWinSize();
	layer = CCLayerColor::create(ccc4(0, 0, 0, 100), s.width, s.height);
	layer->setAnchorPoint(ccp(0.5, 0.5));
	addChild(layer);

	pBackground = CCSprite::create("GameUI/tc1.png");
	pBackground->setAnchorPoint(ccp(0.5,0.5));
	pBackground->setPosition(ccp(400, 240));
	layer->addChild(pBackground);

	pConfirm = CCMenuItemImage::create("GameUI/tc2.png", "GameUI/tc2.png", "GameUI/tc2.png",
		this, menu_selector(PopupLayer::call_back));
	pConfirm->setAnchorPoint(ccp(0.5, 0.5));
	pConfirm->setPosition(ccp(165, 32));

	pCancel = CCMenuItemImage::create("GameUI/confirmNo.png", "GameUI/confirmNo.png", "GameUI/confirmNo.png",
		this, menu_selector(PopupLayer::call_back));
	pCancel->setAnchorPoint(ccp(0.5, 0.5));
	pCancel->setPosition(ccp(315, 150));

	pMenu = CCMenu::create(pConfirm, pCancel, NULL);
	pMenu->setPosition(ccp(0, 0));
	pMenu->setZOrder(1);
	pBackground->addChild(pMenu);
	pMenu->setTouchPriority(-190);

	pDirector->getTouchDispatcher()->addTargetedDelegate(this,-160, true);

	setZOrder(99981);

	return true;
}

bool PopupLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return true;
}


void PopupLayer::onExit(){

	CCLayer::onExit();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	
}

void PopupLayer::call_back(CCObject* pSender)
{
	playSound("music/menu.mp3");

	if (pSender == pConfirm){

		doSure();
	}
	else{

		doCancel();
	}

	pMenu->setEnabled(false);

	runAction(CCRemoveSelf::create());
}


bool PauseBoard::init(){

	CCLayer::init();

	pCanSee = CCSprite::create("imgs/canSee.png");
	pCanSee->setAnchorPoint(ccp(0.5,0.5));
	pCanSee->setPosition(ccp(400,240));
	pCanSee->setScale(12);
	addChild(pCanSee);

	CCSprite* pReliveBoard = CCSprite::create("GameUI/pauseBoard.png");
	pReliveBoard->setAnchorPoint(ccp(0.5, 0.5));
	pReliveBoard->setPosition(ccp(400, 240));
	addChild(pReliveBoard);

	pMenu = CCMenu::create();
	pMenu->setPosition(ccp(0, 0));
	pReliveBoard->addChild(pMenu);

	pCancer = CCMenuItemImage::create("GameUI/pauseMenu2.png", "GameUI/pauseMenu2Lock.png", this, menu_selector(PauseBoard::call_back));
	pCancer->setAnchorPoint(ccp(0.5, 0.5));
	pCancer->setPosition(ccp(274, 200));
	pMenu->addChild(pCancer);

	pSure = CCMenuItemImage::create("GameUI/pauseMenu1.png", "GameUI/pauseMenu1Lock.png", this, menu_selector(PauseBoard::call_back));
	pSure->setAnchorPoint(ccp(0.5, 0.5));
	pSure->setPosition(ccp(274, 87));
	pMenu->addChild(pSure);

	CCDirector::sharedDirector()->pause();
	CCDirector::sharedDirector()->setAnimationInterval(1 / 60.0);

	setZOrder(99981);

	setScale(0.7);
	//runAction(CCEaseBackOut::create(CCScaleTo::create(0.2, 0.7)));

	return true;
}

void PauseBoard::call_back(CCObject* pSender){

	playSound("music/menu.mp3");

	CCDirector::sharedDirector()->resume();

	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	
	if (pSender != pSure){

	}else{
		MainBoard* mainBoard = MainBoard::create();
		//  
		CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(0.2, getSceneWithLayer(mainBoard)));
	}

	pMenu->setEnabled(false);
	pCanSee->runAction(CCFadeOut::create(0.2));
	runAction(CCSequence::create(CCScaleTo::create(0.1, 0.6), CCRemoveSelf::create(), NULL));
}

bool UpBox::init(){

	Human::init();
	
	CCSprite::initWithFile("imgs/UpBox.png");

	setAnchorPoint(ccp(0.5,0));

	pUpBoxArray->addObject(this);

	scheduleUpdate();

	runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.3,1,0.8),CCScaleTo::create(0.3,1,1),NULL)));

	return true;
}

void UpBox::onExit(){

	CCSprite::onExit();

	pUpBoxArray->removeObject(this);
}

void UpBox::update(float dt){

	updateDrop(dt);
}

bool Fire::init(){

	CCSprite::initWithFile("imgs/fire.png");

	playSound("music/warn.ogg");

	setAnchorPoint(ccp(0,0.16));

	CCSprite* pWarnning = CCSprite::create("imgs/warning.png");
	pWarnning->setAnchorPoint(ccp(1,0.1));
	pWarnning->setPositionY(40);
	pWarnning->setVisible(false);
	addChild(pWarnning);

	pWarnning->runAction(CCSequence::create(CCBlink::create(1,2),CCHide::create(),CCRemoveSelf::create(),NULL));

	setPositionX(800);
	setPositionY(Player::sharePlayer()->getPositionY() + PLAYERHEIGHT * 0.6 - 20);
	setZOrder(999);

	runAction(CCSequence::create(CCDelayTime::create(2),CCMoveBy::create(2,ccp(-1300,0)),CCRemoveSelf::create(),NULL));

	scheduleUpdate();
	schedule(schedule_selector(Fire::addShow),0.1);

	return true;
}

void Fire::addShow(float dt){

	CCSprite* pShow = CCSprite::create("imgs/fireShow.png");
	pShow->setAnchorPoint(ccp(0.5,0.5));
	pShow->setPosition(ccp(85,30));
	pShow->setZOrder(111);
	pShow->runAction(CCScaleTo::create(0.5,2));
	pShow->runAction(CCMoveBy::create(0.5,ccp(200,-rand () % 100 + 50)));
	pShow->runAction(CCSequence::create(CCFadeOut::create(0.5),CCRemoveSelf::create(),NULL));
	addChild(pShow);
}

void Fire::update(float dt){

	//check is in gold scene
	if(GameScene::shareGameLayer()->getIsInGoldScene()){

		unscheduleUpdate();
		runAction(CCRemoveSelf::create());
		return;
	}

	if(getRect().intersectsRect(Player::sharePlayer()->getRect())){

		if(Player::sharePlayer()->isInBig() || Player::sharePlayer()->isInUpJump() || Player::sharePlayer()->isInFly()){

		}else{

			if(Player::sharePlayer()->isInDie() == false){

				Player::sharePlayer()->hurt();
				GameScene::shareGameLayer()->doShaky();

				unscheduleUpdate();
				stopAllActions();
				runAction(CCSequence::create(CCDelayTime::create(0.7),CCRemoveSelf::create(),NULL));
			}
		}	
	}
}

void Boss::stay(){

	pBody->getAnimation()->playByIndex(1);
	setPosition(ccp(750,100));
	isDie = false;
	bloodNum = 0;
}

bool Boss::init(){

	CCNode::init();

	bloodNum = 0;
	isDie = false;

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("run/Enemy40.png","run/Enemy40.plist","run/Enemy4.ExportJson");
	pBody = CCArmature::create("Enemy4");
	pBody->getAnimation()->playByIndex(1);
	/*pBody->setScale(0.3);*/
	pBody->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(Boss::armatureCallBack));
	addChild(pBody,2);

	setPosition(ccp(750,100));

	return true;
}

void Boss::armatureCallBack(CCArmature* pArmature, MovementEventType type, const char* str){

	switch (type)
	{
	case cocos2d::extension::START:
		break;
	case cocos2d::extension::COMPLETE:

	case cocos2d::extension::LOOP_COMPLETE:

		if(strcmp(str,"hurt") == 0){

			pBody->getAnimation()->playByIndex(1);
		}
		break;
	default:
		break;
	}
}

void Boss::hurt(){

	if(isGameMode1 || !GameScene::shareGameLayer()->getIsInGoldScene())	return;

	bloodNum ++;

	if(bloodNum > 30 && !isDie){

		isDie = true;
		pBody->getAnimation()->playByIndex(3);
		runAction(CCJumpBy::create(1,ccp(400,0),250,1));
	}else{

		pBody->getAnimation()->playByIndex(3);

		Gift* pGift = Gift::randomCreateFlyGift();

		pGift->setAnchorPoint(ccp(0,0));
		pGift->setPosition(getPosition());	
		pGift->setZOrder(1024);
		pGift->setRotation(-15);

		pGift->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCRotateBy::create(0.3,30),2),CCEaseInOut::create(CCRotateBy::create(0.3,-30),2),NULL)));
		pGift->runAction(CCSequence::create(CCEaseOut::create(CCMoveBy::create(0.2,ccp(rand () % 500,0)),2),CCMoveBy::create(3,ccp(-1500,0)),CCRemoveSelf::create(),NULL));
		pGift->runAction(CCRepeatForever::create(CCSequence::create(CCEaseInOut::create(CCMoveBy::create(0.5,ccp(0,300)),2),CCEaseInOut::create(CCMoveBy::create(0.5,ccp(0,-300)),2),NULL)));

		GameScene::shareGameLayer()->addChild(pGift);
	}
}

bool PlayerSprite::init(){

	Human::init();
	CCSprite::initWithFile("imgs/sprite.png");

	setAnchorPoint(ccp(0.5,0));
	setPosition(ccp(400,400));

	scheduleUpdate();

	isCanReJump = true;

	return true;
}

CCRect PlayerSprite::getRect(){

	return CCRect(getPositionX() - SPRITEWIDTH,getPositionY(),SPRITEWIDTH,SPRITEHEIGHT);
}

void PlayerSprite::update(float dt){

	//掉落
	updateDrop(dt);
	
	//move
	setPositionX(getPositionX() + MAPSPEED);

	if(isNeedJump()){
	
		doJump();
	}

	if(getPositionY() < 40 && dropSpeed > 2){

		doJump();
	}
}

void PlayerSprite::doJump(){

	setDropSpeed(JUMP1POWER);

	CCSprite* pLight = CCSprite::create();
	pLight->setPosition(getPosition());
	pLight->setScale(0.5);
	pLight->runAction(CCSequence::create(getAnimate("jump_light%d.png",7,0.1),CCRemoveSelf::create(),NULL));
	GameScene::shareGameLayer()->getMap()->addChild(pLight);
}