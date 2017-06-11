#pragma once

class BloodTool;

struct BloodTime{

	int min;
	int sec;

	BloodTime(){

		min = -1;
	}

	BloodTime(long allTime){

		min = allTime / 60;
		sec = allTime % 60;
	}
};

struct BloodData{

	BloodTime nextTime;
	int contentBlood;

	BloodData(BloodTime n,int c){

		nextTime = n;
		contentBlood = c;
	}
};

class BloodTool{

public:

	static BloodData getContentBlood();
	static bool deleteBlood();
};