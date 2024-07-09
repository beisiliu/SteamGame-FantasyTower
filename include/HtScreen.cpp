#include"HtScreen.h"

HtScreen::HtScreen()
{
	p_isCombatAnimationPerson = false;
	p_isCombatAnimationEnemy = false;

	m_writeData = new WriteData();
	m_personButton = new PersonButton();

	p_enemyAttribute = nullptr;  // 在render的时候进行初始化
	m_calCombatKPI = new CalCombatKPI();

	m_htRoundNum = 0;
	m_isButtonFrame = true;


	m_personMoveX = 0;
	m_personAnimationX = 1;

	m_attackNum = 1;
	animationNums = 0;

	m_isInfo = false;
}

HtScreen::~HtScreen()
{
	delete m_writeData;
	delete m_personButton;
	delete p_enemyAttribute;
	delete m_calCombatKPI;

}

void HtScreen::render(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{
	initHtGameData(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);
	if (p_enemyAttribute == nullptr) { p_enemyAttribute = new EnemyAttribute(m_htFloor); }

	//ImGui每次循环创建一个新的frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	healthAndStatusToolTip(m_textureManager, p_settingJson, p_settingLanguage);  // 人物血条和状态条的tooltip
	enemyHealthAndStatusToolTip(m_textureManager, p_settingJson, p_settingLanguage); // 敌人血条和状态条的tooltip
	turnBasedNum(p_settingJson, p_settingLanguage);  // 回合数
	if (p_save01Data["HtFloor"] <= 7)
	{
		attackLogFrame(p_settingJson, p_settingLanguage); // 战斗日志框架
		personInformationFrame(p_settingJson, p_settingLanguage, m_textureManager); // 人物信息框架
		enemyInformationFrame(p_settingJson, p_settingLanguage); // 敌人信息框架

		// 战斗按钮框架
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			buttonFrame(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
		// 战斗胜利框架
		if (p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			personWinFrame(m_textureManager, m_gRender, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
		// 战斗失败框架
		if (!p_save01Data["isPersonWin"] and p_save01Data["isPersonLose"])
		{
			personLoseFrame(m_textureManager, m_gRender, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
	}
	else
	{
		// 游戏通关的框架
		endFrame(p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
	}


	if (isHtScreen)  // 如果isHtScreen不为true，就不渲染，通过else中的代码结束掉， 解决三层按钮点击逃跑的bug
	{
		allRender(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);
	}
	else
	{
		ImGui::Render();
	}

}

void HtScreen::allRender(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// 主背景
	ImGui::Render();
	SDL_SetRenderDrawColor(m_gRender, 28, 28, 28, 255);  // 设置背景色 - 黑色
	SDL_RenderClear(m_gRender);

	// 战斗场景的Render
	// 战斗场景的背景墙
	m_textureManager->render(PNG_BACKGROUND_HT_ATTACK_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"] / 4 + 64);
	m_textureManager->render(PNG_BACKGROUND_HT_ID, m_gRender, 0, p_settingJson["SetWindowHeight"] / 12 * 4, p_settingJson["SetWindowWidth"], int(p_settingJson["SetWindowHeight"] / 1.5));

	// 人物名称渲染
	m_textureManager->render(TTF_PLAYER_NAME_ID, m_gRender, int(p_settingJson["SetWindowWidth"] *0.015), 0, m_textureManager->p_playerNameWidth, TTF_PLAYER_NAME_HEIGHT);

	// 人物血条和状态条渲染
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); // 设置黑色上限框
	SDL_RenderDrawRect(m_gRender, &m_personAllHealthRect);  // 血条上限框 暂未控制，暂定500像素
	SDL_RenderDrawRect(m_gRender, &m_personAllStatusRect);  // 状态上限框 暂未控制，暂定500像素

	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_personCurrentHealthRect);

	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //设置状态条蓝色
	SDL_RenderFillRect(m_gRender, &m_personCurrentStatusRect);

	// 战斗场景的Render
	// 渲染胜利图标和敌人倒地
	if (p_save01Data["isPersonWin"])
	{
		m_textureManager->render(PNG_GAMEWIND_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2, p_settingJson["SetWindowHeight"] / 12, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
		if (m_htFloor == 2)
		{
			// 敌人倒地渲染， 有压缩
			m_textureManager->dynamicRender(ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 + 32 - PNG_USUAL_WIDTH64, 64, 128, 1, 1, -90.0);
			// 渲染血条
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 3)
		{
			// 敌人倒地渲染， 有压缩
			m_textureManager->scaleRender(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// m_textureManager->render(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 渲染血条
			showEnemyName_healty_status_Floor002(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 4)
		{
			// 敌人倒地渲染， 有压缩
			m_textureManager->scaleRender(ENEMY_FLOOR003_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// 渲染血条
			showEnemyName_healty_status_Floor003(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 5)
		{
			// 敌人倒地渲染， 有压缩
			m_textureManager->scaleRender(ENEMY_FLOOR004_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// 渲染血条
			showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 6)
		{
			// 敌人倒地渲染， 有压缩
			m_textureManager->scaleRender(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// 渲染血条
			showEnemyName_healty_status_Floor005(m_textureManager, m_gRender, p_settingJson);
		}
	}
	// 渲染失败图标
	if (p_save01Data["isPersonLose"])
	{
		m_textureManager->render(PNG_GAMELOSE_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2, p_settingJson["SetWindowHeight"] / 12, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
		// 人物倒地渲染，有压缩
		m_textureManager->dynamicRender(PNG_ANIMATION_PERSON_ID, m_gRender, 32, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_HEIGHT64 + 32, 64, 128, 1, 1, 90.0);

		if (m_htFloor == 1)
		{
			// 输了后，渲染敌人
			m_textureManager->dynamicRender(ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, 128, 128, 1, 1);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 2)
		{
			// 输了后，渲染敌人
			m_textureManager->render(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor002(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 3)
		{
			// 输了后，渲染敌人
			m_textureManager->render(ENEMY_FLOOR003_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor003(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 4)
		{
			// 输了后，渲染敌人
			m_textureManager->render(ENEMY_FLOOR004_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 5)
		{
			// 输了后，渲染敌人
			m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor005(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 6)
		{
			// 输了后，渲染敌人
			// m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor006(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 7)
		{
			// 输了后，渲染敌人
			// m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// 输了后，渲染敌人的血条
			showEnemyName_healty_status_Floor007(m_textureManager, m_gRender, p_settingJson);
		}
	}
	else  // 只要没有输的情况下，做人物移动渲染
	{
		// 人物渲染，m_personMoveX 控制人物移动，
		// 当人物移动到敌人位置时，人物的判断，人物消失
		
		if (m_personMoveX <  p_settingJson["SetWindowWidth"] - 164 and p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
		{
			m_textureManager->dynamicRender(PNG_ANIMATION_PERSON_ID, m_gRender, m_personMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_HEIGHT64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_personAnimationX, 1, 2);
		}
	}

	// Render 1floor enemy
	if (m_htFloor == 1)
	{
		// 一层敌人的名称和血条
		// 只能在这边渲染，因为clear和present之间
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
			// 当人物移动到敌人位置时，敌人的判断，让敌人消失
			if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
			{
				m_textureManager->dynamicRender(ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, 128, 128, 1, 1);
			}
			else
			{
				m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
			}
		}		
	}
	// Render 2floor enemy
	if (m_htFloor == 2)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor002(m_textureManager, m_gRender, p_settingJson);
			if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
			{
				 m_textureManager->scaleRender(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
			}
			else
			{
				m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
			}	
		}
	}
	// Render 3floor enemy
	if (m_htFloor == 3)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor003(m_textureManager, m_gRender, p_settingJson);
			// 这边 要分情况处理了，以下是玩家速度快的渲染情况
			// if (m_personButton->calPersonCurrentSpeed() >= p_enemyAttribute->p_enemySpeed
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
			{
				if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR003_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}

			}
			else  // 以下是敌人速度快的情况
			{

				if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR003_ID, m_gRender, p_enemyMoveX, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
		}
	}
	// Render 4floor enemy
	if (m_htFloor == 4)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
			// 这边 要分情况处理了，以下是玩家速度快的渲染情况
			// if (m_personButton->calPersonCurrentSpeed() >= p_enemyAttribute->p_enemySpeed)
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
			{
				if (!p_save01Data["isPersonWin"] and p_save01Data["isPersonLose"]) showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
				if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR004_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}

			}
			else  // 以下是敌人速度快的情况
			{

				if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR004_ID, m_gRender, p_enemyMoveX, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
		}
	}
	// Render 5floor enemy
	if (m_htFloor == 5)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor005(m_textureManager, m_gRender, p_settingJson);
			// 这边 要分情况处理了，以下是玩家速度快的渲染情况
			// if (m_personButton->calPersonCurrentSpeed() >= p_enemyAttribute->p_enemySpeed)
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
			{
				if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}

			}
			else  // 以下是敌人速度快的情况
			{

				if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
				{
					m_textureManager->scaleRender(ENEMY_FLOOR005_ID, m_gRender, p_enemyMoveX, m_enemyAnimationY, PNG_USUAL_WIDTH64, m_enemyAnimationHeight, 1, 1);
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
		}
	}
	// Render 6floor enemy
	if (m_htFloor == 6)
	{

		// 这种情况下，会播放敌人和人物的战斗动画
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor006(m_textureManager, m_gRender, p_settingJson);
			// 这边 要分情况处理了，以下是玩家速度快的渲染情况，保证防御按钮下后不执行
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed and !m_isDefenseButton)
			{
				if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
				{
					// 敌人idle的状态
					m_textureManager->dynamicRender(ENEMY_FLOOR006_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
					// 需补充人物跑动的动画，然后出发else

				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
			else // 以下是敌人速度快的情况
			{
				if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
				{
					// idle的情况
					if (!p_isCombatAnimationPerson and !p_isCombatAnimationEnemy)
					{
						m_textureManager->dynamicRender(ENEMY_FLOOR006_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
					}
					// 跑动的情况
					if (!p_isCombatAnimationPerson and p_isCombatAnimationEnemy)
					{
						m_textureManager->dynamicRender(ENEMY_FLOOR006_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
					}
				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
		}
	}
	// Render 7floor enemy
	if (m_htFloor == 7)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{	
			// display chests
			if (!p_save01Data["isEnemyInformaion"] and !p_save01Data["isOpenChests_7floor"]) // not open chests
			{
				m_textureManager->dynamicRender(ANIMATION_CHESTS_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 1);
			}
			if (!p_save01Data["isEnemyInformaion"] and p_save01Data["isOpenChests_7floor"]) // opened chests
			{
				m_textureManager->dynamicRender(ANIMATION_CHESTS_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, 4, 1, 1);
			}
			if (p_save01Data["isEnemyInformaion"]) // go ahead
			{
				showEnemyName_healty_status_Floor007(m_textureManager, m_gRender, p_settingJson);
				// 这边 要分情况处理了，以下是玩家速度快的渲染情况，保证防御按钮下后不执行
				if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed and !m_isDefenseButton)
				{
					if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
					{
						// 敌人idle的状态
						m_textureManager->dynamicRender(ENEMY_FLOOR007_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
						// 需补充人物跑动的动画，然后出发else

					}
					else
					{
						m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
					}
				}
				else // 以下是敌人速度快的情况
				{
					if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
					{
						// idle的情况
						if (!p_isCombatAnimationPerson and !p_isCombatAnimationEnemy)
						{
							m_textureManager->dynamicRender(ENEMY_FLOOR007_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
						}
						// 跑动的情况
						if (!p_isCombatAnimationPerson and p_isCombatAnimationEnemy)
						{
							m_textureManager->dynamicRender(ENEMY_FLOOR007_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
						}
					}
					else
					{
						m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_enemyMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
					}
				}

			}

		}

	}
	
	// ImGui渲染
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_gRender);
}

void HtScreen::initHtGameData(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// 每次render的时候，都要读去保存的数据文件，以做到实时的数据更新，形成局部变量
	std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
	p_save01Data = json::from_bson(gameSaving);
	gameSaving.close();

	// 初始化人物数据
	m_personButton->initPersonData(p_save01Data);

	//初始化血量，状态等相关KPI
	m_personCurrentHealth = p_save01Data["personCurrentHealth"];
	m_personAllHealth = p_save01Data["personAllHealth"];
	m_personCurrentStatus = p_save01Data["personCurrentStatus"];
	m_personAllStatus = p_save01Data["personAllStatus"];

	//如果状态值为负数，要值为0的
	if (m_personCurrentStatus < 0) m_personCurrentStatus = 0;

	m_personAllHealthRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10 - 1, 5 - 1, m_personAllHealth + 2, 25 + 2 };
	m_personCurrentHealthRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10, 5, m_personCurrentHealth, 25 };
	m_personAllStatusRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10 - 1, 38 - 1, m_personAllStatus + 2, 25 + 2 };
	m_personCurrentStatusRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10, 38, m_personCurrentStatus, 25 };

	// 获取游戏天数
	m_gameTime = p_save01Data["gameTime"];
	// 获取HT层数
	m_htFloor = p_save01Data["HtFloor"];

	// 初始化HT场景内容，在点击逃跑和离开按钮后，
	if (!m_isInfo)
	{
		// 人物属性初始化
		m_personCurrentAttack = m_personButton->calPersonCurrentAccack(); // 人物攻击
		m_personCurrentDefense = m_personButton->calPersonCurrentDefense(); // 人物防御
		m_personCurrentSpeed = m_personButton->calPersonCurrentSpeed(); // 人物速度

		// 获取人物技能的json
		m_skillJson = p_save01Data["personSkill"];

		// 在数组中初始化Enemy名称
		m_enemyNameArray = {
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor01Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor02Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor03Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor04Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor05Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor06Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor07Name"].dump()),
		};
		
		// 加载敌人名称到texture
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR001NAME_ID, m_enemyNameArray[0].c_str(), 1, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR002NAME_ID, m_enemyNameArray[1].c_str(), 2, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR003NAME_ID, m_enemyNameArray[2].c_str(), 3, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR004NAME_ID, m_enemyNameArray[3].c_str(), 4, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR005NAME_ID, m_enemyNameArray[4].c_str(), 5, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR006NAME_ID, m_enemyNameArray[5].c_str(), 6, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR007NAME_ID, m_enemyNameArray[6].c_str(), 7, m_gRender, { 0, 255, 255 });

		// 避免重复加载
		m_isInfo = true;
	}

	// 要重新计算属性*0.8
	if ((static_cast<float>(m_personCurrentStatus) / m_personAllStatus) * 100 < 50)
	{
		m_personCurrentAttack = m_personButton->calPersonCurrentAccack(); // 人物攻击
		m_personCurrentDefense = m_personButton->calPersonCurrentDefense(); // 人物防御
		m_personCurrentSpeed = m_personButton->calPersonCurrentSpeed(); // 人物速度
	}

}

void HtScreen::healthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// 制作鼠标移动到血条和状态上显示数值 - toolTip
	ImGui::SetNextWindowSize(ImVec2(512, PNG_USUAL_HEIGHT64 + 48));
	//ImGui::SetNextWindowPos(ImVec2(float(TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10), 0.0f));
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"]* 0.015 + m_textureManager->p_playerNameWidth), 0.0f));
	ImGui::Begin("healthAndStatusTooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	// 基于最大的personAllHealth或最大的personAllStatus来显示invisibleButton的宽度
	if (p_save01Data["personAllHealth"] >= p_save01Data["personAllStatus"])
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(float(p_save01Data["personAllHealth"]), float(p_settingJson["SetWindowHeight"] * 0.08)));
	}
	else
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(float(p_save01Data["personAllStatus"]), float(p_settingJson["SetWindowHeight"] * 0.08)));
	}
	// 当鼠标移动到invisibleButton的时候，显示tooltip
	// 局部变量
	if (ImGui::IsItemHovered())
	{
		// 血条的toolTip
		std::string healthToolTip_name = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
		std::string healthToolTip_current = std::to_string(m_personCurrentHealth);
		std::string healthToolTip_all = std::to_string(m_personAllHealth);
		std::string healthToolTip = healthToolTip_name + ":" + healthToolTip_current + "/" + healthToolTip_all;

		// 状态条的toopTip
		std::string statusToolTip_name = p_settingLanguage["MainMeum"]["Tooltip"]["Status"].dump();
		std::string statusToolTip_current = std::to_string(m_personCurrentStatus);
		std::string statusToolTip_all = std::to_string(m_personAllStatus);
		std::string statusToolTip = statusToolTip_name + ":" + statusToolTip_current + "/" + statusToolTip_all;

		if (ImGui::BeginTooltip())
		{
			ImGui::Text(healthToolTip.c_str());
			ImGui::Text(statusToolTip.c_str());
			ImGui::EndTooltip();
		}
	}
	ImGui::End();
}

void HtScreen::enemyHealthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// 在战斗场景的toolTip
	if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
	{
		if (m_htFloor == 1) 
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor001Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			// ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->m_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1), 4.0f));
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f ));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 2)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor002Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor002Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 3)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor003Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor003Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 4)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2),float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor004Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor004Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 5)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor005Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor005Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			// 血条的字符串拼接
			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 6)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(50 + 6)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor006Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 血条的字符串拼接
			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			// 蓝条的字符串拼接
			std::string status = p_settingLanguage["MainMeum"]["Tooltip"]["Status"].dump();
			std::string enemyCurrentStatus = std::to_string(p_enemyAttribute->p_enemyStatus);
			std::string enemyAllStatus = std::to_string(p_enemyAttribute->p_enemeStatusLimit);
			std::string enemyStatusStr = status + ":" + enemyCurrentStatus + "/" + enemyAllStatus;


			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor006Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 40.0f));

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::Text(enemyStatusStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 7)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(50 + 6)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor007Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 血条的字符串拼接
			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			// 蓝条的字符串拼接
			std::string status = p_settingLanguage["MainMeum"]["Tooltip"]["Status"].dump();
			std::string enemyCurrentStatus = std::to_string(p_enemyAttribute->p_enemyStatus);
			std::string enemyAllStatus = std::to_string(p_enemyAttribute->p_enemeStatusLimit);
			std::string enemyStatusStr = status + ":" + enemyCurrentStatus + "/" + enemyAllStatus;


			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor007Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 40.0f));

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::Text(enemyStatusStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}

	}
	// 在战斗失败场景
	if (p_save01Data["isPersonLose"])
	{
		if (m_htFloor == 1)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor001Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			// ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->m_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1), 4.0f));
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 2)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor002Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 3)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor003Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 4)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor004Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 5)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor005Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor001Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 6)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor006Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor006Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
		if (m_htFloor == 7)
		{
			// 制作tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor007Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// 隐藏按钮，让tooltip挂靠
			ImGui::InvisibleButton("showEnemyHealthFloor007Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			if (ImGui::IsItemHovered())
			{
				if (ImGui::BeginTooltip())
				{
					ImGui::Text(enemyHealthStr.c_str());
					ImGui::EndTooltip();
				}
			}
			ImGui::End();
		}
	}
}

