#include "ShopButton.h"

ShopButton::ShopButton()
{
	m_writeData = new WriteData();
	m_equipmentToolTip = new EquipmentToolTip();

	for (int i = 0; i < TOTALBUY; i++) 
	{
		m_buyButtonArray[i] = false;
	}

	for (int i = 0; i < TOTALSELL; i++)
	{
		m_sellButtonArray[i] = false;
	}

	// map�ж���������װ��������
	BeginnerSwordMap = setBeginnerSword();
	BeginnerHelmetMap = setBeginnerHelmet();
	BeginnerBarcesMap = setBeginnerBarces();
	BeginnerBreastplateMap = setBeginnerBreastplate();
	BeginnerKneepadMap = setBeginnerKneepad();
}

ShopButton::~ShopButton()
{
	delete m_writeData;
	delete m_equipmentToolTip;
}

void ShopButton::shopDataInit(json p_save01Data)
{
	m_shopWeaponsJson = p_save01Data["Weapons"];
	m_shopHeadJson = p_save01Data["HeadEquip"];
	m_shopArmsJson = p_save01Data["ArmsEquip"];
	m_shopBodyJson = p_save01Data["BodyEquip"];
	m_shopLegsJson = p_save01Data["LegsEquip"];
}

void ShopButton::shopButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager* m_textureManager)
{
	
	if (p_save01Data["HtFloor"] <= 2)
	{
		//isOpen = true;
		ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
		ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
		ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
		ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["ShopButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		
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
		if (!p_isShopAnimation)
		{	
			p_shopAnimationX = p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8;
			p_shopAnimationY = p_settingJson["SetWindowHeight"] / 2 - 84;
			shopDataInit(p_save01Data);
			shopBuyFrame(p_settingJson, p_settingLanguage, p_save01Data);
			shopBuyFrameEx(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);
			// shopSellFrame(p_settingJson, p_settingLanguage, p_save01Data);
			// shopSellFrameEx(p_settingJson, p_settingLanguage, p_save01Data, m_textureManager);
			shopWorkFrame(p_settingJson, p_settingLanguage, p_save01Data);
			returnFrame(p_settingJson, p_settingLanguage);
		}

	}
	
}

void ShopButton::shopBuyFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(192.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// �����ʵ��һ���������Ͳֿ⹦��
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	if (p_save01Data["HtFloor"] >= 4)
	{
		// �Ĳ�ֻ��ʾ������, 
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonWeapons"].dump()).c_str()) )
		{

			for (int i = 0; i < TOTALBUY; i++)
			{
				m_buyButtonArray[i] = false;
			}
			m_buyButtonArray[BUYWEAPONS] = true;
		}

		// ������ʾ���ļ��װ�ť
		if (p_save01Data["HtFloor"] >= 5)
		{
			// ���ļ��װ�ť
			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonHead"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALBUY; i++)
				{
					m_buyButtonArray[i] = false;
				}
				m_buyButtonArray[BUYHEAD] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonArms"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALBUY; i++)
				{
					m_buyButtonArray[i] = false;
				}
				m_buyButtonArray[BUYARMS] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonBody"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALBUY; i++)
				{
					m_buyButtonArray[i] = false;
				}
				m_buyButtonArray[BUYBODY] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonLegs"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALBUY; i++)
				{
					m_buyButtonArray[i] = false;
				}
				m_buyButtonArray[BUYLEGS] = true;
			}

		}
		
		// �������ʾҩˮ
		if (p_save01Data["HtFloor"] >= 7)
		{
			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonPotions"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALBUY; i++)
				{
					m_buyButtonArray[i] = false;
				}
				m_buyButtonArray[BUYPOTIONS] = true;
			}
		}
	}


	ImGui::End();
}

