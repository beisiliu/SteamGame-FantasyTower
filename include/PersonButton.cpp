#include"PersonButton.h"

PersonButton::PersonButton()
{
	m_writeData = new WriteData();
	m_equipmentToopTip = new EquipmentToolTip();

	BeginnerSwordMap = setBeginnerSword();
	BeginnerHelmetMap = setBeginnerHelmet();
	BeginnerBarcesMap = setBeginnerBarces();
	BeginnerBreastplateMap = setBeginnerBreastplate();
	BeginnerKneepadMap = setBeginnerKneepad();
}

PersonButton::~PersonButton()
{
	delete m_writeData;
	delete m_equipmentToopTip;
}

void PersonButton::initPersonData(json p_save01Data)
{
	// Ѫ����״̬����ʼ��
	m_personCurrentHealth = p_save01Data["personCurrentHealth"];
	m_personAllHealth = p_save01Data["personAllHealth"];
	m_personCurrentStatus = p_save01Data["personCurrentStatus"];
	m_personAllStatus = p_save01Data["personAllStatus"];

	// �������Գ�ʼ��
	m_personAttributeStrength = p_save01Data["psersonStrength"];
	m_personAttributeDefense = p_save01Data["personDefense"];
	m_personAttributeSpeed = p_save01Data["personSpeed"];

	// ����Ѫ��ֵ��״ֵ̬�İٷֱ�
	m_personHealthPersent = (m_personCurrentHealth / m_personAllHealth) * 100;
	m_personStatusPersent = (static_cast<float>(m_personCurrentStatus) / m_personAllStatus) * 100;

	// ��ȡsave01�ļ�����ȡ��������
	m_weaponsJson = p_save01Data["Weapons"];
	m_headJson = p_save01Data["HeadEquip"];
	m_armsJson = p_save01Data["ArmsEquip"];
	m_bodyJson = p_save01Data["BodyEquip"];
	m_legsJson = p_save01Data["LegsEquip"];
	m_skillJson = p_save01Data["personSkill"];
}

int PersonButton::calPersonCurrentAccack()
{
	int personCurrenAttack = 0;
	// ��ȡ�������˺�
	std::string weaponsName = "";
	// ���ǵ�����p_save01Data["Weapons"]�£����кܶ��������keyΪ��������ͨ��value���жϸ��������̵�(0)���ֿ�(1)��װ����(2)
	for (auto it = m_weaponsJson.begin(); it != m_weaponsJson.end(); ++it)
	{
		if (it.value() == 2)
		{
			weaponsName = it.key();
		}
	}

	// ���ֽ�����+1
	if (weaponsName == BEGINNERSWORD)
	{
		personCurrenAttack = BeginnerSwordMap["attack"];
	}

	// �������״̬ < 50%, ���﹥���� * 0.8
	if (m_personStatusPersent < 50)
	{
		personCurrenAttack = personCurrenAttack + int(m_personAttributeStrength * 0.8);
		return personCurrenAttack;
	}
	return (personCurrenAttack + m_personAttributeStrength);
}

int PersonButton::calPersonCurrentDefense()
{
	int personCurrenDefense = 0;
	// ����ͷ������
	std::string headName = "";
	for (auto it = m_headJson.begin(); it != m_headJson.end(); ++it)
	{
		if (it.value() == 2)
		{
			headName = it.key();
		}
	}
	if (headName == BEGINNERHELMET)
	{
		personCurrenDefense += BeginnerHelmetMap["defense"];
	}

	// �����ֱ۷���
	std::string armsName = "";
	for (auto it = m_armsJson.begin(); it != m_armsJson.end(); ++it)
	{
		if (it.value() == 2)
		{
			armsName = it.key();
		}
	}
	if (armsName == BEGINNERBARCERS)
	{
		personCurrenDefense += BeginnerBarcesMap["defense"];
	}

	// �����׷���
	std::string bodyName = "";
	for (auto it = m_bodyJson.begin(); it != m_bodyJson.end(); ++it)
	{
		if (it.value() == 2)
		{
			bodyName = it.key();
		}
	}
	if (bodyName == BEGINNERBREASTPLATE)
	{
		personCurrenDefense += BeginnerBreastplateMap["defense"];
	}

	// �����ȷ���
	std::string legsName = "";
	for (auto it = m_legsJson.begin(); it != m_legsJson.end(); ++it)
	{
		if (it.value() == 2)
		{
			legsName = it.key();
		}
	}
	if (legsName == BEGINNERKNEEPAD)
	{
		personCurrenDefense += BeginnerKneepadMap["defense"];
	}

	// �������״̬ < 50, ��������� * 0.8
	if (m_personStatusPersent < 50)
	{
		personCurrenDefense = personCurrenDefense + int(m_personAttributeDefense * 0.8);
		return personCurrenDefense;
	}

	return (personCurrenDefense + m_personAttributeDefense);
}