void HtScreen::attackLogFrame(json p_settingJson, json p_settingLanguage)  // 计划做些游戏提示，攻击日志，后续在考虑
{
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"]), ATTACK_FRAME_HEIGHT));
	ImGui::Begin("attackLogFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (m_htFloor == 1)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Message"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint2"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint3"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor001Hint4"].dump()).c_str());
		ImGui::Text("----------------------");
	}

	if (m_htFloor == 2)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor002"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor002Hint1"].dump()).c_str());
	}

	if (m_htFloor == 3)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor003"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor003Hint1"].dump()).c_str());
	}

	if (m_htFloor == 4)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor004"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor004Hint1"].dump()).c_str());
	}

	if (m_htFloor == 5)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005Hint1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor005Hint2"].dump()).c_str());
	}

	if (m_htFloor == 6)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint2"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor006Hint3"].dump()).c_str());
	}

	if (m_htFloor == 7)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007Hint1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["AttackLogFrame"]["Floor007Hint2"].dump()).c_str());
	}
	ImGui::End();
}

void HtScreen::personInformationFrame(json p_settingJson, json p_settingLanguage, TextureManager* m_textureManager)
{
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3 + ATTACK_FRAME_HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 4), float(p_settingJson["SetWindowHeight"] / 3 * 2 - ATTACK_FRAME_HEIGHT)));
	ImGui::Begin("personInformationFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	// current accack
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentAttack"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_personCurrentAttack).c_str());
	// current defense
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentDefense"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_personCurrentDefense).c_str());
	// current speed
	ImGui::Text(removeQuotes(p_settingLanguage["PersonButton"]["CombatCapability"]["CurrentSpeed"].dump()).c_str());
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_personCurrentSpeed).c_str());
	

	// >=7floor,display potions
	if (m_htFloor >= 7)
	{
		ImGui::Text("----------------------------------");
		// Ensure smallHealthPotionCount > 0
		if (p_save01Data["smallHealthPotionCount"] > 0)
		{
			// display icon,name,counts and smallbutton
			// icon
			m_textureManager->imguiIMG(PNG_POTION_SMALLHEALTHPOTION_ID, 16, 16);
			ImGui::SameLine();

			// name
			ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallHealthPotion"].dump()).c_str());
			ImGui::SameLine();
			
			// count
			ImGui::Text("*"); ImGui::SameLine();
			ImGui::Text(p_save01Data["smallHealthPotionCount"].dump().c_str());
			ImGui::SameLine();

			// button
			ImGui::PushID(1);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["Potions"]["DrinkButton"].dump()).c_str()))
			{
				// Think about : health +10, health limit, count -1
				// deal with health +10 and health limit
				if (( int(p_save01Data["personAllHealth"]) - int(p_save01Data["personCurrentHealth"]) ) >= 10)
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", p_save01Data["personCurrentHealth"] + 10);
				}
				else
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", int(p_save01Data["personAllHealth"]));
				}

				// deal with count -1
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallHealthPotionCount", p_save01Data["smallHealthPotionCount"] - 1);
			}
			ImGui::PopID();
		}

		if (p_save01Data["smallStatusPotionCount"] > 0)
		{
			// display icon,name,counts and smallbutton
			// icon
			m_textureManager->imguiIMG(PNG_POTION_SMALLSTATUSPOTION_ID, 16, 16);
			ImGui::SameLine();

			// name
			ImGui::Text(removeQuotes(p_settingLanguage["Potions"]["SmallStatusPotion"].dump()).c_str());
			ImGui::SameLine();

			// count
			ImGui::Text("*"); ImGui::SameLine();
			ImGui::Text(p_save01Data["smallStatusPotionCount"].dump().c_str());
			ImGui::SameLine();

			// button
			ImGui::PushID(2);
			if (ImGui::SmallButton(removeQuotes(p_settingLanguage["Potions"]["DrinkButton"].dump()).c_str()))
			{
				// Think about : status +10 status limit, count -1
				// deal with status +10 and status limit
				if ((int(p_save01Data["personAllStatus"]) - int(p_save01Data["personCurrentStatus"])) >= 10)
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", p_save01Data["personCurrentStatus"] + 10);
				}
				else
				{
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", int(p_save01Data["personAllStatus"]));
				}

				// deal with count -1
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallStatusPotionCount", p_save01Data["smallStatusPotionCount"] - 1);
			}
			ImGui::PopID();
		}
	}
	
	ImGui::End();

	// defense button - defense restore, !p_isCombatAnimationEnemy，when playing animation，keep defense +2
	if (m_isDefenseButton and !p_isCombatAnimationEnemy)
	{
		m_personCurrentDefense -= 2;
		m_personCurrentSpeed += 1;
		m_isDefenseButton = false;
	}

	// HeavyAttack Button
	if (m_isHeavyAttackButton and !p_isCombatAnimationEnemy and !p_isCombatAnimationPerson)
	{
		m_personCurrentAttack -= 2;
		m_isHeavyAttackButton = false;
	}
}

