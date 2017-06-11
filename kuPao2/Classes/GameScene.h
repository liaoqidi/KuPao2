#pragma once

#include "cocos-ext.h"
#include "cocos2d.h"
#include "MainScene.h"
#include "vector"

using namespace cocos2d::gui;
using namespace cocos2d::extension;
using namespace cocos2d;
using namespace std;

class GameScene;
class BackgroundLayer; 
class GameLayer;
class Human;
class Player;
class Gift;
class DrawLayer;
class Enemy;
class GameUILayer;
class StartUILayer;
class ReliveBoard;
class PopupLayer;
class PauseBoard;
class OldScoreBoard;  
class UpBox; 
class Fire;
class Boss;
class PlayerSprite;


enum STATE{

	RUN,
	JUMP,
	ATT,
	FLY,
	DOWN
};

class GameScene : public CCScene{

	GameLayer* pGameLayer;
	GameUILayer* pGameUILayer;
	BackgroundLayer* pBackgroundLayer;
	StartUILayer* pStartUILayer;
public:

	bool init();
	static void gameOver();
	static GameLayer* shareGameLayer();
	static GameScene* shareGameScene();
	static GameUILayer* shareUILayer();
	static BackgroundLayer* shareBackgroundLayer();
	static void checkEveryDayTaskData();
	static void saveEveryDayTaskData();
	CREATE_FUNC(GameScene);
};

class BackgroundLayer : public CCLayer{

	CCNode* pBackground[2];
	CCSpriteBatchNode* pRootBackground;
	int bgWidth;
	int rootBgWidth;
public:

	bool init();
	void move();
	CREATE_FUNC(BackgroundLayer);
};

class GameLayer : public CCLayer{

	Player* pPlayer;
	Player* pPlayerNext;
	CCNode* pMapNode;	
	vector<CCRect> rectVector;
	vector<CCRect> moveRectVector;
	int mapCount;
	CCNode* pFirstMap;
	CCSpriteFrameCache* pCache;
	CCSprite* pCloud;
	CCSprite* pHurtLight;
	CCNode* pGoldScene;
	Boss* pBoss;

	bool isInGoldScene;
public:

	bool init();
	void createMap();
	void createGoldMap();
	void createRect();
	void createEnemy(CCTMXTiledMap* pMap);
	void createGold(CCTMXTiledMap* pMap);
	void createUp(CCTMXTiledMap* pMap);
	void update(float dt);
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void doShaky();
	void doLight(float time = 0.5);
	void clearEnemyInScreen();
	void addHurtLight();
	void low();
	void up();
	void changeInGold();
	void changeOutGold();	
	void resetGoldSceneGold(bool isStart = true);
	void setCloudAlpha(bool isInGlass);
	void addSpeedLine(float dt);	
	void createFire(float dt);
	bool getIsInGoldScene(){return isInGoldScene;};
	bool changePlayer();
	void removeNextPlayer();
	void addNextPlayer(int personIndex);
	Boss* getBoss(){return pBoss;};
	CCNode* getMap();
	vector<CCRect> getRectVector();
	vector<CCRect> getMoveRectVector();
	CREATE_FUNC(GameLayer);
};

class OldScoreBoard : public CCSprite{

	CCLabelAtlas* pScore;
	int bestScore;
	bool isHaveStartAction;
public:

	bool init();
	void update(float dt);
	CREATE_FUNC(OldScoreBoard);
};

class Human : public CCSprite{

private:

public:

	bool init();
	void updateDrop(float dt);
	void updateMove(float speed);
	bool isNeedJump();
	void setDropSpeed(float speed);
	void setIsCanAddSpeed(bool isCanAddSpeed);
	virtual void onLoad();
	virtual CCPoint getStandPosition();
	virtual CCRect getRect() = 0;
protected:

	bool isCanSqu;
	bool isCanAddSpeed;
	bool isCanReJump;
	float dropSpeed;
};

class Player : public Human{

private:

	CCArmature* pBody;
	STATE state;
	CCNode* pLightNode;
	CCSprite* pFly;
	CCSprite* pProtect; 
	int bloodNum;
	int jumpTimes;
	
	bool isMoney;
	bool isDie;
	bool isShoes;
	bool isGlass;
	bool mIsOnDieFly;
	bool mIsDieFly;
	bool mIsOnCar;
	bool mIsOnProtect;
	float mMoneyTimes;
public:

	bool init();
	void doRun();
	void doJump();
	void doAtt();
	void doDown();
	void doUnDown();
	void doFly(int flyTime);
	void doBig(int bigTime);	
	void doDie();