void ShopButton::shopBuyFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager)
{
	ImGui::SetNextWindowSize(ImVec2(384.0f, float( p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(192, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��

	ImGui::Begin("shopBuyFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (m_buyButtonArray[BUYWEAPONS])
	{
		for (auto it = m_shopWeaponsJson.begin(); it != m_shopWeaponsJson.end(); ++it)
		{
			if (it.value() == 0) // 0��ʾ���̵�
			{
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERSWORD and p_save01Data["personMoney"] >= BeginnerSwordMap["buyGold"]) // ���ֽ�����۸��壺20
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerSwordMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 1);  // �ŵ��ֿ⣬1��ʾ�ֿ�
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYHEAD]) 
	{
		for (auto it = m_shopHeadJson.begin(); it != m_shopHeadJson.end(); ++it)
		{
			if (it.value() == 0) // 0��ʾ���̵�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());  // ͷ�����Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // �Ժ�
				m_equipmentToolTip->equitHeadToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERHELMET and p_save01Data["personMoney"] >= BeginnerHelmetMap["buyGold"]) // ����ͷ������۸��壺10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerHelmetMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 1);  // �ŵ��ֿ⣬1��ʾ�ֿ�
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYARMS])
	{
		for (auto it = m_shopArmsJson.begin(); it != m_shopArmsJson.end(); ++it)
		{
			if (it.value() == 0) // 0��ʾ���̵�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());  // �ֱ����Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // �Ժ�
				m_equipmentToolTip->equitArmsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERBARCERS and p_save01Data["personMoney"] >= BeginnerBarcesMap["buyGold"]) // ���ֻ�������۸��壺10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerBarcesMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 1);  // �ŵ��ֿ⣬1��ʾ�ֿ�
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYBODY])
	{
		for (auto it = m_shopBodyJson.begin(); it != m_shopBodyJson.end(); ++it)
		{
			if (it.value() == 0) // 0��ʾ���̵�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // �Ժ�
				m_equipmentToolTip->equitBodyToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERBREASTPLATE and p_save01Data["personMoney"] >= BeginnerBreastplateMap["buyGold"]) // ���ֻ�������۸��壺20
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerBreastplateMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 1);  // �ŵ��ֿ⣬1��ʾ�ֿ�
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYLEGS])
	{
		for (auto it = m_shopLegsJson.begin(); it != m_shopLegsJson.end(); ++it)
		{
			if (it.value() == 0) // 0��ʾ���̵�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				m_equipmentToolTip->equitLegsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERKNEEPAD and p_save01Data["personMoney"] >= BeginnerKneepadMap["buyGold"]) // ���ֻ�ϥ����۸��壺10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerKneepadMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 1);  // �ŵ��ֿ⣬1��ʾ�ֿ�
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYPOTIONS])
	{
		// С�ͽ���
		m_textureManager->imguiIMG(PNG_POTION_SMALLHEALTHPOTION_ID, 16, 16);
		ImGui::SameLine();

		ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallHealthPotion"].dump()).c_str());
		ImGui::SameLine();
		if (ImGui::IsItemHovered())
		{
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallHealthPotionToolTip"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}

		if (p_save01Data["personMoney"] >= 10)
		{
			ImGui::PushID(1);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))
			{
				// ���򽡿�ҩˮ���߼���д�����ļ�
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallHealthPotionCount", p_save01Data["smallHealthPotionCount"] + 1);
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 10);

			}
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["BuyAndSellGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}
		}
		else  // ����10�Ĵ���ʽ
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ɫ��ʾ����  
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ͣʱ��ɫ����  
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ����ʱ��ɫ����
			ImGui::SmallButton(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str());
			ImGui::PopStyleColor(3);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["NotEnoughGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}

		}


		// С��״̬
		m_textureManager->imguiIMG(PNG_POTION_SMALLSTATUSPOTION_ID, 16, 16);
		ImGui::SameLine();

		ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallStatusPotion"].dump()).c_str());
		ImGui::SameLine();
		if (ImGui::IsItemHovered())
		{
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallStatusPotionToolTip"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}

		if (p_save01Data["personMoney"] >= 10)
		{
			ImGui::PushID(2);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))
			{
				// �����״̬ҩˮ�߼�
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallStatusPotionCount", p_save01Data["smallStatusPotionCount"] + 1);
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - 10);
			}
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["BuyAndSellGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ɫ��ʾ����  
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ͣʱ��ɫ����  
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ����ʱ��ɫ����
			ImGui::SmallButton(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str());
			ImGui::PopStyleColor(3);

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(removeQuotes(p_settingLanguage["SchoolButton"]["SkillTrainFrame"]["NotEnoughGold"].dump()).c_str());
					ImGui::EndTooltip();
				}
			}
		}

	}


	ImGui::End();
}

