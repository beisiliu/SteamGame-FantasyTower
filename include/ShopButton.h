#pragma once

#include<SDL.h>
#include<string>
#include<array>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/Config.h"
#include"TextureManager.h"
#include"WriteData.h"
#include"UsualFunc.h"
#include"EquipmentToolTip.h"

#include"Weapons.h"
#include"HeadEquip.h"
#include"ArmEquip.h"
#include"bodyEquip.h"
#include"LegsEquip.h"

using json = nlohmann::json;

enum BuyButton
{
	BUYWEAPONS,
	BUYHEAD,
	BUYARMS,
	BUYBODY,
	BUYLEGS,
	BUYPOTIONS,
	TOTALBUY
};

enum SellButton
{
	SELLWEAPONS,
	SELLHEAD,
	SELLARMS,
	SELLBODY,
	SELLLEGS,
	SELLPOTIONS,
	TOTALSELL
};

class ShopButton {
public:
	ShopButton();
	~ShopButton();
	void shopDataInit(json p_save01Data);
	void shopButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager);
	void shopBuyFrame(json p_settingJson, json p_settingLanguage, json p_save01Data);
	void shopBuyFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager);
	// void shopSellFrame(json p_settingJson, json p_settingLanguage, json p_save01Data);
	// void shopSellFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager);
	void shopWorkFrame(json p_settingJson, json p_settingLanguage, json p_save01Data);
	void returnFrame(json p_settingJson, json p_settingLanguage);

	// ����򹤶���
	bool p_isShopAnimation = false;
	int p_shopAnimationX = 0;
	int p_shopAnimationY = 0;
	int p_shopAnimationSpeed = 5;

	bool p_isReturn = false;
private:
	WriteData* m_writeData;
	EquipmentToolTip* m_equipmentToolTip;

	std::array<bool, TOTALBUY> m_buyButtonArray;
	std::array<bool, TOTALSELL> m_sellButtonArray;

	json m_shopWeaponsJson;
	json m_shopHeadJson;
	json m_shopArmsJson;
	json m_shopBodyJson;
	json m_shopLegsJson;

	// ��������
	std::map<std::string, int> BeginnerSwordMap;

	// ͷ��װ������
	std::map<std::string, int> BeginnerHelmetMap;

	// �ֱ�װ������
	std::map<std::string, int> BeginnerBarcesMap;

	// ����װ������
	std::map<std::string, int> BeginnerBreastplateMap;

	// �Ȳ�װ������
	std::map<std::string, int> BeginnerKneepadMap;
};
