#pragma once

#include<SDL.h>
#include<array>
#include<iostream>
#include<fstream>
#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/config.h"
#include"TextureManager.h"
#include"UsualFunc.h"
#include"WriteData.h"

// 按钮类
#include"PersonButton.h"
#include"HomeButton.h"
#include"SchoolButton.h"
#include"ShopButton.h"
#include"HarvestButton.h"
#include"LogButton.h"
#include"AchievementsButton.h"

// steam接口
#include"StreamAPIAchievements.h"

using json = nlohmann::json;

enum MainMenuStatus
{
	PERSONMENUSTATUS,
	HOMEMENUSTATUS,
	SCHOOLMENUSTATUS,
	SHOPMENUSTATUS,
	HARVESTMENUSTATUS,
	LOGMENUSTATUS,
	ACHIEVEMENTSMENUSTATUS,
	MENUSTATUSTOTAL
};

class MainScreen
{
public:
	MainScreen();
	~MainScreen();
	bool render(SDL_Renderer* m_gRender, 
		TextureManager* m_textureManager, 
		json p_settingJson, 
		json p_settingLanguage,
		bool& isStartScreen,
		bool& isMainScreen, 
		bool& isMainHtScreen,
		bool& isHtScreen);
	// isMainHtScreen是否渲染HT按钮
	void allButtoRender(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, bool isMainHtScreen);
	void initGameData(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingLanguage);
	void setHealthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage);
	void setGoldAndTimeToolTip(json p_settingJson, json p_settingLanguage);
	void returnMainHtScreen(bool& isMainScreen, bool& isMainHtScreen);
	void homeButtonUpdate();
	void schoolButtonUpdate(json p_settingJson);
	void shopButtonUpdate(json p_settingJson);

	// 主场景事件
	void mainScreenEvent(int mouseX, int mouseY, SDL_Event event, json p_settingJson);

	// 初始化steam接口
	bool initSteamApi();
	void setSteamAchievements();

	int p_imgCloudX = 0;
	int p_imgCloudY = 0;

private:
	std::array<bool, MENUSTATUSTOTAL> mainMenuStatusArray;  // 记录各个按钮的窗口状态
	json p_save01Data;  // 在initGameData中初始化
	PersonButton* m_personButton;
	HomeButton* m_homeButton;
	SchoolButton* m_schoolButton;
	ShopButton* m_shopButton;
	HarvestButton* m_harvestButton;
	LogButton* m_logButton;
	AchievementsButton* m_achievementsButton;
	
	WriteData* m_writeData;


	SDL_Rect m_personCurrentHealthRect = { 0, 0, 0, 0 };
	SDL_Rect m_personAllHealthRect = { 0, 0, 0, 0 };
	SDL_Rect m_personCurrentStatusRect = { 0, 0, 0, 0 };
	SDL_Rect m_personAllStatusRect = { 0, 0, 0, 0 };


	// 人物属性
	int m_personCurrentHealth = 0;
	int m_personAllHealth = 0;
	int m_personCurrentStatus = 0;
	int m_personAllStatus = 0;

	SDL_Texture* m_gameTimeTexture = nullptr;  // 游戏时间天数
	SDL_Texture* m_gamePersonGoldTexture = nullptr;  // 金币数
	SDL_Texture* m_gameHtFloorTexture = nullptr;  // 层数

	// 三层云朵变大动画
	int m_imgCloudWidth = PNG_USUAL_WIDTH32;
	int m_imgCloudHeight = PNG_USUAL_HEIGHT32;
	//int m_imgCloudX = 632;
	//int m_imgCloudY = 72;
	//int m_imgCloudX = 0;
	//int m_imgCloudY = 0;
	bool m_isCloudClicked = false;

	// steam成就对象和成就数组
	StreamAPIAchievements* m_streamAPIAchievements = nullptr;
	Achievement_t g_Achievements[];
};


