#pragma once

#include<SDL.h>
#include<fstream>
#include<string>
#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"

#include"setting/config.h"
#include"TextureManager.h"
#include"UsualFunc.h"
#include"WriteData.h"
#include"PersonButton.h"
#include"EnemyAttribute.h"
#include"CalCombatKPI.h"

#include"nlohmann/json.hpp"

using json = nlohmann::json;

// 记录敌人数量
constexpr auto ENEMYCOUNT = 7;

class HtScreen 
{
public:
	HtScreen();
	~HtScreen();
	void render(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void allRender(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage);
	void initHtGameData(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage);
	void healthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage);
	void enemyHealthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage);
	void attackLogFrame(json p_settingJson, json p_settingLanguage);
	void personInformationFrame(json p_settingJson, json p_settingLanguage, TextureManager* m_textureManager);
	void enemyInformationFrame(json p_settingJson, json p_settingLanguage);
	void buttonFrame(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void personWinFrame(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void personLoseFrame(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void animationUpdate(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson);
	void turnBasedNum(json p_settingJson, json p_settingLanguage);
	void endFrame(json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void enemyAttack();
	void personAttack();
	void personSkillAttack(json p_settingLanguage);
	void gooutButton(json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen);
	void showEnemyName_healty_status_Floor001(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor002(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor003(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor004(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor005(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor006(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);
	void showEnemyName_healty_status_Floor007(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson);

	json p_save01Data;  // 在initHtGameData中初始化

	// 是否显示人物战斗动画 - 人物速度快
	bool p_isCombatAnimationPerson;
	// 是否显示敌人战斗动画 - 敌人速度快
	bool p_isCombatAnimationEnemy;

	EnemyAttribute* p_enemyAttribute;

	// 敌人移动
	int p_enemyMoveX = 0;

private:
	// 构造函数中初始化
	PersonButton* m_personButton;
	WriteData* m_writeData;
	CalCombatKPI* m_calCombatKPI;

	//是否已经读取玩怪物名称
	bool m_isInfo;
	//判断防御技能按钮是否被按下
	bool m_isDefenseButton = false;
	bool m_isHeavyAttackButton = false;

	// 人物血条和状态条的Rect，initHtGameData 中初始化
	SDL_Rect m_personCurrentHealthRect = { 0,0,0,0 };
	SDL_Rect m_personAllHealthRect = { 0,0,0,0 };
	SDL_Rect m_personCurrentStatusRect = { 0,0,0,0 };
	SDL_Rect m_personAllStatusRect = { 0,0,0,0 };

	// 敌人血条的Rect对象
	SDL_Rect m_enemyCurrentHealthRect = { 0,0,0,0 };
	SDL_Rect m_enemyAllHealthRect = { 0,0,0,0 };
	// 敌人蓝条的Rect对象
	SDL_Rect m_enemyCurrentStatusRect = { 0,0,0,0 };
	SDL_Rect m_enemyAllStatusRect = { 0,0,0,0 };

	// 人物属性
	int m_personCurrentHealth = 0;
	int m_personAllHealth = 0;
	int m_personCurrentStatus = 0;
	int m_personAllStatus = 0;
	int m_personCurrentAttack = 0;
	int m_personCurrentDefense = 0;
	int m_personCurrentSpeed = 0;

	// 存放敌人名称
	std::array<std::string, ENEMYCOUNT> m_enemyNameArray;

	// 游戏天数
	int m_gameTime = 0;
	// HT层数
	int m_htFloor = 0;
	// 回合数
	int m_htRoundNum;
	// 是否显示buttonFrame
	bool m_isButtonFrame;
	// 7floor 2button
	//bool m_isOpenChests_7floor = false;
	//bool m_isGoAhead_7floor = false;

	// 循环计数的，通过循环数的奇偶判断m_personAnimationX的值，也可用于其他值的判断
	int m_AnimationCount = 0;
	// 人物移动
	int m_personMoveX;
	// 人物动画图片位置
	int m_personAnimationX;
	// 敌人动画效果, initHtGameData的m_isLoadEnemyInfo中初始化
	int m_enemyAnimationY = 0;
	int m_enemyAnimationHeight = 0;
	int m_enemyAnimationPicPos = 0;


	// 移动速度 - 人物和敌人公用
	int m_moveSpeed = 20;
	// 战斗动画 - 人物和敌人公用
	int m_attackNum;
	int animationNums;

	// 为了找到已经装备的技能
	json m_skillJson;
};