int PersonButton::calPersonCurrentSpeed()
{
	// �������״̬ < 50, �����ٶ� * 0.5
	if (m_personStatusPersent < 50)
	{
		return int(m_personAttributeSpeed * 0.5);
	}
	return m_personAttributeSpeed;
}

void PersonButton::personButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager)
{
	//isOpen = true;
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["PersonButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	// ���ﵱǰ����
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CombatCapability"].dump()).c_str());
	ImGui::Text("-------------");
	// ���ﵱǰ���� - ��ǰ������
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentAttack"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(calPersonCurrentAccack()).c_str());
	// ���ﵱǰ���� - ��ǰ������
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentDefense"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(calPersonCurrentDefense()).c_str());
	// ���ﵱǰ���� - ��ǰ�ٶ�
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentSpeed"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(calPersonCurrentSpeed()).c_str());
	ImGui::Text(" "); // ������
	ImGui::Text(" ");

	// �������Ե�������ť������������Ⱦ
	if (ImGui::Button(removeQuotes(p_settingLanguage["PersonButton"]["PersonAttribute"]["PersonAttribute"].dump()).c_str(), ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) - 16, 32)))
	{
		isRecordPoint = false;
		isOpenEquipment = false;
		isOpenSkill = false;
		isOpenAttribute = true;
	}
	
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text("--------------");
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	// �Ĳ��Ժ󿪣�װ��
	if (p_save01Data["HtFloor"] >= 4)
	{
		// ����װ����������ť������������Ⱦ
		if (ImGui::Button(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonEquipment"].dump()).c_str(), ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) - 16, 32)))
		{
			isOpenAttribute = false;
			isRecordPoint = false;
			isOpenSkill = false;
			isOpenEquipment = true;
		}

		if (p_save01Data["HtFloor"] == 4)
		{
			ImGui::Text("--------------");
			ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
		}
	}

	// ����Ժ󿪣�����
	if (p_save01Data["HtFloor"] >= 6)
	{
		// ���＼�ܵ�������ť������������Ⱦ
		if (ImGui::Button(removeQuotes(p_settingLanguage["PersonButton"]["PersonSkill"]["PersonSkill"].dump()).c_str(), ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) - 16, 32)))
		{
			isOpenAttribute = false;
			isOpenEquipment = false;
			isRecordPoint = false;
			isOpenSkill = true;
		}

		if (ImGui::Button(removeQuotes(p_settingLanguage["PersonButton"]["RecordPoint"]["RecordPoint"].dump()).c_str(), ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) - 16, 32)))
		{
			isOpenAttribute = false;
			isOpenEquipment = false;
			isOpenSkill = false;
			isRecordPoint = true;
		}

		ImGui::Text("--------------");
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	// ���ذ�ť
	// ��λ�����ǻ���Frame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		//isOpen = false;
		IsWindowStatus = false;

		// �ر����п�
		isOpenAttribute = false;
		isOpenEquipment = false;
		isOpenSkill = false;
	}

	ImGui::End();

	attributeFame(p_settingJson, p_settingLanguage, p_save01Data);  // �������Կ��
	equipmentFame(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);  // ����װ�����
	skillFame(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager); // ���＼�ܿ��
	recordPoint(p_settingJson, p_settingLanguage, p_save01Data);
}

void PersonButton::attributeFame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	if (isOpenAttribute)
	{
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4 + 64), float( (p_settingJson["SetWindowHeight"] - 192)/2 ) ));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) + 64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["PersonButton"]["PersonAttribute"]["PersonAttribute"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		// ������������
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonAttribute"]["Strength"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_personAttributeStrength).c_str());
		// �����������
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonAttribute"]["Defense"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_personAttributeDefense).c_str());
		//// �����ٶ�����
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonAttribute"]["Speed"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_personAttributeSpeed).c_str());

		ImGui::End();
	}
}