	void doDieFly(){pBody->setVisible(true);isDie = false;mIsOnDieFly = true;doFly(10);};
	void setIsDieFly(bool IsDieFly){mIsDieFly = IsDieFly;};
	bool isDieFly(){return mIsDieFly;};
	bool isOnDieFly(){return mIsOnDieFly;};

	void setMoneyTimes(float moneyTimes){mMoneyTimes = moneyTimes;};
	float getMoneyTimes(){return mMoneyTimes;};

	int getBloodNum(){return bloodNum;};
	void hurt();

	void doRelive();
	void doTakeGlass();
	void doTakeShoes();
	void doInMoney();
	void doOnCar();
	void doInProtect();
	void doBow();

	void checkBody();

	void doUnTakeGlass();
	void doUnTakeShoes();
	void doUnInMoney();
	void doUnOnCar();

	void doUpJump();

	void addTakeLight();

	void initBody();
	void onLoad();
	
	void toolDoneAndStart();
	//
	void update(float delta);
	void armatureCallBack(CCArmature*, MovementEventType, const char *);
	void frameEventCallBack(CCBone *, const char *, int, int);
	int getMaxJumpTimes(){return isShoes ? 3 : 2;};
	void draw();
	void flyDone();
	void createRoot();
	bool checkIsCanUpJump();
	void fightEnemys();
	void fightEnemysOnCar();

	void preBody(char* pFileName,int manIndex,CCPoint pos,float rot);

	bool isCanBeHurt();
	bool isInatt(){return state == ATT;};
	bool isInFly(){return state == FLY;};
	bool isOnCar(){return mIsOnCar;};
	bool isInDie(){ return isDie;};
	bool isInShoes(){return isShoes;};
	bool isInGlass(){return isGlass;};
	bool isInMoney(){return isMoney || state == FLY;};
	bool isInDown(){return state == DOWN;};
	bool isInBig(){return getScale() > 1;};
	bool isInProtect(){return mIsOnProtect;};
	bool isInUpJump(){return jumpTimes >= 99;};

	void setFoucs();
	bool isFoucs();

	CCPoint getPointInScreen(){return ccp(getPositionX() + getParent()->getPositionX(),getPositionY());};
	CCRect getFightRect();
	CCRect getRect();
	CREATE_FUNC(Player);
	static Player* sharePlayer();
};

class Gift : public CCSprite{

private:

	bool isOrbit;
	bool isFollowPlayer;
	CCPoint prePlayerPoint;
public:

	bool init();
	bool initWithFile(const char *pszFilename);
	void update(float delta);
	void beEat();
	void addScoreShow(char* fileName);
	void addGiftShow(char* fileName);
	CCRect getRect();
	CREATE_FUNC(Gift);
	
	static Gift* createWithSpriteFrame(const char *pszFilename){

		Gift* pRet = new Gift();
		pRet->init();
		pRet->setDisplayFrame(CCSpriteFrameCache::
			sharedSpriteFrameCache()->spriteFrameByName(pszFilename));
		pRet->autorelease();

		return pRet;
	}

	static Gift* randomCreateFlyGift();
};

class Fire : public CCSprite{

public:

	bool init();
	void update(float dt);
	void addShow(float dt);
	CCRect getRect(){return CCRect
		(getPositionX() - GameScene::shareGameLayer()->getMap()->getPositionX() + 20,
		getPositionY(),40,40);};
	CREATE_FUNC(Fire);
};

class Enemy : public Human{

private:

	//isGameMode1 == false	then set this visible
	CCArmature* pBody;
	//else then choose this
	CCSprite* pDoor;

	bool isDie;
	bool isBow;
	bool isHaveAtt;

	float needStayY;
	bool isAppear;
public:

	bool init();
	void update(float delta);
	void onExit();
	void att();
	void die();
	void onLoad();
	void bow();
	
	void armatureCallBack(CCArmature*, MovementEventType, const char *);
	
	bool isInDie(){return isDie;};

	CCPoint getStandPosition(){return ccp(getPositionX() + getParent()->getPositionX(),getPositionY());}
	CCRect getRect();
	CCRect getMoveRect();

	CREATE_FUNC(Enemy);
};

class DrawLayer : public CCLayer{

public:

	bool init();
	void draw();
	CREATE_FUNC(DrawLayer);
};

class GameUILayer : public UILayer{

