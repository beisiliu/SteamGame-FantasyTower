#include "SchoolButton.h"

SchoolButton::SchoolButton()
{
	m_writeData = new WriteData();
	m_equipmentToolTip = new EquipmentToolTip();
}

SchoolButton::~SchoolButton()
{
	delete m_writeData;
	delete m_equipmentToolTip;
}

void SchoolButton::schoolButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager* m_textureManager)
{

	if (p_save01Data["HtFloor"] <= 2)
	{
		//isOpen = true;
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["SchoolButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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

	if (p_save01Data["HtFloor"] >= 3)
	{
		p_isReturn = false;
		if (!p_isTrainAnimation)
		{
			baseTrainFrame(p_settingJson, p_settingLanguage, p_save01Data);
			skillTrainFrame(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);
			combatTrainFrame(p_settingJson, p_settingLanguage, p_save01Data);
			returnFrame(p_settingJson, p_settingLanguage);
		}

	}
}

void SchoolButton::baseTrainFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	//ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 3 - 64), float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	//ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowSize(ImVec2(270, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	// ��������ѵ����ť
	if (ImGui::Button(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonStrengthTraining_Junior"].dump()).c_str()))
	{
		if (p_save01Data["schoolJuniorStrengthNums"] < m_juniorStrengthAndDefenseLimits and p_save01Data["personMoney"] >= 10)
		{
			// ������������
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "psersonStrength", p_save01Data["psersonStrength"] + 1);  // ����д�������ļ�

			// ������������ֵ
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorStrengthNums", p_save01Data["schoolJuniorStrengthNums"] + 1);  // ����ֵд�������ļ�

			// ����ʱ��
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", p_save01Data["gameTime"] + 1);

			// ������
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 10);

			// ����ѵ������
			p_isTrainAnimation = true;

		}
	}
	// ��������ѵ��tooltip�� �ҽ����������10
	if (ImGui::IsItemHovered())
	{
		if (p_save01Data["personMoney"] >= 10 and p_save01Data["schoolJuniorStrengthNums"] < m_juniorStrengthAndDefenseLimits)
		{
			std::string juniorStrengthTooltip1 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonStrengthTraining_Junior_ToolTip"].dump());
			std::string juniorStrengthTooltip2 = p_save01Data["schoolJuniorStrengthNums"].dump();
			std::string juniorStrengthTooltip3 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["JuniorStrengthAndDefenseLimits"].dump());
			std::string juniorStrengthTooltip = juniorStrengthTooltip1 + juniorStrengthTooltip2 + juniorStrengthTooltip3;
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(juniorStrengthTooltip.c_str());
				ImGui::EndTooltip();
			}
		}
		else
		{
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["NotEnoughGold"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}

	}

	// ��������ѵ����ť
	if (ImGui::Button(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonDefenseTraining_Junior"].dump()).c_str()))
	{
		if (p_save01Data["schoolJuniorDefenseNums"] < m_juniorStrengthAndDefenseLimits and p_save01Data["personMoney"] >= 10)
		{
			// �����������
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personDefense", p_save01Data["personDefense"] + 1);  // ����д�������ļ�

			// �����������ֵ
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorDefenseNums", p_save01Data["schoolJuniorDefenseNums"] +  1);  // ����ֵд�������ļ�

			// ����ʱ��
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", p_save01Data["gameTime"] + 1);

			// ������
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 10);

			p_isTrainAnimation = true;

		}
	}
	// ��������ѵ��tooltip
	if (ImGui::IsItemHovered())
	{
		if (p_save01Data["personMoney"] >= 10 and p_save01Data["schoolJuniorDefenseNums"] < m_juniorStrengthAndDefenseLimits)
		{
			std::string juniorDefenseTooltip1 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonDefenseTraining_Junior_ToolTip"].dump());
			std::string juniorDefenseTooltip2 = p_save01Data["schoolJuniorDefenseNums"].dump();
			std::string juniorDefenseTooltip3 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["JuniorStrengthAndDefenseLimits"].dump());
			std::string juniorDefenseTooltip = juniorDefenseTooltip1 + juniorDefenseTooltip2 + juniorDefenseTooltip3;
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(juniorDefenseTooltip.c_str());
				ImGui::EndTooltip();
			}
		}
		else
		{
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["NotEnoughGold"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}

	}

	// �����ٶ�ѵ����ť
	if (ImGui::Button(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonSpeedTraining_Junior"].dump()).c_str()))
	{
		if (p_save01Data["schoolJuniorSpeedNums"] < m_juniorSpeedLimits and p_save01Data["personMoney"] >= 30)
		{
			// ���������ٶ�
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personSpeed", p_save01Data["personSpeed"] + 1);  // �ٶ�д�������ļ�
			
			// �����ٶ�����ֵ
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorSpeedNums", p_save01Data["schoolJuniorSpeedNums"] + 1);  // ����ֵд�������ļ�

			// ����ʱ��
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", p_save01Data["gameTime"] + 1);

			// ������
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 30);

			p_isTrainAnimation = true;

		}
	}
	// �����ٶ�ѵ��tooltip
	if (ImGui::IsItemHovered())
	{
		if (p_save01Data["personMoney"] >= 30 and p_save01Data["schoolJuniorSpeedNums"] < m_juniorSpeedLimits)
		{
			std::string juniorSpeedTooltip1 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonSpeedTraining_Junior_ToolTip"].dump());
			std::string juniorSpeedTooltip2 = p_save01Data["schoolJuniorSpeedNums"].dump();
			std::string juniorSpeedTooltip3 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["JuniorSpeedLimits"].dump());
			std::string juniorSpeedTooltip = juniorSpeedTooltip1 + juniorSpeedTooltip2 + juniorSpeedTooltip3;
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(juniorSpeedTooltip.c_str());
				ImGui::EndTooltip();
			}
		}
		else
		{
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["NotEnoughGold"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}
	}

	// 7���Ժ�
	if (p_save01Data["HtFloor"] >= 7)
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonHealthTraining_Junior"].dump()).c_str()))
		{
			if (p_save01Data["schoolJuniorHealthNums"] < m_juniorHealthAndStatusLimits and p_save01Data["personMoney"] >= 5)
			{
				// ����Ѫ��ֵ���޺�Ѫ��ֵ
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personAllHealth", p_save01Data["personAllHealth"] + 1);
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", p_save01Data["personCurrentHealth"] + 1);

				// ����ѧϰ����ֵ
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorHealthNums", p_save01Data["schoolJuniorHealthNums"] + 1);

				// ������
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 5);

				p_isTrainAnimation = true;
			}
		}
		if (ImGui::IsItemHovered())
		{
			if (p_save01Data["personMoney"] >= 5 and p_save01Data["schoolJuniorHealthNums"] < m_juniorHealthAndStatusLimits)
			{
				std::string juniorHealthTooltip1 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonHealthTraining_Junior_ToolTip"].dump());
				std::string juniorHealthTooltip2 = p_save01Data["schoolJuniorHealthNums"].dump();
				std::string juniorHealthTooltip3 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["JuniorHealthAndStatusLimits"].dump());
				std::string juniorHealthTooltip = juniorHealthTooltip1 + juniorHealthTooltip2 + juniorHealthTooltip3;
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(juniorHealthTooltip.c_str());
					ImGui::EndTooltip();
				}
			}
			else
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["NotEnoughGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}
		}


		if (ImGui::Button(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonStatusTraining_Junior"].dump()).c_str()))
		{
			if (p_save01Data["schoolJuniorStatusNums"] < m_juniorHealthAndStatusLimits and p_save01Data["personMoney"] >= 5)
			{
				// ����״ֵ̬���޺�Ѫ��ֵ
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personAllStatus", p_save01Data["personAllStatus"] + 1);
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", p_save01Data["personCurrentStatus"] + 1);

				// ����ѧϰ����ֵ
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorStatusNums", p_save01Data["schoolJuniorStatusNums"] + 1);

				// ������
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 5);

				p_isTrainAnimation = true;
			}
		}
		if (ImGui::IsItemHovered())
		{
			if (p_save01Data["personMoney"] >= 5 and p_save01Data["schoolJuniorStatusNums"] < m_juniorHealthAndStatusLimits)
			{
				std::string juniorStatusTooltip1 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["ButtonStatusTraining_Junior_ToolTip"].dump());
				std::string juniorStatusTooltip2 = p_save01Data["schoolJuniorStatusNums"].dump();
				std::string juniorStatusTooltip3 = removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["JuniorHealthAndStatusLimits"].dump());
				std::string juniorStatusTooltip = juniorStatusTooltip1 + juniorStatusTooltip2 + juniorStatusTooltip3;
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(juniorStatusTooltip.c_str());
					ImGui::EndTooltip();
				}
			}
			else
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["BaseTrainFrame"]["NotEnoughGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}
		}
	}

	ImGui::End();
}

