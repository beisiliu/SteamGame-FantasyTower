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
#include"setting/Config.h"

#include"UsualFunc.h"
#include"WriteData.h"
#include"EquipmentToolTip.h"
using json = nlohmann::json;

enum StashRoomButton
{
	WEAPONS,
	HEAD,
	ARMS,
	BODY,
	LEGS,
	SKILLS,
	POTIONS,
	TOTALSTASH
};

class HomeButton {
public:
	HomeButton();
	~HomeButton();
	void homeButtonFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager);
	// �߶�����֮һ, ��ʱ�ֶ���λ
	void bedRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data); // �����128 �߶�����֮һ
	void stashRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data); // �����128 ��ť�������EX����ʾ������
	void stashRoomFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager *m_textureManager); // �����256
	void workRoomFrame(json p_settingJson, json p_settingLanguage, json p_save01Data); // �����128 ��ť�������EX����ʾ������
	void workRoomFrameEx(json p_settingJson, json p_settingLanguage); // �����256
	void returnFrame(json p_settingJson, json p_settingLanguage); // �����128
	void isPicSkills(std::string skillName, TextureManager *m_textureManager); //��������ͼ����ж�

	bool p_isReturn = false; // ���ڷ��ذ�ť
	bool p_isSleepAnimation = false;

	bool p_isAnimationZ128 = false;
	bool p_isAnimationZ64 = false;
	bool p_isAnimationZ32 = false;
	int p_animationZCount = 0; // �������ƶ����ٶ�
	int p_animationZLoopNum = 0; // ����ѭ������

private:
	//bool isOpen = true;
	WriteData* m_writeData;
	EquipmentToolTip* m_equipmentToolTip;

	std::array<bool, TOTALSTASH> m_statshButtonStatusArray;
	bool m_isWeapons = false;
};