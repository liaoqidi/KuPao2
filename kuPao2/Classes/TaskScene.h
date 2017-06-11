#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;
using namespace cocos2d::gui;
using namespace cocos2d;

class TaskLayer;

class TaskLayer : public UILayer{

	UIButton* pReturn;
	UIButton* pGet;
	UIImageView* pBoard;
	UIImageView* pTaskImg;
	UILabelAtlas* pTaskNum;
	UILoadingBar* pTaskPower;
public:

	bool init();
	void initCom();
	void hideCom();
	void done();
	void callBack(CCObject* pSender,TouchEventType type);
	void checkTask();
	CREATE_FUNC(TaskLayer);
};

const float taskMax[] = {0,2000,200,1};