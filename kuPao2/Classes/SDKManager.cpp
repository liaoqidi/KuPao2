#include "SDKManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

void SDKManager::pay(int priceId)
{
	if (priceId == NULL){ return;}
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
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"kkk/com/cocos2dpaoKu2/paoKu2","payInit","()V");
	if (isHave){
		jstring jstr = minfo.env->NewStringUTF("hahaha");

		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jstr);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	else{
		CCLog("error+++++");
	}
}


extern "C"{
	void Java_kkk_com_cocos2dpaoKu2_paoKu2_InitCallFunc(JNIEnv *env, jobject thiz, jstring jstr)
	{
		const char *pkgName = env->GetStringUTFChars(jstr, NULL);
		CCLog("pkgName is %s", pkgName);
		env->ReleaseStringUTFChars(jstr, pkgName);
	}

	void Java_kkk_com_cocos2dpaoKu2_paoKu2_PayCallFunc(JNIEnv *env, jobject thiz, jstring jstr)
	{
		const char *pkgName = env->GetStringUTFChars(jstr, NULL);
		CCLog("pkgName is %s", pkgName);
		env->ReleaseStringUTFChars(jstr, pkgName);
	}
}

#endif