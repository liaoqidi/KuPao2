#include "SDKManager.h"
#include "GamePay.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

SDKManager *SDKManager::m_instance = NULL;

SDKManager *SDKManager::getinstance()
{
	if (SDKManager::m_instance == NULL)
	{
		SDKManager::m_instance = new SDKManager();
	}
	return SDKManager::m_instance;
}

void SDKManager::pay(int priceId)
{
	m_payStyle = priceId;
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"kkk/com/cocos2dpaoKu2/paoKu2","pay","(Ljava/lang/String;I)V");
	if (isHave){
	jstring jstr = minfo.env->NewStringUTF("2000");     

	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jstr, priceId);
	minfo.env->DeleteLocalRef(minfo.classID);
	}
	else{
		CCLog("error+++++");
	}
}

void SDKManager::init()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"kkk/com/cocos2dpaoKu2/paoKu2","payInit","(Ljava/lang/String;)V");
	if (isHave){
		jstring jstr = minfo.env->NewStringUTF("2000");
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jstr);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	else{
		CCLog("error+++++");
	}
}


extern "C"{
	void Java_kkk_com_cocos2dpaoKu2_paoKu2_InitCallFunc(JNIEnv *env, jobject thiz, jint num)
	{
		GamePay::getInstance()->gameInit(num);
	}
}
extern "C"{
	void Java_kkk_com_cocos2dpaoKu2_paoKu2_PayCallFunc(JNIEnv *env, jobject thiz, jstring jstr)
	{
		const char *pkgName = env->GetStringUTFChars(jstr, NULL);
		CCLog("pkgName is %s", pkgName);
		GamePay::getInstance()->paySuccess(SDKManager::m_payStyle);
		env->ReleaseStringUTFChars(jstr, pkgName);
	}
}
#endif