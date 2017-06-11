#include "BloodTool.h"
#include "cocos2d.h"

using namespace cocos2d;


struct tm* getContentTime(){

	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);
#else  
	struct cc_timeval now;  
	CCTime::gettimeofdayCocos2d(&now, NULL);  
	timep = now.tv_sec; 
#endif  
	return localtime(&timep);
}

long getContentSec(int day,int hour,int min,int sec){

	return ((day * 24 + hour) * 60 + min) * 60 + sec;
}

long getContentSec(){

	struct tm* tm = getContentTime();
	return getContentSec(tm->tm_yday,tm->tm_hour,tm->tm_min,tm->tm_sec);
}

BloodData BloodTool::getContentBlood(){
	
	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int powerNum = pSava->getIntegerForKey("bloodNum");

	int contentTime = getContentSec();

	if(powerNum < 5){

		int NextTime = pSava->getIntegerForKey("preTime",0) + 60 * 35 - 1;

		if(NextTime - contentTime < 0){

			pSava->setIntegerForKey("preTime",contentTime);
			pSava->setIntegerForKey("bloodNum",powerNum + 1);
		}

		return BloodData(BloodTime(NextTime - contentTime),powerNum);
	}else{

		return BloodData(BloodTime(),5);
	}
} 

bool BloodTool::deleteBlood(){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();

	int powerNum = pSava->getIntegerForKey("bloodNum");

	if(powerNum <= 0){

		return false;
	}else{

		pSava->setIntegerForKey("bloodNum",powerNum - 1);
		pSava->setIntegerForKey("preTime",getContentSec());

		return true;
	}
}