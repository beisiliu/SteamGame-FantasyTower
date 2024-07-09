//��ʼ����
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
	void initLanguage(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json &p_settingJson, json &p_settingLanguage); // ��ʼ��language��
	void update(json p_settingJson); // pngBackgroundHT_x �� pngBackgroundHT_y�� Cloud����
	void updateEvent_big(json p_settingJson);  // ȫ����ť���� - ���
	void updateEvent_small(json p_settingJson);  // ȫ����ť���� - ��С

	json p_save01Data;  // �ڼ�����ť �� ��ʼ��Ϸ��ȷ����ť�г�ʼ���������ڿ�ʼ����ʹ��

	int pngCloud01_x = 0;
	int pngCloud02_x = 0;
	int pngCloud03_x = 0;

private:
	bool m_isEnterUserName = false;
	char buf[64] = u8"";  // playerName buffer is 64

	// �ڹ��캯����ʼ��
	WriteData* m_writeData;

	// HT���ֶ���
	int pngBackgroundHT_x;
	int pngBackgroundHT_y;
	int pngBackgroundHT_speed;

	// �ƶ䶯��
	//int pngCloud01_x;
	bool isPngCloud01 = true;
	//int pngCloud02_x;
	bool isPngCloud02 = true;
	//int pngCloud03_x;
	bool isPngCloud03 = true;
	int pngCloud_speed;

	// ȫ����ť����
	int pngFullWindow_x;
	int pngFullWindow_y;
	int pngFullWindow_width;
	int pngFullWindow_height;
	

};