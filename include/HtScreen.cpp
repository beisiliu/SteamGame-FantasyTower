#include"HtScreen.h"

HtScreen::HtScreen()
{
	p_isCombatAnimationPerson = false;
	p_isCombatAnimationEnemy = false;

	m_writeData = new WriteData();
	m_personButton = new PersonButton();

	p_enemyAttribute = nullptr;  // ��render��ʱ����г�ʼ��
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

	//ImGuiÿ��ѭ������һ���µ�frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	healthAndStatusToolTip(m_textureManager, p_settingJson, p_settingLanguage);  // ����Ѫ����״̬����tooltip
	enemyHealthAndStatusToolTip(m_textureManager, p_settingJson, p_settingLanguage); // ����Ѫ����״̬����tooltip
	turnBasedNum(p_settingJson, p_settingLanguage);  // �غ���
	if (p_save01Data["HtFloor"] <= 7)
	{
		attackLogFrame(p_settingJson, p_settingLanguage); // ս����־���
		personInformationFrame(p_settingJson, p_settingLanguage, m_textureManager); // ������Ϣ���
		enemyInformationFrame(p_settingJson, p_settingLanguage); // ������Ϣ���

		// ս����ť���
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			buttonFrame(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
		// ս��ʤ�����
		if (p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			personWinFrame(m_textureManager, m_gRender, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
		// ս��ʧ�ܿ��
		if (!p_save01Data["isPersonWin"] and p_save01Data["isPersonLose"])
		{
			personLoseFrame(m_textureManager, m_gRender, p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
		}
	}
	else
	{
		// ��Ϸͨ�صĿ��
		endFrame(p_settingJson, p_settingLanguage, isMainHtScreen, isHtScreen);
	}


	if (isHtScreen)  // ���isHtScreen��Ϊtrue���Ͳ���Ⱦ��ͨ��else�еĴ���������� ������㰴ť������ܵ�bug
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
	// ������
	ImGui::Render();
	SDL_SetRenderDrawColor(m_gRender, 28, 28, 28, 255);  // ���ñ���ɫ - ��ɫ
	SDL_RenderClear(m_gRender);

	// ս��������Render
	// ս�������ı���ǽ
	m_textureManager->render(PNG_BACKGROUND_HT_ATTACK_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"] / 4 + 64);
	m_textureManager->render(PNG_BACKGROUND_HT_ID, m_gRender, 0, p_settingJson["SetWindowHeight"] / 12 * 4, p_settingJson["SetWindowWidth"], int(p_settingJson["SetWindowHeight"] / 1.5));

	// ����������Ⱦ
	m_textureManager->render(TTF_PLAYER_NAME_ID, m_gRender, int(p_settingJson["SetWindowWidth"] *0.015), 0, m_textureManager->p_playerNameWidth, TTF_PLAYER_NAME_HEIGHT);

	// ����Ѫ����״̬����Ⱦ
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); // ���ú�ɫ���޿�
	SDL_RenderDrawRect(m_gRender, &m_personAllHealthRect);  // Ѫ�����޿� ��δ���ƣ��ݶ�500����
	SDL_RenderDrawRect(m_gRender, &m_personAllStatusRect);  // ״̬���޿� ��δ���ƣ��ݶ�500����

	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_personCurrentHealthRect);

	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //����״̬����ɫ
	SDL_RenderFillRect(m_gRender, &m_personCurrentStatusRect);

	// ս��������Render
	// ��Ⱦʤ��ͼ��͵��˵���
	if (p_save01Data["isPersonWin"])
	{
		m_textureManager->render(PNG_GAMEWIND_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2, p_settingJson["SetWindowHeight"] / 12, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
		if (m_htFloor == 2)
		{
			// ���˵�����Ⱦ�� ��ѹ��
			m_textureManager->dynamicRender(ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 + 32 - PNG_USUAL_WIDTH64, 64, 128, 1, 1, -90.0);
			// ��ȾѪ��
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 3)
		{
			// ���˵�����Ⱦ�� ��ѹ��
			m_textureManager->scaleRender(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// m_textureManager->render(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ��ȾѪ��
			showEnemyName_healty_status_Floor002(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 4)
		{
			// ���˵�����Ⱦ�� ��ѹ��
			m_textureManager->scaleRender(ENEMY_FLOOR003_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// ��ȾѪ��
			showEnemyName_healty_status_Floor003(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 5)
		{
			// ���˵�����Ⱦ�� ��ѹ��
			m_textureManager->scaleRender(ENEMY_FLOOR004_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// ��ȾѪ��
			showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 6)
		{
			// ���˵�����Ⱦ�� ��ѹ��
			m_textureManager->scaleRender(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4, 96, 96, 1, 4);
			// ��ȾѪ��
			showEnemyName_healty_status_Floor005(m_textureManager, m_gRender, p_settingJson);
		}
	}
	// ��Ⱦʧ��ͼ��
	if (p_save01Data["isPersonLose"])
	{
		m_textureManager->render(PNG_GAMELOSE_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2, p_settingJson["SetWindowHeight"] / 12, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
		// ���ﵹ����Ⱦ����ѹ��
		m_textureManager->dynamicRender(PNG_ANIMATION_PERSON_ID, m_gRender, 32, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_HEIGHT64 + 32, 64, 128, 1, 1, 90.0);

		if (m_htFloor == 1)
		{
			// ���˺���Ⱦ����
			m_textureManager->dynamicRender(ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, 128, 128, 1, 1);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 2)
		{
			// ���˺���Ⱦ����
			m_textureManager->render(ENEMY_FLOOR002_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor002(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 3)
		{
			// ���˺���Ⱦ����
			m_textureManager->render(ENEMY_FLOOR003_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor003(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 4)
		{
			// ���˺���Ⱦ����
			m_textureManager->render(ENEMY_FLOOR004_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor004(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 5)
		{
			// ���˺���Ⱦ����
			m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor005(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 6)
		{
			// ���˺���Ⱦ����
			// m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor006(m_textureManager, m_gRender, p_settingJson);
		}
		if (m_htFloor == 7)
		{
			// ���˺���Ⱦ����
			// m_textureManager->render(ENEMY_FLOOR005_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - 32, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
			// ���˺���Ⱦ���˵�Ѫ��
			showEnemyName_healty_status_Floor007(m_textureManager, m_gRender, p_settingJson);
		}
	}
	else  // ֻҪû���������£��������ƶ���Ⱦ
	{
		// ������Ⱦ��m_personMoveX ���������ƶ���
		// �������ƶ�������λ��ʱ��������жϣ�������ʧ
		
		if (m_personMoveX <  p_settingJson["SetWindowWidth"] - 164 and p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
		{
			m_textureManager->dynamicRender(PNG_ANIMATION_PERSON_ID, m_gRender, m_personMoveX, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_HEIGHT64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_personAnimationX, 1, 2);
		}
	}

	// Render 1floor enemy
	if (m_htFloor == 1)
	{
		// һ����˵����ƺ�Ѫ��
		// ֻ���������Ⱦ����Ϊclear��present֮��
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor001(m_textureManager, m_gRender, p_settingJson);
			// �������ƶ�������λ��ʱ�����˵��жϣ��õ�����ʧ
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
			// ��� Ҫ����������ˣ�����������ٶȿ����Ⱦ���
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
			else  // �����ǵ����ٶȿ�����
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
			// ��� Ҫ����������ˣ�����������ٶȿ����Ⱦ���
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
			else  // �����ǵ����ٶȿ�����
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
			// ��� Ҫ����������ˣ�����������ٶȿ����Ⱦ���
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
			else  // �����ǵ����ٶȿ�����
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

		// ��������£��Ქ�ŵ��˺������ս������
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			showEnemyName_healty_status_Floor006(m_textureManager, m_gRender, p_settingJson);
			// ��� Ҫ����������ˣ�����������ٶȿ����Ⱦ�������֤������ť�º�ִ��
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed and !m_isDefenseButton)
			{
				if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
				{
					// ����idle��״̬
					m_textureManager->dynamicRender(ENEMY_FLOOR006_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
					// �貹�������ܶ��Ķ�����Ȼ�����else

				}
				else
				{
					m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
				}
			}
			else // �����ǵ����ٶȿ�����
			{
				if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
				{
					// idle�����
					if (!p_isCombatAnimationPerson and !p_isCombatAnimationEnemy)
					{
						m_textureManager->dynamicRender(ENEMY_FLOOR006_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
					}
					// �ܶ������
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
				// ��� Ҫ����������ˣ�����������ٶȿ����Ⱦ�������֤������ť�º�ִ��
				if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed and !m_isDefenseButton)
				{
					if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164)
					{
						// ����idle��״̬
						m_textureManager->dynamicRender(ENEMY_FLOOR007_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
						// �貹�������ܶ��Ķ�����Ȼ�����else

					}
					else
					{
						m_textureManager->dynamicRender(ANIMATIONHT_ATTACK_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128, p_settingJson["SetWindowHeight"] / 4 - PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_attackNum, 1, 2);
					}
				}
				else // �����ǵ����ٶȿ�����
				{
					if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X)
					{
						// idle�����
						if (!p_isCombatAnimationPerson and !p_isCombatAnimationEnemy)
						{
							m_textureManager->dynamicRender(ENEMY_FLOOR007_ID, m_gRender, p_settingJson["SetWindowWidth"] - 128 - 16, p_settingJson["SetWindowHeight"] / 4 - 62, PNG_USUAL_WIDTH64, PNG_USUAL_WIDTH64, m_enemyAnimationPicPos, 1, 2);
						}
						// �ܶ������
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
	
	// ImGui��Ⱦ
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_gRender);
}

void HtScreen::initHtGameData(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// ÿ��render��ʱ�򣬶�Ҫ��ȥ����������ļ���������ʵʱ�����ݸ��£��γɾֲ�����
	std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
	p_save01Data = json::from_bson(gameSaving);
	gameSaving.close();

	// ��ʼ����������
	m_personButton->initPersonData(p_save01Data);

	//��ʼ��Ѫ����״̬�����KPI
	m_personCurrentHealth = p_save01Data["personCurrentHealth"];
	m_personAllHealth = p_save01Data["personAllHealth"];
	m_personCurrentStatus = p_save01Data["personCurrentStatus"];
	m_personAllStatus = p_save01Data["personAllStatus"];

	//���״ֵ̬Ϊ������ҪֵΪ0��
	if (m_personCurrentStatus < 0) m_personCurrentStatus = 0;

	m_personAllHealthRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10 - 1, 5 - 1, m_personAllHealth + 2, 25 + 2 };
	m_personCurrentHealthRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10, 5, m_personCurrentHealth, 25 };
	m_personAllStatusRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10 - 1, 38 - 1, m_personAllStatus + 2, 25 + 2 };
	m_personCurrentStatusRect = { int(p_settingJson["SetWindowWidth"] * 0.015) + m_textureManager->p_playerNameWidth + 10, 38, m_personCurrentStatus, 25 };

	// ��ȡ��Ϸ����
	m_gameTime = p_save01Data["gameTime"];
	// ��ȡHT����
	m_htFloor = p_save01Data["HtFloor"];

	// ��ʼ��HT�������ݣ��ڵ�����ܺ��뿪��ť��
	if (!m_isInfo)
	{
		// �������Գ�ʼ��
		m_personCurrentAttack = m_personButton->calPersonCurrentAccack(); // ���﹥��
		m_personCurrentDefense = m_personButton->calPersonCurrentDefense(); // �������
		m_personCurrentSpeed = m_personButton->calPersonCurrentSpeed(); // �����ٶ�

		// ��ȡ���＼�ܵ�json
		m_skillJson = p_save01Data["personSkill"];

		// �������г�ʼ��Enemy����
		m_enemyNameArray = {
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor01Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor02Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor03Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor04Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor05Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor06Name"].dump()),
			removeQuotes(p_settingLanguage["HtScreen"]["EnemyInformationFrame"]["EnemyFloor07Name"].dump()),
		};
		
		// ���ص������Ƶ�texture
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR001NAME_ID, m_enemyNameArray[0].c_str(), 1, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR002NAME_ID, m_enemyNameArray[1].c_str(), 2, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR003NAME_ID, m_enemyNameArray[2].c_str(), 3, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR004NAME_ID, m_enemyNameArray[3].c_str(), 4, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR005NAME_ID, m_enemyNameArray[4].c_str(), 5, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR006NAME_ID, m_enemyNameArray[5].c_str(), 6, m_gRender, { 0, 255, 255 });
		m_textureManager->loadTTFEnemyName(ENEMY_FLOOR007NAME_ID, m_enemyNameArray[6].c_str(), 7, m_gRender, { 0, 255, 255 });

		// �����ظ�����
		m_isInfo = true;
	}

	// Ҫ���¼�������*0.8
	if ((static_cast<float>(m_personCurrentStatus) / m_personAllStatus) * 100 < 50)
	{
		m_personCurrentAttack = m_personButton->calPersonCurrentAccack(); // ���﹥��
		m_personCurrentDefense = m_personButton->calPersonCurrentDefense(); // �������
		m_personCurrentSpeed = m_personButton->calPersonCurrentSpeed(); // �����ٶ�
	}

}

void HtScreen::healthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// ��������ƶ���Ѫ����״̬����ʾ��ֵ - toolTip
	ImGui::SetNextWindowSize(ImVec2(512, PNG_USUAL_HEIGHT64 + 48));
	//ImGui::SetNextWindowPos(ImVec2(float(TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10), 0.0f));
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"]* 0.015 + m_textureManager->p_playerNameWidth), 0.0f));
	ImGui::Begin("healthAndStatusTooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	// ��������personAllHealth������personAllStatus����ʾinvisibleButton�Ŀ��
	if (p_save01Data["personAllHealth"] >= p_save01Data["personAllStatus"])
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(float(p_save01Data["personAllHealth"]), float(p_settingJson["SetWindowHeight"] * 0.08)));
	}
	else
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(float(p_save01Data["personAllStatus"]), float(p_settingJson["SetWindowHeight"] * 0.08)));
	}
	// ������ƶ���invisibleButton��ʱ����ʾtooltip
	// �ֲ�����
	if (ImGui::IsItemHovered())
	{
		// Ѫ����toolTip
		std::string healthToolTip_name = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
		std::string healthToolTip_current = std::to_string(m_personCurrentHealth);
		std::string healthToolTip_all = std::to_string(m_personAllHealth);
		std::string healthToolTip = healthToolTip_name + ":" + healthToolTip_current + "/" + healthToolTip_all;

		// ״̬����toopTip
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
	// ��ս��������toolTip
	if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
	{
		if (m_htFloor == 1) 
		{
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor001Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor002Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor003Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2),float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor004Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor005Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
			ImGui::InvisibleButton("showEnemyHealthFloor005Tooltip", ImVec2(float(p_enemyAttribute->p_enemyHealthLimit), 16.0f));

			// Ѫ�����ַ���ƴ��
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(50 + 6)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor006Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// Ѫ�����ַ���ƴ��
			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			// �������ַ���ƴ��
			std::string status = p_settingLanguage["MainMeum"]["Tooltip"]["Status"].dump();
			std::string enemyCurrentStatus = std::to_string(p_enemyAttribute->p_enemyStatus);
			std::string enemyAllStatus = std::to_string(p_enemyAttribute->p_enemeStatusLimit);
			std::string enemyStatusStr = status + ":" + enemyCurrentStatus + "/" + enemyAllStatus;


			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(50 + 6)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor007Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// Ѫ�����ַ���ƴ��
			std::string health = p_settingLanguage["MainMeum"]["Tooltip"]["Health"].dump();
			std::string enemyCurrentHealth = std::to_string(p_enemyAttribute->p_enemyHealth);
			std::string enemyAllHealth = std::to_string(p_enemyAttribute->p_enemyHealthLimit);
			std::string enemyHealthStr = health + ":" + enemyCurrentHealth + "/" + enemyAllHealth;

			// �������ַ���ƴ��
			std::string status = p_settingLanguage["MainMeum"]["Tooltip"]["Status"].dump();
			std::string enemyCurrentStatus = std::to_string(p_enemyAttribute->p_enemyStatus);
			std::string enemyAllStatus = std::to_string(p_enemyAttribute->p_enemeStatusLimit);
			std::string enemyStatusStr = status + ":" + enemyCurrentStatus + "/" + enemyAllStatus;


			// ���ذ�ť����tooltip�ҿ�
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
	// ��ս��ʧ�ܳ���
	if (p_save01Data["isPersonLose"])
	{
		if (m_htFloor == 1)
		{
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor001Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor002Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor003Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor004Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor005Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor006Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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
			// ����tooltip
			ImGui::SetNextWindowSize(ImVec2(float(p_enemyAttribute->p_enemyHealthLimit + 2), float(25 + 2)));
			ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 29), 0.0f));
			ImGui::Begin("enemyHealthFloor007Tooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			// ���ذ�ť����tooltip�ҿ�
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

void HtScreen::attackLogFrame(json p_settingJson, json p_settingLanguage)  // �ƻ���Щ��Ϸ��ʾ��������־�������ڿ���
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

	// defense button - defense restore, !p_isCombatAnimationEnemy��when playing animation��keep defense +2
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
		// ��ͨ������ť
		if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["ButtonFrame"]["Attack"].dump()).c_str()))
		{
			if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
			{
				// ��ҳ��ֺ���Ⱦ��Ҷ���
				p_isCombatAnimationPerson = true;
				personAttack();

				// ���˵�Ѫ����Ϊ0�����˳��ֺ���ҵ�Ѫ��
				if (p_enemyAttribute->p_enemyHealth != 0) enemyAttack();
			}
			else  // �����ȳ���
			{
				// ���˹���
				p_isCombatAnimationEnemy = true;
				enemyAttack();

				// ��ҹ���
				if (m_personCurrentHealth != 0) personAttack();

			}

			// �غ��� + 1
			m_htRoundNum += 1;
		}

		// �����Ժ�Ż��м��ܰ�ť
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
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ɫ��ʾ����  
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ͣʱ��ɫ����  
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ����ʱ��ɫ����
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

	// ����
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
		// ��Ҫ��¼һ��ʱ��
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // ���ú�ɫ
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor002Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor002Award2"].dump()).c_str());
		// ԭ��������+20
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
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // ���ú�ɫ
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award2"].dump()).c_str());

		// ʵ��HT�����������ֽ�Ϊ20G
		if (p_save01Data["isOnceAward"])
		{
			ImGui::Text("----------------------");
			ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor003Award3"].dump()).c_str());
			if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Sell"].dump()).c_str()))
			{
				// ������ + 20
				int gameGold = p_save01Data["personMoney"];
				gameGold += 20;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

				// �������ֽ����̵�
				m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", BEGINNERSWORD, 0);

				//m_isOnceAward = false;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
			}

			ImGui::SameLine();
			if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["NotSell"].dump()).c_str()))
			{
				// �������ֽ����ֿ�
				m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "Weapons", BEGINNERSWORD, 1);
				//m_isOnceAward = false;
				m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
			}
			ImGui::Text("----------------------");
		}
	}
	if (p_save01Data["HtFloor"] == 5)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // ���ú�ɫ
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor004Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor004Award2"].dump()).c_str());
		if (p_save01Data["isOnceAward"])
		{
			// ������ + 20
			int gameGold = p_save01Data["personMoney"];
			gameGold += 20;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			// �����ļ���װ�����̵�
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
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // ���ú�ɫ
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Award"].dump()).c_str());
		ImGui::PopStyleColor(1);
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor005Award1"].dump()).c_str());
		ImGui::Text(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Floor005Award2"].dump()).c_str());
		if (p_save01Data["isOnceAward"])
		{
			// ������ + 10
			int gameGold = p_save01Data["personMoney"];
			gameGold += 10;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", gameGold);

			// ���� ��������Ϊ1����ʾ�ڲֿ��У���¼���ܵȼ�Ϊ1��
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", SKILLDEFENSE, 1);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkillLevel", "DefenseLevel", 1);
			// ���� �ػ�����Ϊ0����ʾ��ѧԺ�п���ѧϰ����¼���ܵȼ�Ϊ1��
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkill", SKILLHEAVYATTACK, 0);
			m_writeData->writeBsonBin2(FILE_GAME_SAVE_PATH, "personSkillLevel", "HeavyAttackLevel", 1);

			// �������Я������������ Ĭ�Ͽ�Я��1�����ܲ�
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personSkillNums", 1);
			// �����Ѿ�Я����װ��������Ĭ����0���͵ķ���Ĭ���ڲֿ�û��װ��
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentSkillNums", 0);

			// ȷ������ֻ���һ��
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isOnceAward", false, true);
		}
		if (!std::filesystem::exists("save/save01_5F.bin"))
		{
			//����һ�������ļ�
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


	// ����ս������ť��ʵ������ʱ�رտ��
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["NextFloor"].dump()).c_str()))
	{

		m_htRoundNum = 0;  // ���ûغ���

		// ��յ�������
		delete p_enemyAttribute;
		//p_enemyAttribute = nullptr;
		p_enemyAttribute = new EnemyAttribute(m_htFloor);

		// �رտ��
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// ����ս��������reload������Ϣ
		//m_isReloadAnimationInfor = true;
	}
	ImGui::SameLine(); ImGui::Text("    "); ImGui::SameLine();

	// �뿪HT��ť, ����ʤ��״̬��û�н�����һ�㣬���������л�
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Leave"].dump()).c_str()))
	{
		m_htRoundNum = 0;  // ���ûغ���

		// �����л�
		isMainHtScreen = true;
		isHtScreen = false;

		// �뿪ʱ����յ�������
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// ��Ҫ���³�ʼ��������������ֹ��������Ե��л�
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

	// �뿪HT��ť��
	if (ImGui::Button(removeQuotes(p_settingLanguage["HtScreen"]["PersonWinFrame"]["Leave"].dump()).c_str()))
	{
		// ��Ϸʱ���1
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", m_gameTime += 1);

		// �رտ��
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// �뿪ʱ���غ��������� 0
		m_htRoundNum = 0;

		// ��յ�������
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// �����л�
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
	// �����ٶȿ��ս������
	if (p_isCombatAnimationPerson)
	{
		// �����ȫ�����ٶ�*2
		if (p_settingJson["isFullWindow"]) m_moveSpeed = 30;

		// ����λ���ƶ�
		if (m_personMoveX < p_settingJson["SetWindowWidth"] - 164) { m_personMoveX += m_moveSpeed; }

		m_AnimationCount += 1;
		if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000��ѭ����������0
		
		// ����ͼƬ���ƶ����߼��ṹ
		if (m_AnimationCount % 10 == 0) m_personAnimationX = 2;  // 10�������ƴ����ģ�10��ѭ�����л�
		else m_personAnimationX = 1;

		if (m_personMoveX >= p_settingJson["SetWindowWidth"] - 164)
		{

			m_attackNum += 1;
			animationNums += 1;
			if (m_attackNum == 3)
			{
				m_attackNum = 0;
				if (animationNums >= 24)  // ս����Ⱦ24��
				{
					m_personMoveX = 0;
					p_isCombatAnimationPerson = false;
					animationNums = 0;
				}

			}

		}
	}

	// �����ƶ����ս������
	if (p_isCombatAnimationEnemy)
	{
		// �����ȫ�����ٶ�*2
		if (p_settingJson["isFullWindow"]) m_moveSpeed = 30;

		// X���λ�ñ仯, 1024*768��44֡  �� ȫ������132֡
		if (p_enemyMoveX > ENEMY_ANIMATION_MOVE_X) 
		{ 
			p_enemyMoveX -= m_moveSpeed; 
		}
		else // ս����������Ⱦ
		{
			m_attackNum += 1;
			animationNums += 1;
			if (m_attackNum == 3)
			{
				m_attackNum = 0;
				if (animationNums >= 24)  // ս����Ⱦ24��
				{
					p_enemyMoveX = p_settingJson["SetWindowWidth"] - 128;
					p_isCombatAnimationEnemy = false;
					animationNums = 0;
				}

			}
		}
	}

	// ���˷�ս��ʱ�Ķ���Ч��
	if (p_save01Data["isPersonWin"] != nullptr and p_save01Data["isPersonLose"] != nullptr)
	{
		if (!p_save01Data["isPersonWin"] and !p_save01Data["isPersonLose"])
		{
			// 2 - 5���ʷ��ķ����Ч��
			if ( (m_htFloor >= 2) and (m_htFloor <= 5) )
			{
				m_AnimationCount += 1;
				if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000��ѭ����������0

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
			else  // 5������
			{
				m_AnimationCount += 1;
				if (m_AnimationCount == 1000) m_AnimationCount = 0;   // 1000��ѭ����������0

				// IDLE״̬��, 0��1���л�
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
		else // ͼƬλ��״̬��ԭ
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
		// �رտ��
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", false, true);
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", false, true);

		// �����л�
		isMainHtScreen = true;
		isHtScreen = false;
	}

	ImGui::End();
}

void HtScreen::enemyAttack()
{
	m_personCurrentHealth = m_calCombatKPI->calNormalAttackAndDefense(m_personCurrentHealth, p_enemyAttribute->p_enemyAttack, m_personCurrentDefense);
	m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentHealth", m_personCurrentHealth);  // �������д�����ļ�

	// �����ҵ�Ѫ����0
	if (m_personCurrentHealth == 0)
	{
		// ��������Ҫ�ƶ�����λ������
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonLose", true, true);

		// д������¶�ȡ
		std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
		p_save01Data = json::from_bson(gameSaving);
		gameSaving.close();
	}
}

void HtScreen::personAttack()
{
	p_enemyAttribute->p_enemyHealth = m_calCombatKPI->calNormalAttackAndDefense(p_enemyAttribute->p_enemyHealth, m_personCurrentAttack, p_enemyAttribute->p_enemyDefense);
	// ������˵�Ѫ����0
	if (p_enemyAttribute->p_enemyHealth == 0)
	{
		// ʤ���󣬲���+1, ��ȡһ�ν�����Ϊtrue
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

		// ��Ⱦʤ�����
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isPersonWin", true, true);

		// д������¶�ȡ
		std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
		p_save01Data = json::from_bson(gameSaving);
		gameSaving.close();
	}
}

void HtScreen::personSkillAttack(json p_settingLanguage)
{
	// ��Ҫ���ҵ�����װ����ʲô����
	for (auto it = m_skillJson.begin(); it != m_skillJson.end(); ++it)
	{
		// ˵���Ѿ�װ����
		if (it.value() == 2)
		{
			//��������
			if (it.key() == SKILLDEFENSE)
			{
				ImGui::SameLine();
				ImGui::Text("  "); // ������
				ImGui::SameLine();
				// ���·�����ť�Ժ��Ч��
				if (m_personCurrentStatus >= 20)
				{
					if (ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["Defense"].dump()).c_str()))
					{
						// ��֤�����ȹ�������֤��ǰ�غ����������+2�������״̬-20 �� ����ս�������ٶ�+1
						// ���˹�����ɺ����ﵱǰ���ٶ�+1 ��personInformationFrame��������
						m_personCurrentDefense += 2;

						// �������״̬Ҫд�����ļ�
						m_personCurrentStatus -= 20;
						m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personCurrentStatus", m_personCurrentStatus);

						// ���﷢���仯������ֵ����ɫ��Ҫ�����仯
						p_isCombatAnimationEnemy = true; // ����Ч�������ڵ���
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
				else // ״̬�������������԰���
				{
					// û��Ч���İ�ť
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ɫ��ʾ����  
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ͣʱ��ɫ����  
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ����ʱ��ɫ����
					ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["Defense"].dump()).c_str());
					ImGui::PopStyleColor(3);
					// ��Ȼ����״̬�����tooltip
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
			//�ػ�����
			if (it.key() == SKILLHEAVYATTACK)
			{
				ImGui::SameLine();
				ImGui::Text("  "); // ������
				ImGui::SameLine();
				// �����ػ���ť�Ժ��Ч��
				if (m_personCurrentStatus >= 20)
				{
					if (ImGui::Button(removeQuotes(p_settingLanguage["Skills"]["HeavyAttack"].dump()).c_str()))
					{
						// ��������+2
						m_personCurrentAttack += 2;

						// �����ٶ��жϣ������й���
						if (m_personCurrentSpeed >= p_enemyAttribute->p_enemySpeed)
						{
							// ��ҳ��ֺ���Ⱦ��Ҷ���
							p_isCombatAnimationPerson = true;
							personAttack();

							// ���˵�Ѫ����Ϊ0�����˳��ֺ���ҵ�Ѫ��
							if (p_enemyAttribute->p_enemyHealth != 0) enemyAttack();
						}
						else  // �����ȳ���
						{
							// ���˹���
							p_isCombatAnimationEnemy = true;
							enemyAttack();

							// ��ҹ���
							if (m_personCurrentHealth != 0) personAttack();

						}

						// �غ��� + 1
						m_htRoundNum += 1;

						// �������״̬Ҫд�����ļ�
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
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ɫ��ʾ����  
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ��ͣʱ��ɫ����  
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // ����ʱ��ɫ����
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

		// �뿪ʱ��Ҫ������ + 1
		m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "gameTime", m_gameTime += 1);
		// �뿪ʱ���غ��������� 1
		m_htRoundNum = 0;
		// �뿪ʱ����յ�������
		delete p_enemyAttribute;
		p_enemyAttribute = nullptr;

		// ��Ҫ���³�ʼ��������������ֹ��������Ե��л�
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
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR001NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - 16, 0, m_textureManager->p_EnemyNameWidthFloor001, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// ���˵�Ѫ����
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor001 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor002(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR002NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - 16, 0, m_textureManager->p_EnemyNameWidthFloor002, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// ���˵�Ѫ����
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor002 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor003(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR003NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - 16, 0, m_textureManager->p_EnemyNameWidthFloor003, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// ���˵�Ѫ����
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor003 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor004(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR004NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - 16, 0, m_textureManager->p_EnemyNameWidthFloor004, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// ���˵�Ѫ����
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor004 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor005(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR005NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - 16, 0, m_textureManager->p_EnemyNameWidthFloor005, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);

	// ���˵�Ѫ����
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor005 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);
}

void HtScreen::showEnemyName_healty_status_Floor006(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR006NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - 16, 0, m_textureManager->p_EnemyNameWidthFloor006, TTF_PLAYER_NAME_HEIGHT);
	
	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect); 
	// ���˵�Ѫ�������޿�
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����Ϊ��ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);

	// ���������򣬶�̬�仯
	m_enemyCurrentStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemyStatus - 20, 32, p_enemyAttribute->p_enemyStatus, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentStatusRect);
	// �������������޿�
	m_enemyAllStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor006 - p_enemyAttribute->p_enemeStatusLimit - 20, 32, p_enemyAttribute->p_enemeStatusLimit, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����������Ϊ��ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllStatusRect);
}

void HtScreen::showEnemyName_healty_status_Floor007(TextureManager* m_textureManager, SDL_Renderer* m_gRender, json p_settingJson)
{
	// ���˵����� -16 ������16
	m_textureManager->render(ENEMY_FLOOR007NAME_ID, m_gRender, p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - 16, 0, m_textureManager->p_EnemyNameWidthFloor007, TTF_PLAYER_NAME_HEIGHT);

	// ���˵�Ѫ���� -20 ������16 �� 4���ص����ֺͿ�֮��ľ��룬�����ŵ���Ѫ���ļ��ٶ�����
	m_enemyCurrentHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealth - 20, 5, p_enemyAttribute->p_enemyHealth, 25 };
	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentHealthRect);
	// ���˵�Ѫ�������޿�
	m_enemyAllHealthRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyHealthLimit - 20 - 1, 5 - 1, p_enemyAttribute->p_enemyHealthLimit + 2, 25 + 2 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����Ѫ����Ϊ��ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllHealthRect);

	// ���������򣬶�̬�仯
	m_enemyCurrentStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemyStatus - 20, 32, p_enemyAttribute->p_enemyStatus, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_enemyCurrentStatusRect);
	// �������������޿�
	m_enemyAllStatusRect = { p_settingJson["SetWindowWidth"] - m_textureManager->p_EnemyNameWidthFloor007 - p_enemyAttribute->p_enemeStatusLimit - 20, 32, p_enemyAttribute->p_enemeStatusLimit, 25 };
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); //����������Ϊ��ɫ
	SDL_RenderDrawRect(m_gRender, &m_enemyAllStatusRect);
}