#pragma once

#include "cocos-ext.h"
#include "cocos2d.h"
using namespace cocos2d::extension;
using namespace cocos2d;

class ArmatureTurn;

class ArmatureTurn{

public:

	void callBack();
	static ArmatureTurn* create(float dt,int playIndex);
};