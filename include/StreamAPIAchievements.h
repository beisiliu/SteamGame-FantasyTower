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
	long m_iAppID; // ���ǵ�ǰ�� AppID��
	Achievement_t* m_pAchievements; // �ɾ����ݡ�
	int m_iNumAchievements; // �ɾ�������
	bool m_bInitialized; //�����Ƿ��ѵ��� RequestStats�����յ��ص���
public:
	StreamAPIAchievements(Achievement_t* Achievemen, int NumAchievements);

	bool RequestStats();
	bool SetAchievement(const char* ID);

	STEAM_CALLBACK(StreamAPIAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(StreamAPIAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(StreamAPIAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};

// �������ǵĳɾ͡�
enum EAchievements
{
	FirstGame = 0,
	//OneDay,
	//TwoDays,
	//FiveDays,
	//EightDays,
	//NineDays
};

// // �����ɾͼ���״̬��������ݵĳɾ����顣
//Achievement_t g_Achievements[] =
//{
//	_ACH_ID(First_into_Game, "First_into_Game"),
//};
