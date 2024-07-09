//游戏主文件
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

//常量文件
#include"setting/Config.h"
#include<iostream>

// 场景头文件
#include"StartScreen.h"
#include"MainScreen.h"
#include"HtScreen.h"

using json = nlohmann::json;


// 存放常见的数组编号
enum ScreenNum
{
    STARTRENDER,
    MAINRENDERHT, // 带HT按钮的主场景
    MAINRENDER, // 主场景
    HTRENDER,  // 战斗场景
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
    json p_settingJson; //读取setting.bin的对象，在构造函数中initLanguage初始化
    json p_settingLanguage; //读取startScreenLanguage_XX.bin的对象，在构造函数中initLanguage初始化
    Mix_Music* p_gMusic;
private:
    //在构造函数中进行初始化, 在析构函数释放内存    
    SDL_Window* m_gWindow;
    SDL_Renderer* m_gRender;

    //场景对象
    StartScreen* m_startScreen;
    MainScreen* m_mainScreen;
    HtScreen* m_htScreen;

    //存放场景切换bool值，数组编号使用枚举，枚举名：ScreenNum，在同一文件中，在构造函数中初始化
    std::array<bool, TOTALRENDER> screenRenderArray;
    //存放加载数据是否成功的bool值
    std::vector<bool> m_isLoadSuccess;
    //获取窗口大小
    int m_width = 0;
    int m_height = 0;

    //初始化敌人的位置, 初始化成功后，值为false，只初始化一次
    bool m_isEnemyMoveX = true;
    //初始化三层隐藏奖励云朵的X,Y轴坐标
    bool m_isCloudInitXY = true;
    //开始场景云朵位置初始化
    bool m_isStartScreenCloudePostion = true;


    TextureManager* m_textureManager;
    WriteData* m_writeData;

};