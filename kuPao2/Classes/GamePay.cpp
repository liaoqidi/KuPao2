#include "GamePay.h"

GamePay* GamePay::m_instance = NULL;

GamePay* GamePay::getInstance()
{
	if (GamePay::m_instance == NULL)
	{
		GamePay::m_instance = new GamePay();
	}

	return GamePay::m_instance;
}

GamePay::GamePay()
{
	m_isSecond   = 0;
	m_isDistinct = 0;
	signLayer = NULL;
	mainBoard = NULL;
}

void GamePay::gameInit(int initNum)
{
	if (initNum != NULL)
	{
		m_isSecond   = (initNum/10)%10;
		m_isDistinct = (initNum%10);
	}
}

void GamePay::paySuccess(int payStyle)
{
	switch (payStyle)
	{
	case PAY_GIFT:
		if (signLayer){
			signLayer->revGift();
		}
		break;
	case PAY_GOLD:
		saveAndAdd("contentGold", 10000);
		break;
	case PAY_MONEY:
		saveAndAdd("contentMoney", 500);
		break;
	case PAY_BLOOD:
		CCUserDefault::sharedUserDefault()->setIntegerForKey("bloodNum", 5);
		break;
	case PAY_VIP:
		CCUserDefault::sharedUserDefault()->setBoolForKey("isLord", true);
		if (mainBoard){
			mainBoard->checkCom();
		}
		saveAndAdd("contentGold", 2000);
		saveAndAdd("contentMoney", 188);
		break;
	default:
		break;
	}
}

void GamePay::saveAndAdd(const char* pName, int num){

	CCUserDefault* pSava = CCUserDefault::sharedUserDefault();
	int contentNum = pSava->getIntegerForKey(pName);
	pSava->setIntegerForKey(pName, contentNum + num);
}

void GamePay::payFail()
{


}

bool GamePay::getIsSecond()
{
	if (m_isSecond == 0)
	{
		return false;
	}
	return true;
}

bool GamePay::getIsDistinct()
{
	if (m_isDistinct == 0)
	{
		return true;
	}
	return false;
}
