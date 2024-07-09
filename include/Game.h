//��Ϸ���ļ�
#pragma once

#include<array>
#include<vector>
#include<fstream>
#include<SDL.h>
#include<SDL_mixer.h>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"TextureManager.h"
#include"WriteData.h"

//�����ļ�
#include"setting/Config.h"
#include<iostream>

// ����ͷ�ļ�
#include"StartScreen.h"
#include"MainScreen.h"
#include"HtScreen.h"

using json = nlohmann::json;


// ��ų�����������
enum ScreenNum
{
    STARTRENDER,
    MAINRENDERHT, // ��HT��ť��������
    MAINRENDER, // ������
    HTRENDER,  // ս������
    TOTALRENDER
};

class Game
{
public:
    Game();
    ~Game();
    bool initWindow(const char* title, int xPost, int yPost, int width, int height, bool isFullWindow);
    bool init();
    bool loadGame();
    void handleEvents();
    void update();   // variable update
    void render();
    void clean();   // release memory

    bool p_gameIsRunning;
    json p_settingJson; //��ȡsetting.bin�Ķ����ڹ��캯����initLanguage��ʼ��
    json p_settingLanguage; //��ȡstartScreenLanguage_XX.bin�Ķ����ڹ��캯����initLanguage��ʼ��
    Mix_Music* p_gMusic;
private:
    //�ڹ��캯���н��г�ʼ��, �����������ͷ��ڴ�    
    SDL_Window* m_gWindow;
    SDL_Renderer* m_gRender;

    //��������
    StartScreen* m_startScreen;
    MainScreen* m_mainScreen;
    HtScreen* m_htScreen;

    //��ų����л�boolֵ��������ʹ��ö�٣�ö������ScreenNum����ͬһ�ļ��У��ڹ��캯���г�ʼ��
    std::array<bool, TOTALRENDER> screenRenderArray;
    //��ż��������Ƿ�ɹ���boolֵ
    std::vector<bool> m_isLoadSuccess;
    //��ȡ���ڴ�С
    int m_width = 0;
    int m_height = 0;

    //��ʼ�����˵�λ��, ��ʼ���ɹ���ֵΪfalse��ֻ��ʼ��һ��
    bool m_isEnemyMoveX = true;
    //��ʼ���������ؽ����ƶ��X,Y������
    bool m_isCloudInitXY = true;
    //��ʼ�����ƶ�λ�ó�ʼ��
    bool m_isStartScreenCloudePostion = true;


    TextureManager* m_textureManager;
    WriteData* m_writeData;

};