#include "LogButton.h"

LogButton::LogButton()
{
	// 层数按钮的状态, 暂时初始化10层
	for (int i = 0; i < 10; ++i)
	{
		floorButtonVector.push_back(false);
	}
}

void LogButton::logButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager* m_textureManager)
{
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["LogButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// 时间记录
	if (ImGui::Button(removeQuotes(p_settingLanguage["LogButton"]["TimeRecord"].dump()).c_str(), ImVec2(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) - 10, 32.0))))
	{
		isGameHintButton = false;
		isEnemyButton = false;
		isLogButton = true;
	}

	// 信息回顾
	if (ImGui::Button(removeQuotes(p_settingLanguage["LogButton"]["GameHint"].dump()).c_str(), ImVec2(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) - 10, 32.0))))
	{
		isLogButton = false;
		isEnemyButton = false;
		isGameHintButton = true;
	}

	// 敌人信息
	if (ImGui::Button(removeQuotes(p_settingLanguage["LogButton"]["EnemyInformation"].dump()).c_str(), ImVec2(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) - 10, 32.0))))
	{
		isLogButton = false;
		isGameHintButton = false;
		isEnemyButton = true;
	}

	// 返回按钮
	// 定位坐标是基于Frame框架内的X,Y坐标的
	ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
	// 返回到HT主场景
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		IsWindowStatus = false;
	}

	ImGui::End();

	logButtonFrameEx(p_settingJson, p_settingLanguage, p_save01Data);
	gameHintButtonFrameEx(p_settingJson, p_settingLanguage, p_save01Data);
	enemyButtonFrameEx(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);
}

void LogButton::logButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	if (isLogButton)
	{
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 2), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) + 64.0, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
		ImGui::Begin("logButtonFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		// 当前在第二层的时候，显示第一层的数据
		for (int i = 2; i <= p_save01Data["HtFloor"]; i++)
		{
			// 字符串拼接
			std::string floorTimeKey = "FloorTime" + std::to_string(i - 1);
			std::string logString = removeQuotes(p_settingLanguage["LogButton"]["FloorMessage1"].dump()) + removeQuotes(std::to_string(i - 1)) + removeQuotes(p_settingLanguage["LogButton"]["FloorMessage2"].dump())
				+ removeQuotes(p_save01Data[floorTimeKey.c_str()].dump()) + removeQuotes(p_settingLanguage["LogButton"]["FloorMessage3"].dump());
			ImGui::Text(logString.c_str());
		}

		ImGui::End();
	}
}

void LogButton::floorButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	// 放楼层按钮
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 8), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) + 64.0, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin("gameHintButtonFrameEx01", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	for (int i = 1; i <= p_save01Data["HtFloor"]; ++i)
	{
		std::string floorNum = "Floor00" + std::to_string(i);
		if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"][floorNum].dump()).c_str()))
		{
			if (floorNum == "Floor001")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[0] = true;
			}
			if (floorNum == "Floor002")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[1] = true;
			}
			if (floorNum == "Floor003")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[2] = true;
			}
			if (floorNum == "Floor004")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[3] = true;
			}
			if (floorNum == "Floor005")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[4] = true;
			}
			if (floorNum == "Floor006")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[5] = true;
			}
			if (floorNum == "Floor007")
			{
				for (int i = 0; i < p_save01Data["HtFloor"]; ++i) floorButtonVector[i] = false;
				floorButtonVector[6] = true;
			}
		}
	}

	ImGui::End();
}