void HtScreen::buttonFrame(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 4), float(p_settingJson["SetWindowHeight"] / 3 + ATTACK_FRAME_HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 4 * 2), float(p_settingJson["SetWindowHeight"] / 3 * 2 - ATTACK_FRAME_HEIGHT)));
	ImGui::Begin("buttonFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	// attack button
	if (!p_isCombatAnimationPerson and p_save01Data["isEnemyInformaion"])
	{
		// 普通攻击按钮
		if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["Attack"].dump()).c_str()))
		{
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
			{
				// 玩家出手后，渲染玩家动画
				p_isCombatAnimationPerson = true;
				personAttack();

				// 敌人的血量不为0，敌人出手后，玩家的血量
				if (p_enemyAttribute->p_enemyHealth != 0) enemyAttack();
			}
			else  // 敌人先出手
			{
				// 敌人攻击
				p_isCombatAnimationEnemy = true;
				enemyAttack();

				// 玩家攻击
				if (m_personCurrentHealth != 0) personAttack();

			}

			// 回合数 + 1
			m_htRoundNum += 1;
		}

		// 六层以后才会有技能按钮
		if (m_htFloor >= 6)
		{
			personSkillAttack(p_settingLanguage);
		}
		
		// GoOut button
		gooutButton(p_settingLanguage, isMainHtScreen, isHtScreen);

	}
	// Renderer attacking... text
	if (p_isCombatAnimationPerson and p_save01Data["isEnemyInformaion"])
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonAttacking"]["PersonAttacking"].dump()).c_str());
	}
	// No EnemyInformation button
	if (!p_isCombatAnimationPerson and !p_save01Data["isEnemyInformaion"])
	{
		// 7floor button and not open chests
		if (m_htFloor == 7)
		{
			// chests test
			if (!p_save01Data["isOpenChests_7floor"]) ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor007Chests1"].dump()).c_str());
			else {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
				ImGui::PopStyleColor(1);
				ImGui::SameLine();

				m_textureManager->imguiIMG(PNG_PRIMARYSTRENGTHENINGSTONE_ID, 16, 16);
				ImGui::SameLine();
				ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor007ChestsAward1"].dump()).c_str());
			}
			// open button, if person status
			if (p_save01Data["personCurrentStatus"] >= 10 and !p_save01Data["isOpenChests_7floor"])
			{
				if (ImGui::Button((removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["Open"].dump()).c_str())))
				{
					// status -10
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", p_save01Data["personCurrentStatus"] - 10);
					// open the chests
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOpenChests_7floor", true, true);
					// primary stone +1
					m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "primaryStrengtheninStoneCount", 1);
				}
			}
			else // not open chests
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 灰色表示禁用  
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 悬停时颜色不变  
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 按下时颜色不变
				ImGui::Button((removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["Open"].dump()).c_str()));
				ImGui::PopStyleColor(3);

				if (ImGui::IsItemHovered())
				{
					if (ImGui::BeginTooltip())
					{
						ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["NotEnoughStatusORopned"].dump()).c_str());
						ImGui::EndTooltip();
					}
				}
			}
			ImGui::SameLine();

			// go ahead button
			if (ImGui::Button((removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["GoAhead"].dump()).c_str())))
			{
				// display enemy information
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isEnemyInformaion", true, true);
			}
			// GoOut button , not allow go out
			// gooutButton(p_settingLanguage, isMainHtScreen, isHtScreen);
		}
	}
	

	ImGui::End();
}

