#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;
using namespace cocos2d::gui;
using namespace cocos2d;

class SetLayer;

class SetLayer : public UILayer{

	UIButton* pReturn;
	UIImageView* pBoard;

	UIButton* pSound;
	UIButton* pMusic;

	UIImageView* pSoundState;
	UIImageView* pSoundPoint;

	UIImageView* pMusicState;
	UIImageView* pMusicPoint;
public:

	bool init();
	void initCom();
	void hideCom();
	void done();
	void checkState();
	void callBack(CCObject* pSender,TouchEventType type);
	CREATE_FUNC(SetLayer);
};