void PersonButton::equipmentFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager *m_textureManager)
{
	if (isOpenEquipment)
	{
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4 + 64), float((p_settingJson["SetWindowHeight"] - 192) / 2)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) + 64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonEquipment"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		// ˵��������԰�Ĳֿ���Ҫװ��װ��, ���ú�ɫ
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonEquipmentInfo01"].dump()).c_str());
		ImGui::PopStyleColor(1);

		ImGui::Text("--------");
		// ��������
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonWeapons"].dump()).c_str());
		for (auto it = m_weaponsJson.begin(); it != m_weaponsJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				// ���ֽ���ͼ�꣬ ����Ҫ���ж�
				ImGui::SameLine();
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				
				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());
				ImGui::SameLine();
				//tooltip
				m_equipmentToopTip->equitWeaponsToolTip(it.key(), p_settingLanguage);

				//ж�ذ�ť
				ImGui::PushID(m_weaponsUnmountButtonID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 1);
				}
				ImGui::PopID();
			}
		}
		// ����ͷ��
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonHead"].dump()).c_str());
		for (auto it = m_headJson.begin(); it != m_headJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				ImGui::SameLine();
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);

				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				//tooltip
				// m_equipmentToopTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToopTip->equitHeadToolTip(it.key(), p_settingLanguage);
				//ж�ذ�ť
				ImGui::PushID(m_headUnmountButtonID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 1);
				}
				ImGui::PopID();
			}
		}
		// �����ֱ�
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonArms"].dump()).c_str());
		for (auto it = m_armsJson.begin(); it != m_armsJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				ImGui::SameLine();
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);

				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				//tooltip
				// m_equipmentToopTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToopTip->equitArmsToolTip(it.key(), p_settingLanguage);
				//ж�ذ�ť
				ImGui::PushID(m_armsUnmountButtonID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 1);
				}
				ImGui::PopID();
			}
		}
		// ��������
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonBody"].dump()).c_str());
		for (auto it = m_bodyJson.begin(); it != m_bodyJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				ImGui::SameLine();
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);

				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				//tooltip
				// m_equipmentToopTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToopTip->equitBodyToolTip(it.key(), p_settingLanguage);
				//ж�ذ�ť
				ImGui::PushID(m_bodyUnmountButtonID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 1);
				}
				ImGui::PopID();
			}
		}
		// �����Ȳ�
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["PersonLegs"].dump()).c_str());
		for (auto it = m_legsJson.begin(); it != m_legsJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				ImGui::SameLine();
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);

				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());
				ImGui::SameLine();
				//tooltip
				// m_equipmentToopTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToopTip->equitLegsToolTip(it.key(), p_settingLanguage);
				//ж�ذ�ť
				ImGui::PushID(m_LegsUnmountButtonID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 1);
				}
				ImGui::PopID();
			}
		}

		ImGui::End();
	}
}

void PersonButton::skillFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager)
{
	if (isOpenSkill)
	{
		// ж�ذ�ť��ID
		int buttonUmountID = 0;

		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4 + 64), float((p_settingJson["SetWindowHeight"] - 192) / 2)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) + 64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["PersonButton"]["PersonSkill"]["PersonSkill"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		// ��Я����������
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonSkill"]["PersonSkillNums"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text(p_save01Data["personSkillNums"].dump().c_str());
		ImGui::Text("--------"); // �ָ���

		// ˵��������԰�Ĳֿ���Ҫ����װ��, ���ú�ɫ
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["PersonSkill"]["PersonSkillInfo01"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text("--------"); // �ָ���

		// ��ʾ��װ���ļ���
		for (auto it = m_skillJson.begin(); it != m_skillJson.end(); ++it)
		{
			if (it.value() == 2)
			{
				
				isPicSkills(it.key(), m_textureManager);
				buttonUmountID += 1;  // ��ֹ��ť�����ظ��������ĵ����������
				// �������ܺͰ�ť����ʾ��Ⱦ
				ImGui::Text(removeQuotes(p_settingLanguage["Skills"][it.key()].dump()).c_str());
				m_equipmentToopTip->skillsTootTip(it.key(), p_settingLanguage);
				ImGui::SameLine();

				// ж��װ����ť
				ImGui::PushID(buttonUmountID);
				if (ImGui::SmallButton(removeQuotes(p_settingLanguage["PersonButton"]["PersonEquipment"]["Unmount"].dump()).c_str()))
				{
					m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", it.key(), 1);
					// ��װ���������� -1
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentSkillNums", p_save01Data["personCurrentSkillNums"] - 1);
				}
				ImGui::PopID();

			}
		}

		ImGui::End();
	}
}

void PersonButton::isPicSkills(std::string skillName, TextureManager* m_textureManager)
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

void PersonButton::recordPoint(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	if (isRecordPoint)
	{
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 4 + 64), float((p_settingJson["SetWindowHeight"] - 192) / 2)));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) + 64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["PersonButton"]["RecordPoint"]["RecordPoint"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		// �����߼�
		if (std::filesystem::exists("save/save01_5F.bin"))
		{
			if (ImGui::Button(removeQuotes(p_settingLanguage["PersonButton"]["RecordPoint"]["GoTo5Floor"].dump()).c_str(), ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3) - 32, 32)))
			{
				// ɾ��save01.bin����save01_5F.bin������Ϊsave01.bin
				removeFile("save/save01.bin");
				renameFile(L"save/save01_5F.bin", L"save/save01.bin");
			}
		}

		ImGui::End();
	}
}