void HtScreen::enemyInformationFrame(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 4 * 3), float(p_settingJson["SetWindowHeight"] / 3 + ATTACK_FRAME_HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 4), float(p_settingJson["SetWindowHeight"] / 3 * 2 - ATTACK_FRAME_HEIGHT)));
	ImGui::Begin("enemyInoformationFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	// mulit screen
	if(p_save01Data["isEnemyInformaion"])
	{
		// Enemy Attack
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyAttack"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text("%d", p_enemyAttribute->p_enemyAttack);
		// Enemy Defense
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyDefense"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text("%d", p_enemyAttribute->p_enemyDefense);
		// Enemy Speed
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemySpeed"].dump()).c_str());
		ImGui::SameLine();
		ImGui::Text("%d", p_enemyAttribute->p_enemySpeed);
	}

	// 7floor
	if (!p_save01Data["isEnemyInformaion"] and m_htFloor == 7 and !p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
	{
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["Chests"].dump()).c_str());
	}

	
	ImGui::End();
}

void HtScreen::personWinFrame(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{

	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 4), float(p_settingJson["SetWindowHeight"] / 3 + ATTACK_FRAME_HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 4 * 2), float(p_settingJson["SetWindowHeight"] / 3 * 2 - ATTACK_FRAME_HEIGHT)));
	ImGui::Begin("personWinFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// 奖励
	if (p_save01Data["HtFloor"] == 2)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor001Award"].dump()).c_str());

		if (p_save01Data["isOnceAward"]) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true); }
		ImGui::Text("");
	}
	if (p_save01Data["HtFloor"] == 3)
	{
		// 需要记录一个时间
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // 设置红色
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor002Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor002Award2"].dump()).c_str());
		// 原来基本上+20
		if (p_save01Data["isOnceAward"])
		{
			int gameGold = p_save01Data["personMoney"];
			gameGold += 20;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			//m_isOnceAward = false;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
		}

		ImGui::Text("");
	}
	if (p_save01Data["HtFloor"] == 4)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // 设置红色
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award2"].dump()).c_str());

		// 实现HT场景卖出新手剑为20G
		if (p_save01Data["isOnceAward"])
		{
			ImGui::Text("----------------------");
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award3"].dump()).c_str());
			if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Sell"].dump()).c_str()))
			{
				// 处理金币 + 20
				int gameGold = p_save01Data["personMoney"];
				gameGold += 20;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

				// 处理新手剑到商店
				m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", BEGINNERSWORD, 0);

				//m_isOnceAward = false;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
			}

			ImGui::SameLine();
			if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["NotSell"].dump()).c_str()))
			{
				// 处理新手剑到仓库
				m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", BEGINNERSWORD, 1);
				//m_isOnceAward = false;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
			}
			ImGui::Text("----------------------");
		}
	}
	if (p_save01Data["HtFloor"] == 5)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // 设置红色
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor004Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor004Award2"].dump()).c_str());
		if (p_save01Data["isOnceAward"])
		{
			// 处理金币 + 20
			int gameGold = p_save01Data["personMoney"];
			gameGold += 20;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			// 处理四件套装备到商店
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "HeadEquip", BEGINNERHELMET, 0);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "ArmsEquip", BEGINNERBARCERS, 0);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "BodyEquip", BEGINNERBREASTPLATE, 0);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "LegsEquip", BEGINNERKNEEPAD, 0);

			//m_isOnceAward = false;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
		}
	}
	if (p_save01Data["HtFloor"] == 6)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // 设置红色
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor005Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor005Award2"].dump()).c_str());
		if (p_save01Data["isOnceAward"])
		{
			// 处理金币 + 10
			int gameGold = p_save01Data["personMoney"];
			gameGold += 10;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			// 处理 防御技能为1，表示在仓库中，记录技能等级为1级
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", SKILLDEFENSE, 1);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkillLevel", "DefenseLevel", 1);
			// 处理 重击技能为0，表示在学院中可以学习，记录技能等级为1级
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", SKILLHEAVYATTACK, 0);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkillLevel", "HeavyAttackLevel", 1);

			// 保存可以携带技能数量， 默认可携带1个技能操
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personSkillNums", 1);
			// 保存已经携带的装备数量，默认是0，送的防御默认在仓库没有装备
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentSkillNums", 0);

			// 确保奖励只获得一次
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
		}
		if (!std::filesystem::exists("save/save01_5F.bin"))
		{
			//复制一份配置文件
			copyFile("save/save01.bin", "save/save01_5F.bin");
		}
	}
	if (p_save01Data["HtFloor"] == 7)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // setting red
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor006Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor006Award2"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor006Award3"].dump()).c_str());
		if (p_save01Data["isOnceAward"])
		{
			// deal with gold + 10
			int gameGold = p_save01Data["personMoney"];
			gameGold += 10;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			// deal with potions
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallHealthPotionCount", 1);
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "smallStatusPotionCount", 1);

			// deal with school learn : Health and Status
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorHealthNums", 0);
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "schoolJuniorStatusNums", 0);

			// ensure get once award
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);

			// not display enemy information
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isEnemyInformaion", false, true);
		}
	}


	// 进入战斗，按钮的实际作用时关闭框架
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["NextFloor"].dump()).c_str()))
	{

		m_htRoundNum = 0;  // 重置回合数

		// 清空敌人数据
		delete p_enemyAttribute;
		//p_enemyAttribute = nullptr;
		p_enemyAttribute = new EnemyAttribute(m_htFloor);

		// 关闭框架
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// 进入战斗后，重新reload敌人信息
		//m_isReloadAnimationInfor = true;
	}
	ImGui::SameLine(); ImGui::Text("    "); ImGui::SameLine();

	// 离开HT按钮, 还是胜利状态，没有进入下一层，仅作场景切换
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Leave"].dump()).c_str()))
	{
		m_htRoundNum = 0;  // 重置回合数

		// 场景切换
		isMainHtScreen = true;
		isHtScreen = false;

		// 离开时，清空敌人数据
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// 需要重新初始化以下人名，防止玩家做语言的切换
		m_isInfo = false;
	}

	ImGui::End();
}

