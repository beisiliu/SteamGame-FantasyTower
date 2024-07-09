#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"steam/steam_api.h"
#include<stdio.h>
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

class StreamAPIAchievements
{
private:
	long m_iAppID; // 我们当前的 AppID。
	Achievement_t* m_pAchievements; // 成就数据。
	int m_iNumAchievements; // 成就数量。
	bool m_bInitialized; //我们是否已调用 RequestStats，并收到回调？
public:
	StreamAPIAchievements(Achievement_t* Achievemen, int NumAchievements);

	bool RequestStats();
	bool SetAchievement(const char* ID);

	STEAM_CALLBACK(StreamAPIAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(StreamAPIAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(StreamAPIAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};

// 定义我们的成就。
enum EAchievements
{
	FirstGame = 0,
	//OneDay,
	//TwoDays,
	//FiveDays,
	//EightDays,
	//NineDays
};

// // 包含成就及其状态的相关数据的成就数组。
//Achievement_t g_Achievements[] =
//{
//	_ACH_ID(First_into_Game, "First_into_Game"),
//};
