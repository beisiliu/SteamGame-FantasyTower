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
	void initPersonData(json p_save01Data);		// ��ʼ����������
	void personButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager);
	int calPersonCurrentAccack();  // ��������Ĺ�����
	int calPersonCurrentDefense(); // ��������ķ�����
	int calPersonCurrentSpeed();   // ����������ٶ�
	void attributeFame(json p_settingJson, json p_settingLanguage, json p_save01Data); // �������Կ��
	void equipmentFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager *m_textureManager); // װ�����Կ��
	void skillFame(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager); // �������Կ��
	void isPicSkills(std::string skillName, TextureManager* m_textureManager);
	void recordPoint(json p_settingJson, json p_settingLanguage, json p_save01Data);
private:
	WriteData* m_writeData;
	EquipmentToolTip* m_equipmentToopTip;

	// bool isOpen = true;
	// �����ж���Ӧ�İ�ť�Ƿ��£����º���Ӧ�Ĵ���Ҫ��
	bool isOpenAttribute = false; 
	bool isOpenEquipment = false;
	bool isOpenSkill = false;
	bool isRecordPoint = false;

	// ��������
	int m_personCurrentHealth = 0;
	int m_personAllHealth = 0;
	int m_personCurrentStatus = 0;
	int m_personAllStatus = 0;
	// ����������
	int m_personAttributeStrength = 0;
	int m_personAttributeDefense = 0;
	int m_personAttributeSpeed = 0;

	// ����Ѫ����״̬�ٷֱ�
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