void HtScreen::personLoseFrame(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 4), float(p_settingJson["SetWindowHeight"] / 3 + ATTACK_FRAME_HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"] / 4 * 2), float(p_settingJson["SetWindowHeight"] / 3 * 2 - ATTACK_FRAME_HEIGHT)));
	ImGui::Begin("personLoseFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonLoseFrame"]["FloorLoseText1"].dump()).c_str());
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonLoseFrame"]["FloorLoseText2"].dump()).c_str());
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonLoseFrame"]["FloorLoseText3"].dump()).c_str());
	ImGui::Text("");

	// 离开HT按钮。
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Leave"].dump()).c_str()))
	{
		// 游戏时间加1
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", m_gameTime += 1);

		// 关闭框架
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// 离开时，回合数，重置 0
		m_htRoundNum = 0;

		// 清空敌人数据
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// 场景切换
		isMainHtScreen = true;
		isHtScreen = false;
	}
	ImGui::End();
}

void HtScreen::turnBasedNum(json p_settingJson, json p_settingLanguage)
{
	std::string roundInformation = "  " + removeQuotes(p_settingLanguage["HtScreen"]["TurnBasedNum"]["Round"].dump()) + std::to_string(m_htRoundNum);
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] / 2 - 80), float(p_settingJson["SetWindowHeight"] / 12)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"]), float(p_settingJson["SetWindowHeight"] / 12)));
	ImGui::Begin("turnBasedNum", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	ImGui::TextColored(ImVec4(1.0f, 0, 0, 1.0f), roundInformation.c_str());
	//ImGui::Text(roundInformation.c_str());
	
	ImGui::End();
}

