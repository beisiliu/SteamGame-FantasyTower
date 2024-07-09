#pragma once
#include<SDL.h>
#include<string>
#include<set>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/Config.h"
#include"TextureManager.h"
#include"WriteData.h"
#include"EquipmentToolTip.h"

#include"UsualFunc.h"

#include"Weapons.h"
#include"HeadEquip.h"
#include"ArmEquip.h"
#include"bodyEquip.h"
#include"LegsEquip.h"

using json = nlohmann::json;
class PersonButton
{
public:
	PersonButton();
	~PersonButton();
	void initPersonData(json p_save01Data);		// 初始化人物数据
	void personButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager);
	int calPersonCurrentAccack();  // 计算人物的攻击力
	int calPersonCurrentDefense(); // 计算人物的防御力
	int calPersonCurrentSpeed();   // 计算人物的速度
	void attributeFame(json p_settingJson, json p_settingLanguage, json p_save01Data); // 人物属性框架
	void equipmentFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager *m_textureManager); // 装备属性框架
	void skillFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager); // 技能属性框架
	void isPicSkills(std::string skillName, TextureManager* m_textureManager);
	void recordPoint(json p_settingJson, json p_settingLanguage, json p_save01Data);
private:
	WriteData* m_writeData;
	EquipmentToolTip* m_equipmentToopTip;

	// bool isOpen = true;
	// 用来判断相应的按钮是否按下，按下后，相应的窗口要打开
	bool isOpenAttribute = false; 
	bool isOpenEquipment = false;
	bool isOpenSkill = false;
	bool isRecordPoint = false;

	// 人物属性
	int m_personCurrentHealth = 0;
	int m_personAllHealth = 0;
	int m_personCurrentStatus = 0;
	int m_personAllStatus = 0;
	// 人物三属性
	int m_personAttributeStrength = 0;
	int m_personAttributeDefense = 0;
	int m_personAttributeSpeed = 0;

	// 人物血量和状态百分比
	int m_personHealthPersent = 0;
	int m_personStatusPersent = 0;

	json m_weaponsJson;
	json m_headJson;
	json m_armsJson;
	json m_bodyJson;
	json m_legsJson;
	json m_skillJson;

	// button id
	int m_weaponsUnmountButtonID = 1;
	int m_headUnmountButtonID = 2;
	int m_armsUnmountButtonID = 3;
	int m_bodyUnmountButtonID = 4;
	int m_LegsUnmountButtonID = 5;

	// 武器属性
	std::map<std::string, int> BeginnerSwordMap;

	// 头部装备属性
	std::map<std::string, int> BeginnerHelmetMap;

	// 手臂装备属性
	std::map<std::string, int> BeginnerBarcesMap;

	// 身体装备属性
	std::map<std::string, int> BeginnerBreastplateMap;

	// 腿部装备属性
	std::map<std::string, int> BeginnerKneepadMap;

};