#pragma once

#include<SDL.h>
#include<string>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/Config.h"
#include"TextureManager.h"
#include"WriteData.h"
#include"EquipmentToolTip.h"

#include"UsualFunc.h"

using json = nlohmann::json;

class SchoolButton {
public:
	SchoolButton();
	~SchoolButton();
	void schoolButtonFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager);
	void baseTrainFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data);
	void skillTrainFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data, TextureManager* m_textureManager);
	void combatTrainFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data);
	void returnFrame(json p_settingJson, json p_settingLanguage); // �����128
	
	// ���������ƶ�����
	bool p_isTrainAnimation = false;
	int p_personX = 0; // �����ƶ�X�����
	int p_personFrame = 0; // �����ƶ���ͼƬ֡������
	int p_personXSpeed = 10;

	bool p_isReturn = false;
private:
	//bool isOpen = true;
	int m_juniorStrengthAndDefenseLimits = 10;
	int m_juniorSpeedLimits = 5;
	int m_juniorHealthAndStatusLimits = 100;
	WriteData* m_writeData;
	EquipmentToolTip* m_equipmentToolTip;

	// ����
	json skillJson;

};