void LogButton::gameHintButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	if (isGameHintButton)
	{
		floorButtonFrameEx(p_settingJson, p_settingLanguage, p_save01Data);

		// 放楼层提示信息
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 2 + 128.0), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) + float((p_settingJson["SetWindowWidth"] - 96) / 8) + 64.0, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
		ImGui::Begin("gameHintButtonFrameEx02", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		// 需要判断vector的长度，不能超范围
		// 1层被按下的效果
		if (floorButtonVector[0])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Message"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint1"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint2"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint3"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint4"].dump()).c_str());
		}

		// 2层被按下的效果
		if (floorButtonVector[1])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor002"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor002Hint1"].dump()).c_str());
		}

		// 3层被按下的效果
		if (floorButtonVector[2])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor003"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor003Hint1"].dump()).c_str());
		}

		// 4层被按下的效果
		if (floorButtonVector[3])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor004"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor004Hint1"].dump()).c_str());
		}

		// 5层被按下的效果
		if (floorButtonVector[4])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005Hint1"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005Hint2"].dump()).c_str());
		}

		// 6层被按下的效果
		if (floorButtonVector[5])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint1"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint2"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint3"].dump()).c_str());
		}
		
		// 7层被按下的效果
		if (floorButtonVector[6])
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007Hint1"].dump()).c_str());
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007Hint2"].dump()).c_str());
		}

		ImGui::End();
	}
}

void LogButton::enemyInformation(int floorNum, TextureManager* m_textureManager, json p_settingLanguage)
{
	if (m_enemyAttribute != nullptr) delete m_enemyAttribute;
	// 初始化敌人数据
	m_enemyAttribute = new EnemyAttribute(floorNum);

	std::string Health = removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyHealth"].dump()) + std::to_string(m_enemyAttribute->p_enemyHealthLimit);
	std::string Attack = removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyAttack"].dump()) + std::to_string(m_enemyAttribute->p_enemyAttack);
	std::string Defense = removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyDefense"].dump()) + std::to_string(m_enemyAttribute->p_enemyDefense);
	std::string Speed = removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemySpeed"].dump()) + std::to_string(m_enemyAttribute->p_enemySpeed);

	ImGui::Text(Health.c_str());
	ImGui::Text(Attack.c_str());
	ImGui::Text(Defense.c_str());
	ImGui::Text(Speed.c_str());
}

void LogButton::enemyButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager)
{
	if (isEnemyButton)
	{
		floorButtonFrameEx(p_settingJson, p_settingLanguage, p_save01Data);

		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 2 + 128.0), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4) + float((p_settingJson["SetWindowWidth"] - 96) / 8) + 64.0, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
		ImGui::Begin("gameHintButtonFrameEx02", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		if (floorButtonVector[0])
		{
			m_textureManager->imguiIMG(PNG_LOG_ENEMY_FLOOR001_ID, 64, 64);
			enemyInformation(1, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[1])
		{
			m_textureManager->imguiIMG(ENEMY_FLOOR002_ID, 64, 64);
			enemyInformation(2, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[2])
		{
			m_textureManager->imguiIMG(ENEMY_FLOOR003_ID, 64, 64);
			enemyInformation(3, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[3])
		{
			m_textureManager->imguiIMG(ENEMY_FLOOR004_ID, 64, 64);
			enemyInformation(4, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[4])
		{
			m_textureManager->imguiIMG(ENEMY_FLOOR005_ID, 64, 64);
			enemyInformation(5, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[5])
		{
			m_textureManager->imguiIMG(PNG_LOG_ENEMY_FLOOR006_ID, 64, 64);
			enemyInformation(6, m_textureManager, p_settingLanguage);
		}

		if (floorButtonVector[6])
		{
			// chests
			m_textureManager->imguiIMG(PNG_LOG_CHESTS_ID, 64, 64);

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
			ImGui::PopStyleColor(1);
			ImGui::SameLine();

			m_textureManager->imguiIMG(PNG_PRIMARYSTRENGTHENINGSTONE_ID, 16, 16);
			ImGui::SameLine();
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor007ChestsAward1"].dump()).c_str());

			// enemy
			m_textureManager->imguiIMG(PNG_LOG_ENEMY_FLOOR007_ID, 64, 64);
			enemyInformation(7, m_textureManager, p_settingLanguage);
		}
		ImGui::End();


	}
}
