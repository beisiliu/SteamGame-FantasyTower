//开始场景
#pragma once

#include<SDL.h>
#include<iostream>
#include<filesystem>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"TextureManager.h"
#include"HtScreen.h"
#include"WriteData.h"
#include"UsualFunc.h"

#include"setting/config.h"

using json = nlohmann::json;

class StartScreen
{
public:
	StartScreen();
	~StartScreen();
	void render(SDL_Renderer* m_gRender, 
		TextureManager* m_textureManager, 
		json p_settingJson, 
		json p_settingLanguage, 
		int& radioButtonChose, 
		bool& p_gameIsRunning,
		bool& isStartScreen,
		bool& isMainScreen,
		bool& isMainHtScreen,
		HtScreen* m_htScreen);
	void initLanguage(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json &p_settingJson, json &p_settingLanguage); // 初始化language。
	void update(json p_settingJson); // pngBackgroundHT_x ， pngBackgroundHT_y， Cloud动画
	void updateEvent_big(json p_settingJson);  // 全屏按钮动画 - 变大
	void updateEvent_small(json p_settingJson);  // 全屏按钮动画 - 变小

	json p_save01Data;  // 在继续按钮 或 开始游戏的确定按钮中初始化，仅仅在开始场景使用

	int pngCloud01_x = 0;
	int pngCloud02_x = 0;
	int pngCloud03_x = 0;

private:
	bool m_isEnterUserName = false;
	char buf[64] = u8"";  // playerName buffer is 64

	// 在构造函数初始化
	WriteData* m_writeData;

	// HT文字动画
	int pngBackgroundHT_x;
	int pngBackgroundHT_y;
	int pngBackgroundHT_speed;

	// 云朵动画
	//int pngCloud01_x;
	bool isPngCloud01 = true;
	//int pngCloud02_x;
	bool isPngCloud02 = true;
	//int pngCloud03_x;
	bool isPngCloud03 = true;
	int pngCloud_speed;

	// 全屏按钮动画
	int pngFullWindow_x;
	int pngFullWindow_y;
	int pngFullWindow_width;
	int pngFullWindow_height;
	

};