void HtScreen::animationUpdate(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson)
{
	// 人物速度快的战斗动画
	if (p_isCombatAnimationPerson)
	{
		// 如果是全屏，速度*2
		if (p_settingJson["isFullWindow"]) m_moveSpeed = 30;

		// 人物位置移动
		if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164) { m_personMoveX += m_moveSpeed; }

		m_AnimationCount += 1;
		if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000个循环次数后置0
		
		// 人物图片的移动的逻辑结构
		if (m_AnimationCount % 10 == 0) m_personAnimationX = 2;  // 10用来控制次数的，10个循环后切换
		else m_personAnimationX = 1;

		if (m_personMoveX >= p_settingJson["SetWindowWidth"] - 164)
		{

			m_attackNum += 1;
			animationNums += 1;
			if (m_attackNum == 3)
			{
				m_attackNum = 0;
				if (animationNums >= 24)  // 战斗渲染24次
				{
					m_personMoveX = 0;
					p_isCombatAnimationPerson = false;
					animationNums = 0;
				}

			}

		}
	}

	// 敌人移动快的战斗动画
	if (p_isCombatAnimationEnemy)
	{
		// 如果是全屏，速度*2
		if (p_settingJson["isFullWindow"]) m_moveSpeed = 30;

		// X轴的位置变化, 1024*768是44帧  ， 全屏的是132帧
		if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X) 
		{ 
			p_enemyMoveX -= m_moveSpeed; 
		}
		else // 战斗动画的渲染
		{
			m_attackNum += 1;
			animationNums += 1;
			if (m_attackNum == 3)
			{
				m_attackNum = 0;
				if (animationNums >= 24)  // 战斗渲染24次
				{
					p_enemyMoveX = p_settingJson["SetWindowWidth"] - 128;
					p_isCombatAnimationEnemy = false;
					animationNums = 0;
				}

			}
		}
	}

	// 敌人非战斗时的动画效果
	if (p_save01Data["isPersonWin"] != nullptr and p_save01Data["isPersonLose"] != nullptr)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			// 2 - 5层的史莱姆动画效果
			if ( (m_htFloor >= 2) and (m_htFloor <= 5) )
			{
				m_AnimationCount += 1;
				if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000个循环次数后置0

				if (m_AnimationCount % 10 == 0)
				{

					m_enemyAnimationY = p_settingJson["SetWindowHeight"] / 4 - 32;
					m_enemyAnimationHeight = PNG_USUAL_HEIGHT64 + 10;
				}
				else
				{
					m_enemyAnimationY = p_settingJson["SetWindowHeight"] / 4 - 42;
					m_enemyAnimationHeight = PNG_USUAL_HEIGHT64;
				}
			}
			else  // 5层以上
			{
				m_AnimationCount += 1;
				if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000个循环次数后置0

				// IDLE状态下, 0和1的切换
				if (!p_isCombatAnimationPerson and !p_isCombatAnimationEnemy)
				{
					if (m_AnimationCount % 10 == 0)m_enemyAnimationPicPos = 1;
					else m_enemyAnimationPicPos = 0;
				}
				if (!p_isCombatAnimationPerson and p_isCombatAnimationEnemy)
				{
					if (m_AnimationCount % 10 == 0)m_enemyAnimationPicPos = 2;
					else m_enemyAnimationPicPos = 3;
				}

			}
		}
		else // 图片位置状态还原
		{
			m_enemyAnimationPicPos = 0;
		}
		
	}



}

