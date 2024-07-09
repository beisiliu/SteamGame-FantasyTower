#include "StreamAPIAchievements.h"
#include <iostream>
//#include <Windows.h>

// �˹��캯�����ʼ��������Ա��������ץȡĿǰ���е� AppID�� 
// ����֮�⣬������ʹ���� Steam ���첽���õĻص������ҹ��� ��󣬻���� RequestStats() �����״ε��ã��Ի�ȡ��ǰ�û���ͳ����ɾ͡�
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


// �˷���������ֻ�ǽ��� ISteamUserStats::RequestCurrentStats�ĵ��ð�װ���������õ���Ϊ�첽���ã������� Steam ����ǰ�û���ͳ����ɾ͡� 
// �������Ƚ��д˵��ã���������ͳ�ƻ�ɾ͡� �˷������״ε����ڹ��캯������ɡ� �պ���������º��ͳ����ɾͣ�����ʱ���ô˷�����
bool StreamAPIAchievements::RequestStats()
{
	// �Ƿ��Ѽ��� Steam�� �����������޷���ȡͳ�ơ�
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// �û��Ƿ��ѵ�¼��  �����������޷���ȡͳ�ơ�
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// �����û�ͳ�����ݡ�
	return SteamUserStats()->RequestCurrentStats();
}
// �˷����������ɾ�����Ϊ�Ѵ�ɣ������������ Steam�� ���ɶ���趨ͬһ������ɾͣ������Ͳ��õ���ֻ���趨�����õĳɾ��ˡ�
// �˵���Ϊ�첽���ã��ᴥ�������ص���OnUserStatsStored() �� OnAchievementStored()��
bool StreamAPIAchievements::SetAchievement(const char* ID)
{
	// �Ƿ����յ����� Steam �Ļص���
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// �������޷����óɾ͡�
	return false;
}

// �˷�����ÿ�γ�������ͳ��ʱ���еĻص��� 
// ʹ�� RequestStats() ����ͳ����ɾ����ݡ� 
// �˷��������³�Ա���� m_Achievements���Է�ӳ Steam ���ص�����ͳ����ɾ����ݡ�
void StreamAPIAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// ����Ҳ����յ�������Ϸ��ͳ�ƻص�������ԡ�
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//OutputDebugString(TEXT("Received stats and achievements from Steam\n"));
			std::cout << "Received stats and achievements from Steam\n";
			m_bInitialized = true;

			// ����ɾ͡�
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

// �˷�����ÿ�γ����� Steam �ϴ���ͳ��ʱ���еĻص���
void StreamAPIAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// ����Ҳ����յ�������Ϸ��ͳ�ƻص�������ԡ�
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

// �˷�����ÿ���� Steam �ϳɹ�����ɾ�ʱ���еĻص���
void StreamAPIAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// ����Ҳ����յ�������Ϸ��ͳ�ƻص�������ԡ�
	if (m_iAppID == pCallback->m_nGameID)
	{
		//OutputDebugString("Stored Achievement for Steam\n");
		std::cout << "Stored Achievement for Steam\n";
	}
}