/*
void ShopButton::shopSellFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(192.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(896 - 128 - 384, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// �����ʵ��һ���������Ͳֿ⹦��
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	if (p_save01Data["HtFloor"] >= 4)
	{
		// ������, 
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonWeapons"].dump()).c_str()))
		{
			for (int i = 0; i < TOTALSELL; i++)
			{
				m_sellButtonArray[i] = false;
			}
			m_sellButtonArray[SELLWEAPONS] = true;
		}

		if (p_save01Data["HtFloor"] >= 5)
		{
			// ���ļ��װ�ť
			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonHead"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALSELL; i++)
				{
					m_sellButtonArray[i] = false;
				}
				m_sellButtonArray[SELLHEAD] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonArms"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALSELL; i++)
				{
					m_sellButtonArray[i] = false;
				}
				m_sellButtonArray[SELLARMS] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonBody"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALSELL; i++)
				{
					m_sellButtonArray[i] = false;
				}
				m_sellButtonArray[SELLBODY] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonLegs"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALSELL; i++)
				{
					m_sellButtonArray[i] = false;
				}
				m_sellButtonArray[SELLLEGS] = true;
			}

			if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonPotions"].dump()).c_str()))
			{
				for (int i = 0; i < TOTALSELL; i++)
				{
					m_sellButtonArray[i] = false;
				}
				m_sellButtonArray[SELLPOTIONS] = true;
			}
		}
	}


	ImGui::End();
}

void ShopButton::shopSellFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager)
{
	ImGui::SetNextWindowSize(ImVec2(192.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(896 - 128 - 192, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin("shopSellFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (m_sellButtonArray[SELLWEAPONS])
	{
		for (auto it = m_shopWeaponsJson.begin(); it != m_shopWeaponsJson.end(); ++it)
		{
			if (it.value() == 1) // 1��ʾ�ڲֿ�
			{
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERSWORD) // ���ֽ������۸��壺10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerSwordMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 0);  // �ŵ��̵꣬0��ʾ�̵�
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLHEAD])
	{
		for (auto it = m_shopHeadJson.begin(); it != m_shopHeadJson.end(); ++it)
		{
			if (it.value() == 1) // 1��ʾ�ڲֿ�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());  // ͷ���Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitHeadToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERHELMET) // ����ͷ�����۸��壺5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerHelmetMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 0);  // �ŵ��̵꣬0��ʾ�̵�
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLARMS])
	{
		for (auto it = m_shopArmsJson.begin(); it != m_shopArmsJson.end(); ++it)
		{
			if (it.value() == 1) // 1��ʾ�ڲֿ�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());  // �ֱ����Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitArmsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERBARCERS) // ����ͷ�����۸��壺5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerBarcesMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 0);  // �ŵ��̵꣬0��ʾ�̵�
					}
				}
			}
		}
	}
	
	if (m_sellButtonArray[SELLBODY])
	{
		for (auto it = m_shopBodyJson.begin(); it != m_shopBodyJson.end(); ++it)
		{
			if (it.value() == 1) // 1��ʾ�ڲֿ�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitBodyToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERBREASTPLATE) // ���ֻ��׳��۸��壺10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerBreastplateMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 0);  // �ŵ��̵꣬0��ʾ�̵�
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLLEGS])
	{
		for (auto it = m_shopLegsJson.begin(); it != m_shopLegsJson.end(); ++it)
		{
			if (it.value() == 1) // 1��ʾ�ڲֿ�
			{
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());  // �������Ƶ��ı�text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitLegsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // ��ť��
				{
					if (it.key() == BEGINNERKNEEPAD) // �����������۸��壺5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerKneepadMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 0);  // �ŵ��̵꣬0��ʾ�̵�
					}
				}
			}
		}
	}

	ImGui::End();
}
*/
void ShopButton::shopWorkFrame(json p_settingJson, json p_settingLanguage, json p_save01Data)
{
	ImGui::SetNextWindowSize(ImVec2(320.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(float(896 - 320), float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["JuniorButtonWork"].dump()).c_str()))
	{
		// �����򹤣�״̬-20����� +10��ʱ�� +1
		if (p_save01Data["personCurrentStatus"] > 20)
		{
			// ����״̬
			int personCurrentStatus = p_save01Data["personCurrentStatus"];
			personCurrentStatus -= 20;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", personCurrentStatus);

			// ����ʱ��
			int gameTime = p_save01Data["gameTime"];
			gameTime += 1;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", gameTime);

			// ������
			int personGold = p_save01Data["personMoney"];
			personGold += 10;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", personGold);

			p_isShopAnimation = true;
		}
	}
	if (ImGui::IsItemHovered())
	{
		if (p_save01Data["personCurrentStatus"] >= 20)
		{
			std::string juniorWorkTooltip = removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["JuniorButtonWorkToolTip1"].dump());
			if (ImGui::BeginTooltip())
			{
				ImGui::Text(juniorWorkTooltip.c_str());
				ImGui::EndTooltip();
			}
		}
		else
		{
			if (ImGui::BeginTooltip())
			{

				ImGui::Text(removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["JuniorButtonWorkToolTip2"].dump()).c_str());
				ImGui::EndTooltip();
			}
		}

	}

	ImGui::End();
}

void ShopButton::returnFrame(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowSize(ImVec2(128.0f, float(p_settingJson["SetWindowHeight"] / 3 + 64)));
	ImGui::SetNextWindowPos(ImVec2(896, float(p_settingJson["SetWindowHeight"] / 3 * 2 - 128)));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin("shopReturnFrame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);


	// ��λ�����ǻ���retrunFrame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(10, float(p_settingJson["SetWindowHeight"] / 3 + 64 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		p_isReturn = true;
	}
	ImGui::End();
}