void HtScreen::endFrame(json p_settingJson, json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{
	ImGui::SetNextWindowPos(ImVec2(0, float(p_settingJson["SetWindowHeight"] / 3)));
	ImGui::SetNextWindowSize(ImVec2(float(p_settingJson["SetWindowWidth"]), 512));
	ImGui::Begin("endFrame", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	// test
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["endFrame"]["infor1"].dump()).c_str());
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["endFrame"]["infor2"].dump()).c_str());
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["endFrame"]["infor3"].dump()).c_str());
	ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["endFrame"]["infor4"].dump()).c_str());
	ImGui::Text("----------------");

	// button
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		// 关闭框架
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// 场景切换
		isMainHtScreen = true;
		isHtScreen = false;
	}

	ImGui::End();
}

void HtScreen::enemyAttack()
{
	m_personCurrentHealth = m_calCombatKPI->calNormalAttackAndDefense(m_personCurrentHealth, p_enemyAttribute->p_enemyAttack, m_personCurrentDefense);
	m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", m_personCurrentHealth);  // 玩家数据写配置文件

	// 如果玩家的血量到0
	if (m_personCurrentHealth == 0)
	{
		// 重置人物要移动到的位置坐标
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", true, true);

		// 写入后重新读取
		std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
		p_save01Data = json::from_bson(gameSaving);
		gameSaving.close();
	}
}

void HtScreen::personAttack()
{
	p_enemyAttribute->p_enemyHealth = m_calCombatKPI->calNormalAttackAndDefense(p_enemyAttribute->p_enemyHealth, m_personCurrentAttack, p_enemyAttribute->p_enemyDefense);
	// 如果敌人的血量到0
	if (p_enemyAttribute->p_enemyHealth == 0)
	{
		// 胜利后，层数+1, 获取一次奖励设为true
		m_htFloor += 1;
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "HtFloor", m_htFloor);
		//m_isOnceAward = true;
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", true, true);

		if (m_htFloor == 2) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime1", p_save01Data["gameTime"].dump()); }
		if (m_htFloor == 3) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime2", p_save01Data["gameTime"].dump()); }
		if (m_htFloor == 4) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime3", p_save01Data["gameTime"].dump()); }
		if (m_htFloor == 5) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime4", p_save01Data["gameTime"].dump()); }
		if (m_htFloor == 6) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime5", p_save01Data["gameTime"].dump()); }
		if (m_htFloor == 7) { m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "FloorTime6", p_save01Data["gameTime"].dump()); }

		// 渲染胜利框架
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", true, true);

		// 写入后重新读取
		std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
		p_save01Data = json::from_bson(gameSaving);
		gameSaving.close();
	}
}