	UIButton* pJump;
	UIButton* pAtt;
	UIButton* pPause;
	UILabelAtlas* pScore;
	UILabelAtlas* pGold;
	int contentScore;
	int contentGold;
	UILoadingBar* pPower;
	UILabelAtlas* pPowerNum;

	UIImageView* pShopShow[3];
	UILoadingBar* pShopShowPro[3];

	UIImageView* pIcon[4];

	float contentPower;
	bool isHaveUseDieFly;
public:

	bool init();
	void callBack(CCObject* pSender,TouchEventType type);
	void addScore(int score);
	int getGold(){return contentGold;};
	int getMetle(){return contentScore / 50;};
	void addGold(int gold);
	void addPower(int goldNum);
	
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void startCheck();
	bool endCheck();
	void deleteShowPro(float dt);

	void protectBrken(){pIcon[2]->setVisible(false);};

	CREATE_FUNC(GameUILayer);
};

class StartShow : public MenuBoard{

	StartUILayer* pNode;
public:

	bool init();
	void doSure(){};
	void doCancel();
	void setStartUILayer(StartUILayer* pNode){this->pNode = pNode;};
	CREATE_FUNC(StartShow);
};

class StartUILayer : public UILayer{

	UIButton* pStart;
	UIButton* pSet;
	UIButton* pBuy;

	UIButton* pCarBuy;
	UIButton* pShoesBuy;
	UIButton* pGlassBuy;
	UIButton* pGoldBuy;
	UIButton* pMoneyBuy;
	UIButton* pFlyBuy;
	UIButton* pDieFlyBuy;
	UIButton* pTempMoneyBuy;

	UIButton* pPreButton;

	UILabelAtlas* pContentGold;
	UILabelAtlas* pNeedGold;
	
	UIImageView* pNeedType1;
	UIImageView* pNeedType2;

	UIImageView* pShoesIcon;
	UIImageView* pGlassIcon;
	UIImageView* pMoneyIcon;
	UIImageView* pFlyIcon;
	UIImageView* pDieFlyIcon;
	UIImageView* pTempMoneyIcon;

	int contentNeedGold;
	bool buyType;
	
	bool isBuyFly;
	bool isBuyDieFly;
	bool isBuyTempMoney;
public:

	bool init();
	void callBack(CCObject* pSender,TouchEventType type);
	void update(float dt);
	void doBuy();
	void initUI();
	void checkBuyIcon();
	void startCheck();
	CREATE_FUNC(StartUILayer);
};

class ReliveBoard : public CCLayer{

	CCMenu* pMenu;
	CCMenuItemImage* pSure;
	CCMenuItemImage* pCancer;
public:
	bool init();
	void call_back(CCObject* pSender);
	CREATE_FUNC(ReliveBoard);
};

class PopupLayer : public CCLayer{
	CCLayerColor* layer;
	CCSprite* pBackground;
	CCMenuItemImage* pConfirm;
	CCMenuItemImage* pCancel;
	CCMenu* pMenu;
	CCSprite* pLight;
public:
	bool init();
	void onExit();
	void call_back(CCObject* pSender);
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void doSure() = 0;
	virtual void doCancel() = 0;
};

class PauseBoard : public CCLayer{

	CCSprite* pCanSee;
	CCMenu* pMenu;
	CCMenuItemImage* pSure;
	CCMenuItemImage* pCancer;
public:
	bool init();
	void call_back(CCObject* pSender);
	CREATE_FUNC(PauseBoard);
};

class UpBox : public Human{

public:

	bool init();
	void onExit();
	void update(float dt);

	CCRect getRect(){return CCRect(getPositionX() - 74 / 2 + getParent()->getPositionX(),
		getPositionY(),74,65);};

	CCRect getDropRect(){return CCRect(getPositionX() - 74 / 2 + getParent()->getPositionX(),
		getPositionY() + 64,74,1);};

	CREATE_FUNC(UpBox);
};

class Boss : public CCNode{

	bool isDie;
	int bloodNum;
	CCArmature* pBody;
public:
	bool init();
	void stay();
	void hurt();	
	void armatureCallBack(CCArmature*, MovementEventType, const char *);
	CREATE_FUNC(Boss);
};

//PlayerSprite's parent must be Player
class PlayerSprite : public Human{

public:

	bool init();
	void update(float dt);
	void doJump();
	CCRect getRect();
	CREATE_FUNC(PlayerSprite);
};

CCAnimate* getAnimate(char* pFirstNameStr,int animationFrameCount,float dt);
CCAnimate* getAnimate(char* pFirstNameStr,int firstIndex,int lastIndex,float dt);