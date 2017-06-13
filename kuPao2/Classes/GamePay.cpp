#include "GamePay.h"
#include "AppDelegate.h"
#include "SignScene.h"
#include "MainBoard.h"
GamePay::GamePay()
{
	m_isSecond   = "0";
	m_isDistinct = "0";
}

void GamePay::gameInit(char* str)
{
	vector<char*> initVer;
	//·Ö¸î×Ö·û´®
	initVer = this->splitWithStl(str);
	if (initVer[1] != NULL && initVer[2] != NULL)
	{
		m_isSecond = initVer[1];
		m_isDistinct = initVer[2];
	}
}

void GamePay::paySuccess(int payStyle)
{
	SignLayer sign;
	MainBoard board;
	switch (payStyle)
	{
	case PAY_GIFT:
		sign.revGift();
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
		board.checkCom();
		saveAndAdd("contentGold", 2000);
		saveAndAdd("contentMoney", 188);
		break;
	default:
		break;
	}
}


void GamePay::payFail()
{


}

vector<char*> GamePay::splitWithStl(char *Str)
{
	vector<char*> ver;
	char *Delim = ",";
	char*temp = strtok(Str, Delim);
	while (temp)
	{
		ver.push_back(temp);
		temp = strtok(NULL, Delim);
		
	}
	return ver;
}