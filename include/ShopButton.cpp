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

	// map中定义武器和装备的属性
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
		ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
		ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["ShopButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());

		// 返回按钮
		// 定位坐标是基于Frame框架内的X,Y坐标的
		ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
		// 返回到HT主场景
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// 三层后，实现一个简单买卖和仓库功能
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	if (p_save01Data["HtFloor"] >= 4)
	{
		// 四层只显示买武器, 
		if (ImGui::Button(removeQuotes(p_settingLanguage["HomeButton"]["StashRoomFrame"]["ButtonWeapons"].dump()).c_str()) )
		{

			for (int i = 0; i < TOTALBUY; i++)
			{
				m_buyButtonArray[i] = false;
			}
			m_buyButtonArray[BUYWEAPONS] = true;
		}

		// 五层后，显示买四件套按钮
		if (p_save01Data["HtFloor"] >= 5)
		{
			// 买四件套按钮
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
		
		// 六层后，显示药水
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明

	ImGui::Begin("shopBuyFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (m_buyButtonArray[BUYWEAPONS])
	{
		for (auto it = m_shopWeaponsJson.begin(); it != m_shopWeaponsJson.end(); ++it)
		{
			if (it.value() == 0) // 0表示在商店
			{
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());  // 武器名称的文本text
				ImGui::SameLine();
				m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // 按钮买
				{
					if (it.key() == BEGINNERSWORD and p_save01Data["personMoney"] >= BeginnerSwordMap["buyGold"]) // 新手剑买入价格定义：20
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerSwordMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 1);  // 放到仓库，1表示仓库
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYHEAD]) 
	{
		for (auto it = m_shopHeadJson.begin(); it != m_shopHeadJson.end(); ++it)
		{
			if (it.value() == 0) // 0表示在商店
			{
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());  // 头盔名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // 稍后
				m_equipmentToolTip->equitHeadToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // 按钮买
				{
					if (it.key() == BEGINNERHELMET and p_save01Data["personMoney"] >= BeginnerHelmetMap["buyGold"]) // 新手头盔买入价格定义：10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerHelmetMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 1);  // 放到仓库，1表示仓库
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYARMS])
	{
		for (auto it = m_shopArmsJson.begin(); it != m_shopArmsJson.end(); ++it)
		{
			if (it.value() == 0) // 0表示在商店
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());  // 手臂名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // 稍后
				m_equipmentToolTip->equitArmsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // 按钮买
				{
					if (it.key() == BEGINNERBARCERS and p_save01Data["personMoney"] >= BeginnerBarcesMap["buyGold"]) // 新手护臂买入价格定义：10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerBarcesMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 1);  // 放到仓库，1表示仓库
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYBODY])
	{
		for (auto it = m_shopBodyJson.begin(); it != m_shopBodyJson.end(); ++it)
		{
			if (it.value() == 0) // 0表示在商店
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());  // 护甲名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage); // 稍后
				m_equipmentToolTip->equitBodyToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // 按钮买
				{
					if (it.key() == BEGINNERBREASTPLATE and p_save01Data["personMoney"] >= BeginnerBreastplateMap["buyGold"]) // 新手护甲买入价格定义：20
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerBreastplateMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 1);  // 放到仓库，1表示仓库
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYLEGS])
	{
		for (auto it = m_shopLegsJson.begin(); it != m_shopLegsJson.end(); ++it)
		{
			if (it.value() == 0) // 0表示在商店
			{
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());  // 护甲名称的文本text
				ImGui::SameLine();
				m_equipmentToolTip->equitLegsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopBuyFrame"]["FrameTitle"].dump()).c_str()))  // 按钮买
				{
					if (it.key() == BEGINNERKNEEPAD and p_save01Data["personMoney"] >= BeginnerKneepadMap["buyGold"]) // 新手护膝买入价格定义：10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] - BeginnerKneepadMap["buyGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 1);  // 放到仓库，1表示仓库
					}
				}
			}
		}
	}

	if (m_buyButtonArray[BUYPOTIONS])
	{
		// 小型健康
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
				// 购买健康药水的逻辑，写配置文件
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
		else  // 不足10的处理方式
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 灰色表示禁用  
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 悬停时颜色不变  
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 按下时颜色不变
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


		// 小型状态
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
				// 购买的状态药水逻辑
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
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 灰色表示禁用  
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 悬停时颜色不变  
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 按下时颜色不变
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// 三层后，实现一个简单买卖和仓库功能
	if (p_save01Data["HtFloor"] <= 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}

	if (p_save01Data["HtFloor"] >= 4)
	{
		// 卖武器, 
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
			// 卖四件套按钮
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin("shopSellFrameEx", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	if (m_sellButtonArray[SELLWEAPONS])
	{
		for (auto it = m_shopWeaponsJson.begin(); it != m_shopWeaponsJson.end(); ++it)
		{
			if (it.value() == 1) // 1表示在仓库
			{
				m_textureManager->imguiIMG(PNG_BEGINEERSWORD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["Weapons"][it.key()].dump()).c_str());  // 武器名称的文本text
				ImGui::SameLine();
				m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // 按钮卖
				{
					if (it.key() == BEGINNERSWORD) // 新手剑卖出价格定义：10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerSwordMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", it.key(), 0);  // 放到商店，0表示商店
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLHEAD])
	{
		for (auto it = m_shopHeadJson.begin(); it != m_shopHeadJson.end(); ++it)
		{
			if (it.value() == 1) // 1表示在仓库
			{
				m_textureManager->imguiIMG(PNG_BEGINNERHELMET_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["HeadEquip"][it.key()].dump()).c_str());  // 头名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitHeadToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // 按钮卖
				{
					if (it.key() == BEGINNERHELMET) // 新手头卖出价格定义：5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerHelmetMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", it.key(), 0);  // 放到商店，0表示商店
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLARMS])
	{
		for (auto it = m_shopArmsJson.begin(); it != m_shopArmsJson.end(); ++it)
		{
			if (it.value() == 1) // 1表示在仓库
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBARCERS_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["ArmsEquip"][it.key()].dump()).c_str());  // 手臂名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitArmsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // 按钮卖
				{
					if (it.key() == BEGINNERBARCERS) // 新手头卖出价格定义：5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerBarcesMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", it.key(), 0);  // 放到商店，0表示商店
					}
				}
			}
		}
	}
	
	if (m_sellButtonArray[SELLBODY])
	{
		for (auto it = m_shopBodyJson.begin(); it != m_shopBodyJson.end(); ++it)
		{
			if (it.value() == 1) // 1表示在仓库
			{
				m_textureManager->imguiIMG(PNG_BEGINNERBREASTPLATE_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["BodyEquip"][it.key()].dump()).c_str());  // 护甲名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitBodyToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // 按钮卖
				{
					if (it.key() == BEGINNERBREASTPLATE) // 新手护甲出价格定义：10
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerBreastplateMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", it.key(), 0);  // 放到商店，0表示商店
					}
				}
			}
		}
	}

	if (m_sellButtonArray[SELLLEGS])
	{
		for (auto it = m_shopLegsJson.begin(); it != m_shopLegsJson.end(); ++it)
		{
			if (it.value() == 1) // 1表示在仓库
			{
				m_textureManager->imguiIMG(PNG_BEGINNERKNEEPAD_ID, 16, 16);
				ImGui::SameLine();

				ImGui::Text(removeQuotes(p_settingLanguage["LegsEquip"][it.key()].dump()).c_str());  // 护甲名称的文本text
				ImGui::SameLine();
				//m_equipmentToolTip->equitWeaponsToolTip(it.key(), p_settingLanguage);
				m_equipmentToolTip->equitLegsToolTip(it.key(), p_settingLanguage);
				if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopSellFrame"]["FrameTitle"].dump()).c_str()))  // 按钮卖
				{
					if (it.key() == BEGINNERKNEEPAD) // 新手腿卖出价格定义：5
					{
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + BeginnerKneepadMap["sellGold"]);
						m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", it.key(), 0);  // 放到商店，0表示商店
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin(removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["FrameTitle"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	if (ImGui::Button(removeQuotes(p_settingLanguage["ShopButton"]["ShopWorkFrame"]["JuniorButtonWork"].dump()).c_str()))
	{
		// 初级打工，状态-20，金币 +10，时间 +1
		if (p_save01Data["personCurrentStatus"] > 20)
		{
			// 处理状态
			int personCurrentStatus = p_save01Data["personCurrentStatus"];
			personCurrentStatus -= 20;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", personCurrentStatus);

			// 处理时间
			int gameTime = p_save01Data["gameTime"];
			gameTime += 1;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", gameTime);

			// 处理金币
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
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin("shopReturnFrame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);


	// 定位坐标是基于retrunFrame框架内的X,Y坐标的
	ImGui::SetCursorPos(ImVec2(10, float(p_settingJson["SetWindowHeight"] / 3 + 64 - 30)));
	// 返回到HT主场景
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		p_isReturn = true;
	}
	ImGui::End();
}