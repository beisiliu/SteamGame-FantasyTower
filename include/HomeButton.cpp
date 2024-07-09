#include "HomeButton.h"

HomeButton::HomeButton()
{
	m_writeData = new WriteData();
	m_equipmentToolTip = new EquipmentToolTip();

	for (int i = 0; i < TOTALSTASH; i++)
	{
		m_statshButtonStatusArray[i] = false;
	}
}

HomeButton::~HomeButton()
{
	delete m_writeData;
	delete m_equipmentToolTip;
}

void HomeButton::homeButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager)
{
	if (p_save01Data["HtFloor"] == 1)
	{
		//isOpen = true;
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["HomeButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());

		// ���ذ�ť
		// ��λ�����ǻ���Frame����ڵ�X,Y�����
		ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
		// ���ص�HT������
		if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
		{
			//isOpen = false;
			IsWindowStatus = false;
		}

		ImGui::End();
	}
	// ��ʾ�������ݣ���ֻ��ʾ���ҵ�˯����ť
	if (p_save01Data["HtFloor"] >= 2)
	{
		p_isReturn = false;
		if (!p_isSleepAnimation)
		{
			bedRoomFrame(p_settingJson, p_settingLanguage, p_save01Data);
			stashRoomFrame(p_settingJson, p_settingLanguage, p_save01Data);  // ���ݵĽ����ڶ�Ӧ�Ĵ����н��п��ƺͽ���
			stashRoomFrameEx(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);
			workRoomFrame(p_settingJson, p_settingLanguage, p_save01Data);
			workRoomFrameEx(p_settingJson, p_settingLanguage);
			returnFrame(p_settingJson, p_settingLanguage);
		}

	}

}

void HomeButton::bedRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin(removeQuotes(p_settingLanguage["HomeButton"]["BedRoomFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// ˯����ť����
	if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["BedRoomFrame"]["Sleep"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		// Ѫ�� +20 ��״̬������ʱ���1��tooltip����˵����Ѫ�� +20 ��Ҫ���������ļ�����Ͻ���ʵʱ�Ķ�
		// ����Ѫ��
		int health = int(p_save01Data["personCurrentHealth"]) + int(p_save01Data["homeSleepAddHealth"]);
		if (health <= p_save01Data["personCurrentHealth"])  // ����󣬻���С������ֵ����health������ǰ������ֵ
		{
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", health);
		}
		else // ����ֱ�ӻ��� 
		{
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", int(p_save01Data["personAllHealth"]));
		}
		// ����״ֵ̬��״ֵ̬����
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", int(p_save01Data["personAllStatus"]));
		// ʱ���һ��
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", int(p_save01Data["gameTime"] + 1));

		// ����˯������
		p_isSleepAnimation = true;
	}
	// ˯����ť��ToolTip
	if (ImGui::IsItemHovered()) 
	{
		std::string healthRecoverNum = p_save01Data["homeSleepAddHealth"].dump();
		std::string sleepToolTip = removeQuotes(p_settingLanguage["HomeButton"]["BedRoomFrame"]["SleepToolTip1"].dump()) + healthRecoverNum + removeQuotes(p_settingLanguage["HomeButton"]["BedRoomFrame"]["SleepToolTip2"].dump());
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(sleepToolTip.c_str());
			ImGui::EndTooltip();
		}
	}

	ImGui::End();
}

void HomeButton::stashRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}
	// ���ڵ���4��ʱ����ʾ����
	if (p_save01Data["HtFloor"] >= 4)
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonWeapons"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[WEAPONS] = true;
		}
	}
	// ���ڵ���5��ʱ����ʾװ���ļ���
	if (p_save01Data["HtFloor"] >= 5)
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonHead"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[HEAD] = true;
		}

		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonArms"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[ARMS] = true;
		}

		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonBody"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[BODY] = true;
		}

		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonLegs"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[LEGS] = true;
		}
	}
	// ���ڵ���6��ʱ����ʾ����
	if (p_save01Data["HtFloor"] >= 6)
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonSkills"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[SKILLS] = true;
		}
	}
	// ���ڵ���7��ʱ����ʾҩˮ
	if (p_save01Data["HtFloor"] >= 7)
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonPotions"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSTASH; i++)
			{
				m_statshButtonStatusArray[i] = false;
			}
			m_statshButtonStatusArray[POTIONS] = true;
		}
	}

	ImGui::End();
}