void HtScreen::personSkillAttack(json p_settingLanguage)
{
	// 需要先找到人物装备了什么技能
	for (auto it = m_skillJson.begin(); it != m_skillJson.end(); ++it)
	{
		// 说明已经装备了
		if (it.value() == 2)
		{
			//防御技能
			if (it.key() == SKILLDEFENSE)
			{
				ImGui::SameLine();
				ImGui::Text("  "); // 空两格
				ImGui::SameLine();
				// 按下防御按钮以后的效果
				if (m_personCurrentStatus >= 20)
				{
					if (ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["Defense"].dump()).c_str()))
					{
						// 保证敌人先攻击，保证当前回合人物防御力+2，人物的状态-20 ， 本次战斗人物速度+1
						// 敌人攻击完成后，人物当前的速度+1 在personInformationFrame框架里面加
						m_personCurrentDefense += 2;

						// 减掉后的状态要写配置文件
						m_personCurrentStatus -= 20;
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", m_personCurrentStatus);

						// 人物发生变化的属性值的颜色，要发生变化
						p_isCombatAnimationEnemy = true; // 动画效果后面在调整
						enemyAttack();

						m_htRoundNum += 1;

						m_isDefenseButton = true;

					}
					if (ImGui::IsItemHovered())
					{
						if (ImGui::BeginTooltip())
						{
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["DefenseDes"].dump()).c_str());
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["DefenseCost"].dump()).c_str());
							ImGui::EndTooltip();
						}
					}
				}
				else // 状态不足的情况，可以按，
				{
					// 没有效果的按钮
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 灰色表示禁用  
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 悬停时颜色不变  
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 按下时颜色不变
					ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["Defense"].dump()).c_str());
					ImGui::PopStyleColor(3);
					// 显然人物状态不足的tooltip
					if (ImGui::IsItemHovered())
					{
						if (ImGui::BeginTooltip())
						{
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["PersonStatusNotEnough"].dump()).c_str());
							ImGui::EndTooltip();
						}
					}
				}
				

			}
			//重击技能
			if (it.key() == SKILLHEAVYATTACK)
			{
				ImGui::SameLine();
				ImGui::Text("  "); // 空两格
				ImGui::SameLine();
				// 按下重击按钮以后的效果
				if (m_personCurrentStatus >= 20)
				{
					if (ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["HeavyAttack"].dump()).c_str()))
					{
						// 做攻击力+2
						m_personCurrentAttack += 2;

						// 攻击速度判断，并进行攻击
						if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
						{
							// 玩家出手后，渲染玩家动画
							p_isCombatAnimationPerson = true;
							personAttack();

							// 敌人的血量不为0，敌人出手后，玩家的血量
							if (p_enemyAttribute->p_enemyHealth != 0) enemyAttack();
						}
						else  // 敌人先出手
						{
							// 敌人攻击
							p_isCombatAnimationEnemy = true;
							enemyAttack();

							// 玩家攻击
							if (m_personCurrentHealth != 0) personAttack();

						}

						// 回合数 + 1
						m_htRoundNum += 1;

						// 减掉后的状态要写配置文件
						m_personCurrentStatus -= 20;
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", m_personCurrentStatus);

						m_isHeavyAttackButton = true;
					}
					if (ImGui::IsItemHovered())
					{
						if (ImGui::BeginTooltip())
						{
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["HeavyAttackDes"].dump()).c_str());
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["HeavyAttackCost"].dump()).c_str());
							ImGui::EndTooltip();
						}
					}
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 灰色表示禁用  
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 悬停时颜色不变  
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // 按下时颜色不变
					ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["HeavyAttack"].dump()).c_str());
					ImGui::PopStyleColor(3);

					if (ImGui::IsItemHovered())
					{
						if (ImGui::BeginTooltip())
						{
							ImGui::Text(removeQuotes(p_settingLanguage["SkillsToolTip"]["PersonStatusNotEnough"].dump()).c_str());
							ImGui::EndTooltip();
						}
					}
				}

			}
		}
	}
}

void HtScreen::gooutButton(json p_settingLanguage, bool& isMainHtScreen, bool& isHtScreen)
{

	ImGui::Text("---------");
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["GoOut"].dump()).c_str()))
	{
		isMainHtScreen = true;
		isHtScreen = false;

		// 离开时，要做天数 + 1
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", m_gameTime += 1);
		// 离开时，回合数，重置 1
		m_htRoundNum = 0;
		// 离开时，清空敌人数据
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// 需要重新初始化以下人名，防止玩家做语言的切换
		m_isInfo = false;
	}
	if (ImGui::IsItemHovered())
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["GoOutToolTip"].dump()).c_str());
			ImGui::EndTooltip();
		}
	}
}

void HtScreen::showEnemyName_healty_status_Floor001(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR001NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - 16, 0, m_textureManager->p_EnemyNameWidthFloor001, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// 敌人的血条框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条红色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor002(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR002NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - 16, 0, m_textureManager->p_EnemyNameWidthFloor002, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// 敌人的血条框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条红色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor003(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR003NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - 16, 0, m_textureManager->p_EnemyNameWidthFloor003, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// 敌人的血条框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条红色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor004(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR004NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - 16, 0, m_textureManager->p_EnemyNameWidthFloor004, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// 敌人的血条框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条红色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor005(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR005NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - 16, 0, m_textureManager->p_EnemyNameWidthFloor005, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// 敌人的血条框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条红色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor006(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR006NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - 16, 0, m_textureManager->p_EnemyNameWidthFloor006, TTF_PLAYER_NAME_HEIGHT);
	
	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect); 
	// 敌人的血条的上限框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条框为黑色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);

	// 敌人蓝条框，动态变化
	m_enemyCurrentStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyStatus - 20, 32, p_enemyAttribute->p_enemyStatus, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentStatusRect);
	// 敌人蓝条的上限框
	m_enemyAllStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemeStatusLimit - 20, 32, p_enemyAttribute->p_enemeStatusLimit, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置蓝条框为黑色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllStatusRect);
}

void HtScreen::showEnemyName_healty_status_Floor007(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// 敌人的姓名 -16 是留边16
	m_textureManager->render(ENEMY_FLOOR007NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - 16, 0, m_textureManager->p_EnemyNameWidthFloor007, TTF_PLAYER_NAME_HEIGHT);

	// 敌人的血条框 -20 是留边16 和 4像素的名字和框之间的距离，会随着敌人血量的减少而减少
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);
	// 敌人的血条的上限框
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置血条框为黑色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);

	// 敌人蓝条框，动态变化
	m_enemyCurrentStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyStatus - 20, 32, p_enemyAttribute->p_enemyStatus, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //设置血条红色
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentStatusRect);
	// 敌人蓝条的上限框
	m_enemyAllStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemeStatusLimit - 20, 32, p_enemyAttribute->p_enemeStatusLimit, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //设置蓝条框为黑色
	SDL_RenderDrawRect(m_gRender, &m_enemyAllStatusRect);
}