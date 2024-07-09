#include "StreamAPIAchievements.h"
#include <iostream>
//#include <Windows.h>

// 此构造函数会初始化几个成员函数，并抓取目前运行的 AppID。 
// 除此之外，它还会和处理对 Steam 的异步调用的回调方法挂钩。 最后，还会对 RequestStats() 进行首次调用，以获取当前用户的统计与成就。
StreamAPIAchievements::StreamAPIAchievements(Achievement_t* Achievements, int NumAchievements) :
    m_iAppID(0),
    m_bInitialized(false),
    m_CallbackUserStatsReceived(this, &StreamAPIAchievements::OnUserStatsReceived),
    m_CallbackUserStatsStored(this, &StreamAPIAchievements::OnUserStatsStored),
    m_CallbackAchievementStored(this, &StreamAPIAchievements::OnAchievementStored)
{
    m_iAppID = SteamUtils()->GetAppID();
    m_pAchievements = Achievements;
    m_iNumAchievements = NumAchievements;

	RequestStats();

}


// 此方法基本上只是将对 ISteamUserStats::RequestCurrentStats的调用包装了起来。该调用为异步调用，用来向 Steam 请求当前用户的统计与成就。 
// 您必须先进行此调用，才能设置统计或成就。 此方法的首次调用在构造函数中完成。 日后若需检查更新后的统计与成就，可随时调用此方法。
bool StreamAPIAchievements::RequestStats()
{
	// 是否已加载 Steam？ 若否，则我们无法获取统计。
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// 用户是否已登录？  若否，则我们无法获取统计。
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// 请求用户统计数据。
	return SteamUserStats()->RequestCurrentStats();
}
// 此方法将给定成就设置为已达成，并将结果发至 Steam。 您可多次设定同一项给定成就，这样就不用担心只能设定尚设置的成就了。
// 此调用为异步调用，会触发两个回调：OnUserStatsStored() 与 OnAchievementStored()。
bool StreamAPIAchievements::SetAchievement(const char* ID)
{
	// 是否已收到来自 Steam 的回调？
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// 若否，则无法设置成就。
	return false;
}

// 此方法是每次尝试请求统计时进行的回调。 
// 使用 RequestStats() 请求统计与成就数据。 
// 此方法将更新成员变量 m_Achievements，以反映 Steam 返回的最新统计与成就数据。
void StreamAPIAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// 我们也许会收到其他游戏的统计回调，请忽略。
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//OutputDebugString(TEXT("Received stats and achievements from Steam\n"));
			std::cout << "Received stats and achievements from Steam\n";
			m_bInitialized = true;

			// 载入成就。
			for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
			{
				Achievement_t& ach = m_pAchievements[iAch];
				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			//OutputDebugString(buffer);
			std::cout << buffer << std::endl;
		}
	}
}

// 此方法是每次尝试在 Steam 上储存统计时进行的回调。
void StreamAPIAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// 我们也许会收到其他游戏的统计回调，请忽略。
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//OutputDebugString("Stored stats for Steam\n");
			//std::cout << "Stored stats for Steam\n";
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			//OutputDebugString(buffer);
			std::cout << buffer << std::endl;
		}
	}
}

// 此方法是每次在 Steam 上成功储存成就时进行的回调。
void StreamAPIAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// 我们也许会收到其他游戏的统计回调，请忽略。
	if (m_iAppID == pCallback->m_nGameID)
	{
		//OutputDebugString("Stored Achievement for Steam\n");
		std::cout << "Stored Achievement for Steam\n";
	}
}