void HomeButton::stashRoomFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager *m_textureManager)
{
	ImGui::SetNextWindowSize(ImVec2(256.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(128+128.0f, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin("stashRoomFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (m_statshButtonStatusArray[WEAPONS])
	{
		json weaponsJson = p_save01Data["Weapons"];
		for (auto it = weaponsJson.begin(); it != weaponsJson.end(); ++it)
		{
			if (it.value() == 1)
			{
				// ���ֽ���ͼ�꣬ ����Ҫ���ж�
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());
				ImGui::SameLine();
				m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);

				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ����װ����ж��,��֤һ��װ��һ��װ��
					for (auto it = weaponsJson.begin(); it != weaponsJson.end(); ++it)
					{
						if (it.value() == 2)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 1);
						}
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 2); // ���װ���󣬰Ѹ�������ֵ��Ϊ2
				}
			}
		}
	}

	if (m_statshButtonStatusArray[HEAD])
	{
		json headJson = p_save01Data["HeadEquip"];
		for (auto it = headJson.begin(); it != headJson.end(); ++it)
		{
			if (it.value() == 1)
			{
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				// m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitHeadToolTip(it.key(), p_settingLanguage);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ����װ����ж��
					for (auto it = headJson.begin(); it != headJson.end(); ++it)
					{
						if (it.value() == 2)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 1);
						}
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 2); // ���װ���󣬰Ѹ�������ֵ��Ϊ2
				}
			}
		}
	}

	if (m_statshButtonStatusArray[ARMS])
	{
		json armsJson = p_save01Data["ArmsEquip"];
		for (auto it = armsJson.begin(); it != armsJson.end(); ++it)
		{
			if (it.value() == 1)
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				// m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitArmsToolTip(it.key(), p_settingLanguage);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ����װ����ж��
					for (auto it = armsJson.begin(); it != armsJson.end(); ++it)
					{
						if (it.value() == 2)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 1);
						}
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 2); // ���װ���󣬰Ѹ�������ֵ��Ϊ2
				}
			}
		}
	}

	if (m_statshButtonStatusArray[BODY])
	{
		json bodyJson = p_save01Data["BodyEquip"];
		for (auto it = bodyJson.begin(); it != bodyJson.end(); ++it)
		{
			if (it.value() == 1)
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				// m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitBodyToolTip(it.key(), p_settingLanguage);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ����װ����ж��
					for (auto it = bodyJson.begin(); it != bodyJson.end(); ++it)
					{
						if (it.value() == 2)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 1);
						}
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 2); // ���װ���󣬰Ѹ�������ֵ��Ϊ2
				}
			}
		}

	}

	if (m_statshButtonStatusArray[LEGS])
	{
		json legsJson = p_save01Data["LegsEquip"];
		for (auto it = legsJson.begin(); it != legsJson.end(); ++it)
		{
			if (it.value() == 1)
			{
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());
				m_equipmentToolTip->equitLegsToolTip(it.key(), p_settingLanguage);
				ImGui::SameLine();

				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ����װ����ж��
					for (auto it = legsJson.begin(); it != legsJson.end(); ++it)
					{
						if (it.value() == 2)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 1);
						}
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 2); // ���װ���󣬰Ѹ�������ֵ��Ϊ2
				}
			}
		}

	}

	if (m_statshButtonStatusArray[SKILLS])
	{
		json skillJson = p_save01Data["personSkill"];
		int buttonMountID = 0;
		for (auto it = skillJson.begin(); it != skillJson.end(); ++it)
		{
			// û��װ�����߼�
			if (it.value() == 1)
			{
				
				buttonMountID += 1;
				isPicSkills(it.key(), m_textureManager); // ����ͼ��
				ImGui::Text(removeQuotes(p_settingLanguage["Skills"][it.key()].dump()).c_str());
				m_equipmentToolTip->skillsTootTip(it.key(), p_settingLanguage);
				ImGui::SameLine();

				// ��װ����ť���߼�
				ImGui::PushID(buttonMountID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equip"].dump()).c_str()))
				{
					// ע������it��������һ��
					for (auto it = skillJson.begin(); it != skillJson.end(); ++it)
					{
						// ���õ���Ҫ��һ���жϣ�value != 0�����
						if (it.value() != 0)
						{
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", it.key(), 1);
						}	
					}
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", it.key(), 2);
				}
				ImGui::PopID();
			}

			if (it.value() == 2)
			{
				buttonMountID += 1;
				isPicSkills(it.key(), m_textureManager); // ����ͼ��
				ImGui::Text(removeQuotes(p_settingLanguage["Skills"][it.key()].dump()).c_str());
				m_equipmentToolTip->skillsTootTip(it.key(), p_settingLanguage);
				ImGui::SameLine();
				ImGui::PushID(buttonMountID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["Equiping"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", it.key(), 1);
				}
				ImGui::PopID();
			}

		}
	}

	if (m_statshButtonStatusArray[POTIONS])
	{
		if (p_save01Data["smallHealthPotionCount"] > 0)
		{
			m_textureManager->imguiIMG(PNG_POTION_SMALLHEALTHPOTION_ID, 16, 16);
			ImGui::SameLine();

			ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallHealthPotion"].dump()).c_str());
			ImGui::SameLine();
			ImGui::Text("*");
			ImGui::SameLine();
			ImGui::Text(p_save01Data["smallHealthPotionCount"].dump().c_str());
			ImGui::SameLine();

			// can drinking
			ImGui::PushID(1);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["Potions"]["DrinkButton"].dump()).c_str()))
			{
				// Think about : health +10, health limit, count -1
				// deal with health +10 and health limit
				if ((int(p_save01Data["personAllHealth"]) - int(p_save01Data["personCurrentHealth"])) >= 10)
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", p_save01Data["personCurrentHealth"] + 10);
				}
				else
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", int(p_save01Data["personAllHealth"]));
				}

				// deal with count -1
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallHealthPotionCount", p_save01Data["smallHealthPotionCount"] - 1);
			}
			ImGui::PopID();

		}

		if (p_save01Data["smallStatusPotionCount"] > 0)
		{
			m_textureManager->imguiIMG(PNG_POTION_SMALLSTATUSPOTION_ID, 16, 16);
			ImGui::SameLine();

			ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallStatusPotion"].dump()).c_str()); // test
			ImGui::SameLine();
			ImGui::Text("*");
			ImGui::SameLine();
			ImGui::Text(p_save01Data["smallStatusPotionCount"].dump().c_str()); // count
			ImGui::SameLine();

			// can drinking
			ImGui::PushID(2);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["Potions"]["DrinkButton"].dump()).c_str()))
			{
				// Think about : status +10 status limit, count -1
				// deal with status +10 and status limit
				if ((int(p_save01Data["personAllStatus"]) - int(p_save01Data["personCurrentStatus"])) >= 10)
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", p_save01Data["personCurrentStatus"] + 10);
				}
				else
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", int(p_save01Data["personAllStatus"]));
				}

				// deal with count -1
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallStatusPotionCount", p_save01Data["smallStatusPotionCount"] - 1);
			}
			ImGui::PopID();
		}
	}

	ImGui::End();
}

void HomeButton::workRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(128 + 128 + 256, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin(removeQuotes(p_settingLanguage["HomeButton"]["WorkRoomFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (p_save01Data["HtFloor"] <= 6)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	ImGui::End();
}

void HomeButton::workRoomFrameEx(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowSize(ImVec2(256.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(128 + 128 + 256 + 128, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin("workRoomFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::End();
}

void HomeButton::returnFrame(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowSize(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(128 + 128 + 256 + 128 + 256, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin("returnFrame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// ��λ�����ǻ���retrunFrame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(10, float(p_settingJson["SetWindowHeight"] / 3 + 64 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		p_isReturn = true;
	}

	ImGui::End();
}

void HomeButton::isPicSkills(std::string skillName, TextureManager *m_textureManager)
{
	if (skillName == SKILLDEFENSE)
	{
		m_textureManager->imguiIMG(PNG_SKILL_DEFENSE_ID, 16, 16);
		ImGui::SameLine();
	}
	if (skillName == SKILLHEAVYATTACK)
	{
		m_textureManager->imguiIMG(PNG_SKILL_HEAVYATTACK_ID, 16, 16);
		ImGui::SameLine();
	}
}