void SchoolButton::skillTrainFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager)
{
	ImGui::SetNextWindowSize(ImVec2(270, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(270, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	if (p_save01Data["HtFloor"] <= 5) { ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str()); }
	if (p_save01Data["HtFloor"] >= 6)
	{
		skillJson = p_save01Data["personSkill"];
		for (auto it = skillJson.begin(); it != skillJson.end(); ++it)
		{
			if (it.value() == 0)
			{
				//ImGui::Text(removeQuotes(p_settingLanguage["Skills"][it.key()].dump()).c_str());
				//m_equipmentToolTip->skillsTootTip(SKILLHEAVYATTACK, p_settingLanguage);
				//ImGui::SameLine();
				if (it.key() == SKILLHEAVYATTACK) // ����ػ��Ĵ���ʽ
				{
					m_textureManager->imguiIMG(PNG_SKILL_HEAVYATTACK_ID, 16, 16);
					ImGui::SameLine();

					ImGui::Text(removeQuotes(p_settingLanguage["Skills"][it.key()].dump()).c_str());
					m_equipmentToolTip->skillsTootTip(SKILLHEAVYATTACK, p_settingLanguage);
					ImGui::SameLine();
					// ��ť�����£�����ѧϰ��Ҫ�۳���ң��������ŵ�����ֿ�
					if (ImGui::SmallButton((removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["ButtonLearn"].dump()).c_str())))
					{
						if (p_save01Data["personMoney"] >= 20)
						{
							// �ػ���ť���ֿ�
							m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", SKILLHEAVYATTACK, 1);

							// ������
							m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 20 );
						}
					}
					// �ػ���toolTip
					if (ImGui::IsItemHovered())
					{
						if (p_save01Data["personMoney"] >= 20) 
						{
							if (ImGui::BeginTooltip())
							{
								ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["ButtonLearnHeavyAttack_ToolTip"].dump()).c_str());
								ImGui::EndTooltip();
							}
						}
						else
						{
							if (ImGui::BeginTooltip())
							{
								ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["NotEnoughGold"].dump()).c_str());
								ImGui::EndTooltip();
							}
						}
					}
				}
			}
		}
	}
	ImGui::End();
}

void SchoolButton::combatTrainFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	//ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 3 - 32), float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	//ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 3 * 2 - 64 - 32), float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowSize(ImVec2(270, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(540, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["SchoolButton"]["CombatTrainFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	if (p_save01Data["HtFloor"] <= 5) { ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str()); }
	ImGui::End();
}

void SchoolButton::returnFrame(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowSize(ImVec2(214, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(810, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin("schoolReturnFrame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	
	// ��λ�����ǻ���retrunFrame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(10, float(p_settingJson["SetWindowHeight"] / 3 + 64 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		p_isReturn = true;
	}
	ImGui::End();
}

