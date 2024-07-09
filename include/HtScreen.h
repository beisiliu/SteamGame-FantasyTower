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

// ��¼��������
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

	json p_save01Data;  // ��initHtGameData�г�ʼ��

	// �Ƿ���ʾ����ս������ - �����ٶȿ�
	bool p_isCombatAnimationPerson;
	// �Ƿ���ʾ����ս������ - �����ٶȿ�
	bool p_isCombatAnimationEnemy;

	EnemyAttribute* p_enemyAttribute;

	// �����ƶ�
	int p_enemyMoveX = 0;

private:
	// ���캯���г�ʼ��
	PersonButton* m_personButton;
	WriteData* m_writeData;
	CalCombatKPI* m_calCombatKPI;

	//�Ƿ��Ѿ���ȡ���������
	bool m_isInfo;
	//�жϷ������ܰ�ť�Ƿ񱻰���
	bool m_isDefenseButton = false;
	bool m_isHeavyAttackButton = false;

	// ����Ѫ����״̬����Rect��initHtGameData �г�ʼ��
	SDL_Rect m_personCurrentHealthRect = { 0,0,0,0 };
	SDL_Rect m_personAllHealthRect = { 0,0,0,0 };
	SDL_Rect m_personCurrentStatusRect = { 0,0,0,0 };
	SDL_Rect m_personAllStatusRect = { 0,0,0,0 };

	// ����Ѫ����Rect����
	SDL_Rect m_enemyCurrentHealthRect = { 0,0,0,0 };
	SDL_Rect m_enemyAllHealthRect = { 0,0,0,0 };
	// ����������Rect����
	SDL_Rect m_enemyCurrentStatusRect = { 0,0,0,0 };
	SDL_Rect m_enemyAllStatusRect = { 0,0,0,0 };

	// ��������
	int m_personCurrentHealth = 0;
	int m_personAllHealth = 0;
	int m_personCurrentStatus = 0;
	int m_personAllStatus = 0;
	int m_personCurrentAttack = 0;
	int m_personCurrentDefense = 0;
	int m_personCurrentSpeed = 0;

	// ��ŵ�������
	std::array<std::string, ENEMYCOUNT> m_enemyNameArray;

	// ��Ϸ����
	int m_gameTime = 0;
	// HT����
	int m_htFloor = 0;
	// �غ���
	int m_htRoundNum;
	// �Ƿ���ʾbuttonFrame
	bool m_isButtonFrame;
	// 7floor 2button
	//bool m_isOpenChests_7floor = false;
	//bool m_isGoAhead_7floor = false;

	// ѭ�������ģ�ͨ��ѭ��������ż�ж�m_personAnimationX��ֵ��Ҳ����������ֵ���ж�
	int m_AnimationCount = 0;
	// �����ƶ�
	int m_personMoveX;
	// ���ﶯ��ͼƬλ��
	int m_personAnimationX;
	// ���˶���Ч��, initHtGameData��m_isLoadEnemyInfo�г�ʼ��
	int m_enemyAnimationY = 0;
	int m_enemyAnimationHeight = 0;
	int m_enemyAnimationPicPos = 0;


	// �ƶ��ٶ� - ����͵��˹���
	int m_moveSpeed = 20;
	// ս������ - ����͵��˹���
	int m_attackNum;
	int animationNums;

	// Ϊ���ҵ��Ѿ�װ���ļ���
	json m_skillJson;
};
