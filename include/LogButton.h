#pragma once

#include<SDL.h>
#include<string>
#include<vector>
#include"EnemyAttribute.h"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/Config.h"
#include"TextureManager.h"

#include"UsualFunc.h"

using json = nlohmann::json;

class LogButton {
public:
	LogButton();
	void logButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager* m_textureManager);
	void logButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data);
	void floorButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data);
	void gameHintButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data);
	void enemyButtonFrameEx(json p_settingJson, json p_settingLanguage, json p_save01Data, TextureManager* m_textureManager);
	void enemyInformation(int floorNum, TextureManager* m_textureManager, json p_settingLanguage);
private:
	bool isLogButton = false;
	bool isGameHintButton = false;
	bool isEnemyButton = false;

	// 记录层数按钮的状态，暂时初始化10层
	std::vector<bool> floorButtonVector;

	EnemyAttribute* m_enemyAttribute = nullptr;

};
