#include"MainScreen.h"

MainScreen::MainScreen()
{
	// ��ʼ���������ڶ���
	m_personButton = new PersonButton();
	m_homeButton = new HomeButton();
	m_schoolButton = new SchoolButton();
	m_shopButton = new ShopButton();
	m_harvestButton = new HarvestButton();
	m_logButton = new LogButton();
	m_writeData = new WriteData();
	

	for (int i = 0; i < MENUSTATUSTOTAL; i++)
	{
		mainMenuStatusArray[i] = false;
	}
}

MainScreen::~MainScreen()
{
	delete m_personButton;
	delete m_homeButton;
	delete m_schoolButton;
	delete m_shopButton;
	delete m_harvestButton;
	delete m_logButton;
	delete m_writeData;	
	
	SteamAPI_Shutdown();
	if (m_streamAPIAchievements) { delete m_streamAPIAchievements; }
}

bool MainScreen::render(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, bool& isStartScreen, bool& isMainScreen, bool& isMainHtScreen, bool& isHtScreen)
{
	initGameData(m_gRender, m_textureManager, p_settingLanguage); // ÿ��render��Ҫ�����ݽ��г�ʼ��������֤���ݵ�ʵʱ����

	//ImGuiÿ��ѭ������һ���µ�frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// �·����˵�
	ImGui::SetNextWindowSize(ImVec2(WINDOWSIZE_MENU_WIDTH, WINDOWSIZE_MENU_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(32.0f, float(p_settingJson["SetWindowHeight"] - WINDOWSIZE_MENU_HEIGHT)));
	ImGui::Begin("menu", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["PersonButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[PERSONMENUSTATUS] = true;
	}
	ImGui::SameLine();

	if(ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["HomeButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[HOMEMENUSTATUS] = true;
	}
	ImGui::SameLine();

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["SchoolButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)) )
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[SCHOOLMENUSTATUS] = true;

	}
	ImGui::SameLine();

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["ShopButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[SHOPMENUSTATUS] = true;
	}
	ImGui::SameLine();

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["HarvestButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[HARVESTMENUSTATUS] = true;
	}
	ImGui::SameLine();

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["LogButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[LOGMENUSTATUS] = true;
	}
	ImGui::SameLine();

	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["AchievementsButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}
		mainMenuStatusArray[ACHIEVEMENTSMENUSTATUS] = true;
	}
	ImGui::SameLine();

	// ���exit��ť��һ��Ҫreturn����������������ִ�У��Ӷ�Ӱ�쳡����true �� false��ֵ
	if (ImGui::Button(removeQuotes(p_settingLanguage["MainMeum"]["ExitButton"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		// �رմ��ڶ���
		for (int i = 0; i < MENUSTATUSTOTAL; i++)
		{
			mainMenuStatusArray[i] = false;
		}

		isStartScreen = true;
		isMainHtScreen = false;
		isMainScreen = false;
		isHtScreen = false;
		ImGui::End();
		ImGui::Render();
		return true;
	}

	ImGui::End(); 

	// ����Ѫ����״̬����tooltip
	setHealthAndStatusToolTip(m_textureManager, p_settingJson, p_settingLanguage);
	setGoldAndTimeToolTip(p_settingJson, p_settingLanguage);

	// �������������Ⱦ
	if (mainMenuStatusArray[PERSONMENUSTATUS])
	{
		// ��ʼ�������������
		m_personButton->initPersonData(p_save01Data);
		// ��ʾ������尴ť
		m_personButton->personButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[PERSONMENUSTATUS], m_textureManager);
		isMainHtScreen = false;
		isMainScreen = true;
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// ��԰�����Ⱦ
	if (mainMenuStatusArray[HOMEMENUSTATUS])
	{
		isMainHtScreen = false;
		isMainScreen = true;
		m_homeButton->homeButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[HOMEMENUSTATUS], m_textureManager);
		if (m_homeButton->p_isReturn)
		{
			returnMainHtScreen(isMainScreen, isMainHtScreen);
		}
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// ѧԺ�����Ⱦ
	if (mainMenuStatusArray[SCHOOLMENUSTATUS])
	{
		isMainHtScreen = false;
		isMainScreen = true;
		m_schoolButton->schoolButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[SCHOOLMENUSTATUS], m_textureManager);
		if (m_schoolButton->p_isReturn)
		{
			returnMainHtScreen(isMainScreen, isMainHtScreen);
		}
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// �̵������Ⱦ
	if (mainMenuStatusArray[SHOPMENUSTATUS])
	{
		isMainHtScreen = false;
		isMainScreen = true;
		m_shopButton->shopButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[SHOPMENUSTATUS], m_textureManager);
		if (m_shopButton->p_isReturn)
		{
			returnMainHtScreen(isMainScreen, isMainHtScreen);
		}
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// �ɼ������Ⱦ
	if (mainMenuStatusArray[HARVESTMENUSTATUS])
	{
		m_harvestButton->harvestButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[HARVESTMENUSTATUS]);
		isMainHtScreen = false;
		isMainScreen = true;
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// ��־�����Ⱦ
	if (mainMenuStatusArray[LOGMENUSTATUS]) 
	{
		//m_logButton->initLogData(p_save01Data);
		m_logButton->logButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[LOGMENUSTATUS], m_textureManager);
		isMainHtScreen = false;
		isMainScreen = true;
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// �ɾ������Ⱦ
	if (mainMenuStatusArray[ACHIEVEMENTSMENUSTATUS])
	{
		m_achievementsButton->achievementsButtonFrame(p_settingJson, p_settingLanguage, p_save01Data, mainMenuStatusArray[ACHIEVEMENTSMENUSTATUS], m_textureManager);
		isMainHtScreen = false;
		isMainScreen = true;
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	// ����ʱ�����ؽ���
	if (m_isCloudClicked)
	{
		isMainHtScreen = false;
		isMainScreen = true;
		// ����ƶ䰴ť��ĵ���
		ImGui::SetNextWindowSize(ImVec2(AWARD_USUAL_FRAME_WIDTH, AWARD_USUAL_FRAME_HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(float((p_settingJson["SetWindowWidth"] / 2) - (AWARD_USUAL_FRAME_WIDTH / 2)), float((p_settingJson["SetWindowHeight"] / 2) - (AWARD_USUAL_FRAME_HEIGHT / 2))));
		ImGui::Begin("CloudClicked", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), removeQuotes(p_settingLanguage["HiddenReward"]["HiddenReward"].dump()).c_str());

		ImGui::Text(removeQuotes(p_settingLanguage["HiddenReward"]["Floor3Text"].dump()).c_str());
		ImGui::Text("");
		if (ImGui::Button(removeQuotes(p_settingLanguage["HiddenReward"]["Take"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
		{
			m_isCloudClicked = false;
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "personMoney", p_save01Data["personMoney"] + 20);

			// ���ƶ���ʧ�������ظ���ȡ����
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isGetFloorThreeAward", true, true);
		}
		ImGui::SameLine();
		if (ImGui::Button(removeQuotes(p_settingLanguage["HiddenReward"]["Drop"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL96_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
		{
			m_isCloudClicked = false;

			// ���ƶ���ʧ�������ظ���ȡ����
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isGetFloorThreeAward", true, true);
		}

		ImGui::End();
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);

	}

	// �����������ر�ʱ������������Ⱦ
	if (!mainMenuStatusArray[PERSONMENUSTATUS] and !mainMenuStatusArray[HOMEMENUSTATUS] and !mainMenuStatusArray[SCHOOLMENUSTATUS]
		and !mainMenuStatusArray[SHOPMENUSTATUS] and !mainMenuStatusArray[HARVESTMENUSTATUS] and !mainMenuStatusArray[LOGMENUSTATUS] and !mainMenuStatusArray[ACHIEVEMENTSMENUSTATUS]
		and !m_isCloudClicked)
	{
		isMainHtScreen = true;
		isMainScreen = false;
		allButtoRender(m_gRender, m_textureManager, p_settingJson, isMainHtScreen);
	}

	return true;
}

void MainScreen::allButtoRender(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, bool isMainHtScreen)
{
	ImGui::Render();
	SDL_RenderClear(m_gRender);

	// ��԰����ͼƬ��Ⱦ
	if (mainMenuStatusArray[HOMEMENUSTATUS])
	{
		m_textureManager->render(PNG_BACKGROUND_HOME_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
		if (m_homeButton->p_isSleepAnimation)
		{
			m_textureManager->render(PNG_BACKGROUND_HOME_SLEEP_ID, m_gRender, 0, 256, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"] / 3);
			// ��ȾZ����
			if (m_homeButton->p_isAnimationZ128){ m_textureManager->render(PNG_BACKGROUND_HOME_SLEEPZ_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2, p_settingJson["SetWindowHeight"] / 3 + 128, 128, 128); }
			if (m_homeButton->p_isAnimationZ64){ m_textureManager->render(PNG_BACKGROUND_HOME_SLEEPZ_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2 + 128, p_settingJson["SetWindowHeight"] / 3 + 128 - 64, PNG_USUAL_HEIGHT64, PNG_USUAL_HEIGHT64); }
			if (m_homeButton->p_isAnimationZ32) { m_textureManager->scaleRender(PNG_BACKGROUND_HOME_SLEEPZ_ID, m_gRender, p_settingJson["SetWindowWidth"] / 2 + 128 + 64, p_settingJson["SetWindowHeight"] / 3 + 128 - 64 - 32, PNG_USUAL_HEIGHT64, PNG_USUAL_HEIGHT64, 2, 2); } // ԭͼ������ 64 * 64������scaleRender
		}
	}

	// ѧԺ����ͼƬ��Ⱦ
	if (mainMenuStatusArray[SCHOOLMENUSTATUS])
	{
		m_textureManager->render(PNG_BACKGROUND_SCHOOL_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
		if (m_schoolButton->p_isTrainAnimation)
		{
			m_textureManager->dynamicRender(ANIMATION_PERSON_SCHOOL_ID, m_gRender, m_schoolButton->p_personX, p_settingJson["SetWindowHeight"] / 3 * 2 + 64, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, m_schoolButton->p_personFrame, 1, 2);
		}
	}

	// �̵걳��ͼƬ��Ⱦ
	if (mainMenuStatusArray[SHOPMENUSTATUS])
	{
		m_textureManager->render(PNG_BACKGROUND_SHOP_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
		if (m_shopButton->p_isShopAnimation)
		{
			// �򹤶���Ч����Ⱦ
			m_textureManager->render(PNG_BACKGROUND_SHOPWORK_ID, m_gRender, m_shopButton->p_shopAnimationX, m_shopButton->p_shopAnimationY, PNG_USUAL_WIDTH32, PNG_USUAL_HEIGHT32); // 1
		}
	}

	// ���а�ť���ر�ʱ����ʾ����ͼƬ
	if (!mainMenuStatusArray[HOMEMENUSTATUS] and !mainMenuStatusArray[SCHOOLMENUSTATUS] and !mainMenuStatusArray[SHOPMENUSTATUS])
	{
		m_textureManager->render(PNG_BACKGROUND_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
	}
	// ����ͷ����Ⱦ
	m_textureManager->dynamicRender(PNG_ANIMATION_PERSON_ID, m_gRender, 0, 0, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, 0, 1);
	// ����������Ⱦ
	m_textureManager->render(TTF_PLAYER_NAME_ID, m_gRender, TTF_PLAYER_NAME_X, TTF_PLAYER_NAME_Y, m_textureManager->p_playerNameWidth, TTF_PLAYER_NAME_HEIGHT);
	// ���ͼƬ��Ⱦ
	m_textureManager->render(PNG_PERSON_GOLD_ID, m_gRender, p_settingJson["SetWindowWidth"] - 264, 0, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
	// ���������Ⱦ
	 m_textureManager->render(m_gamePersonGoldTexture, m_gRender, p_settingJson["SetWindowWidth"] - 200, 0, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
	// ʱ��ͼƬ��Ⱦ
	m_textureManager->render(PNG_GAME_TIME_ID, m_gRender, p_settingJson["SetWindowWidth"] - 136, 0, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
	// ʱ��������Ⱦ
	m_textureManager->render(m_gameTimeTexture, m_gRender, p_settingJson["SetWindowWidth"] - 72, 0, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);
	
	// ����Ѫ����Ⱦ
	SDL_SetRenderDrawColor(m_gRender, 0, 0, 0, 255); // ���ú�ɫ���޿�
	SDL_RenderDrawRect(m_gRender, &m_personAllHealthRect);  // Ѫ�����޿� ��δ���ƣ��ݶ�500����
	SDL_RenderDrawRect(m_gRender, &m_personAllStatusRect);  // ״̬���޿� ��δ���ƣ��ݶ�500����

	SDL_SetRenderDrawColor(m_gRender, 255, 0, 0, 255); //����Ѫ����ɫ
	SDL_RenderFillRect(m_gRender, &m_personCurrentHealthRect);

	SDL_SetRenderDrawColor(m_gRender, 0, 0, 255, 255); //����״̬����ɫ
	SDL_RenderFillRect(m_gRender, &m_personCurrentStatusRect);

	// ��SDL2 Render��ȾHT��ť
	if (isMainHtScreen)
	{
		//m_textureManager->render(PNT_HT_BUTTON_ID, m_gRender, PNG_HT_BUTTON_X, PNG_HT_BUTTON_Y, p_settingJson["SetWindowWidth"] - 512, p_settingJson["SetWindowHeight"] - 256);
		m_textureManager->render(PNT_HT_BUTTON_ID, m_gRender, int(p_settingJson["SetWindowWidth"] * 0.25), int(p_settingJson["SetWindowHeight"] * 0.24), int(p_settingJson["SetWindowWidth"] * 0.5), int(p_settingJson["SetWindowHeight"] * 0.7));

		m_gameHtFloorTexture = m_textureManager->loadTTF(p_save01Data["HtFloor"].dump().c_str(), m_gRender);  // ��ȡHT����
		m_textureManager->render(TTF_GAME_FLOOR_ID, m_gRender, int(p_settingJson["SetWindowWidth"] * 0.06), int(p_settingJson["SetWindowHeight"] * 0.41), int(p_settingJson["SetWindowWidth"] * 0.06), int(p_settingJson["SetWindowHeight"] * 0.08));
		m_textureManager->render(m_gameHtFloorTexture, m_gRender, int(p_settingJson["SetWindowWidth"] * 0.12), int(p_settingJson["SetWindowHeight"] * 0.41), int(p_settingJson["SetWindowWidth"] * 0.06), int(p_settingJson["SetWindowHeight"] * 0.08));

		// ������ʱ����Ⱦһ���ƣ��������ȡ���20
		if (p_save01Data["HtFloor"] == 3 and !p_save01Data["isGetFloorThreeAward"])
		{
			m_textureManager->render(PNG_BACKGROUND_CLOUD_ID, m_gRender, p_imgCloudX, p_imgCloudY, m_imgCloudWidth, m_imgCloudHeight);
		}

	}

	// ImGui��Ⱦ
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_gRender);
}

void MainScreen::initGameData(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingLanguage)
{

	//ÿ��render��ʱ�򣬶�Ҫ��ȥ����������ļ���������ʵʱ�����ݸ��£��γɾֲ�����
	std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
	p_save01Data = json::from_bson(gameSaving);
	gameSaving.close();

	//��ʼ��Ѫ����״̬�����KPI
	m_personCurrentHealth = p_save01Data["personCurrentHealth"];
	m_personAllHealth = p_save01Data["personAllHealth"];
	m_personCurrentStatus = p_save01Data["personCurrentStatus"];
	m_personAllStatus = p_save01Data["personAllStatus"];

	m_personAllHealthRect = { TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10 - 1, 5 - 1, m_personAllHealth + 2, 25 + 2};
	m_personCurrentHealthRect = { TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10, 5, m_personCurrentHealth, 25 };
	m_personAllStatusRect = { TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10 - 1, 38 - 1, m_personAllStatus + 2, 25 + 2 };
	m_personCurrentStatusRect = { TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth + 10, 38, m_personCurrentStatus, 25};

	// ���¶�ȡ���� ���⣺TTF_FONT01_PAHT����ͣ�����ڴ����ڼ�,û���ͷŵ�
	m_gameTimeTexture = m_textureManager->loadTTF(p_save01Data["gameTime"].dump().c_str(), m_gRender);  // ��ȡ��Ϸʱ��
	m_gamePersonGoldTexture = m_textureManager->loadTTF(p_save01Data["personMoney"].dump().c_str(), m_gRender); // ��ȡ��Ϸ���
	//m_gameHtFloorTexture =  m_textureManager->loadTTF(p_save01Data["HtFloor"].dump().c_str(), m_gRender);  // ��ȡHT����
}

void MainScreen::setHealthAndStatusToolTip(TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage)
{
	// ��������ƶ���Ѫ����״̬����ʾ��ֵ - toolTip  , + 48 ��Ϊ�˷�ֹ�������ĳ���
	ImGui::SetNextWindowSize(ImVec2(512, PNG_USUAL_HEIGHT64 + 48));
	ImGui::SetNextWindowPos(ImVec2(float(TTF_PLAYER_NAME_X + m_textureManager->p_playerNameWidth), 0.0f));
	ImGui::Begin("healthAndStatusTooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	// ��������personAllHealth������personAllStatus����ʾinvisibleButton�Ŀ��
	if (p_save01Data["personAllHealth"] >= p_save01Data["personAllStatus"])
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(p_save01Data["personAllHealth"], float(p_settingJson["SetWindowHeight"] * 0.08)));
	}
	else
	{
		ImGui::InvisibleButton("showHealthAndStatusTooltip", ImVec2(p_save01Data["personAllStatus"], float(p_settingJson["SetWindowHeight"] * 0.08)));
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

void MainScreen::setGoldAndTimeToolTip(json p_settingJson, json p_settingLanguage)
{
	ImGui::SetNextWindowSize(ImVec2(128, PNG_USUAL_HEIGHT64));
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - 264), 0.0f));
	ImGui::Begin("GoldTooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::InvisibleButton("showGoldTooltip", ImVec2(128, PNG_USUAL_HEIGHT64 - 20));
	if (ImGui::IsItemHovered())
	{
		if (ImGui::BeginTooltip())
		{
			std::string goldToolTip = removeQuotes(p_settingLanguage["MainMeum"]["Tooltip"]["Gold"].dump()) + p_save01Data["personMoney"].dump();
			ImGui::Text(goldToolTip.c_str());
			ImGui::EndTooltip();
		}
	}

	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(128, PNG_USUAL_HEIGHT64));
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - 136), 0.0f));
	ImGui::Begin("TimeTooltip", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::InvisibleButton("showTimeTooltip", ImVec2(128, PNG_USUAL_HEIGHT64 - 20));
	if (ImGui::IsItemHovered())
	{
		if (ImGui::BeginTooltip())
		{
			std::string timeToolTip = removeQuotes(p_settingLanguage["MainMeum"]["Tooltip"]["Time"].dump()) + p_save01Data["gameTime"].dump();
			ImGui::Text(timeToolTip.c_str());
			ImGui::EndTooltip();
		}
	}

	ImGui::End();


}

void MainScreen::returnMainHtScreen(bool& isMainScreen, bool& isMainHtScreen)
{
	for (int i = 0; i < MENUSTATUSTOTAL; i++)
	{
		mainMenuStatusArray[i] = false;
	}
	isMainHtScreen = true;
	isMainScreen = false;
}

void MainScreen::homeButtonUpdate()
{
	if (mainMenuStatusArray[HOMEMENUSTATUS] and m_homeButton->p_isSleepAnimation)
	{
		m_homeButton->p_animationZCount += 1;
		if (m_homeButton->p_animationZCount == 20) { m_homeButton->p_isAnimationZ128 = true; }
		if (m_homeButton->p_animationZCount == 40) { m_homeButton->p_isAnimationZ64 = true; }
		if (m_homeButton->p_animationZCount == 60) { m_homeButton->p_isAnimationZ32 = true; }
		if (m_homeButton->p_animationZCount == 80) 
		{
			m_homeButton->p_animationZLoopNum += 1;
			m_homeButton->p_animationZCount = 0;

			// ������ԭ
			m_homeButton->p_isAnimationZ128 = false;
			m_homeButton->p_isAnimationZ64 = false;
			m_homeButton->p_isAnimationZ32 = false;
			if (m_homeButton->p_animationZLoopNum == 2)
			{
				m_homeButton->p_isSleepAnimation = false;
				m_homeButton->p_animationZLoopNum = 0;

				// ������ԭ
				m_homeButton->p_isAnimationZ128 = false;
				m_homeButton->p_isAnimationZ64 = false;
				m_homeButton->p_isAnimationZ32 = false;
			}
		}

	}
}

void MainScreen::schoolButtonUpdate(json p_settingJson)
{
	if (mainMenuStatusArray[SCHOOLMENUSTATUS] and m_schoolButton->p_isTrainAnimation)
	{
		// �����ܶ��Ķ���Ч������
		m_schoolButton->p_personX += m_schoolButton->p_personXSpeed;
		if (m_schoolButton->p_personFrame == 0)
		{
			m_schoolButton->p_personFrame = 1;
		}
		else
		{
			m_schoolButton->p_personFrame = 0;
		}
		if (m_schoolButton->p_personX >= p_settingJson["SetWindowWidth"])
		{
			m_schoolButton->p_isTrainAnimation = false;
			m_schoolButton->p_personX = 0;
		}
	}
}

void MainScreen::shopButtonUpdate(json p_settingJson)
{
	// �򹤵Ķ���Ч��~
	if (mainMenuStatusArray[SHOPMENUSTATUS] and m_shopButton->p_isShopAnimation)
	{
		//m_shopButton->p_shopAnimationY = p_settingJson["SetWindowHeight"] / 2 - 84;
		// �����˶�
		if (m_shopButton->p_shopAnimationX < p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8 + int(p_settingJson["SetWindowWidth"] / 5) and m_shopButton->p_shopAnimationY == p_settingJson["SetWindowHeight"] / 2 - 84)
		{
			m_shopButton->p_shopAnimationX += m_shopButton->p_shopAnimationSpeed;
		}
		// �ƶ�������
		if (m_shopButton->p_shopAnimationX >= p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8 + int(p_settingJson["SetWindowWidth"] / 5) and m_shopButton->p_shopAnimationY == p_settingJson["SetWindowHeight"] / 2 - 84)
		{
			m_shopButton->p_shopAnimationX = p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8;
			m_shopButton->p_shopAnimationY = p_settingJson["SetWindowHeight"] / 2 - 84 + 100;
		}
		// �����˶�
		if (m_shopButton->p_shopAnimationX < p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8 + int(p_settingJson["SetWindowWidth"] / 5) and m_shopButton->p_shopAnimationY == p_settingJson["SetWindowHeight"] / 2 - 84 + 100)
		{
			m_shopButton->p_shopAnimationX += m_shopButton->p_shopAnimationSpeed;
		}
		// �ƶ������沢��������
		if (m_shopButton->p_shopAnimationX >= p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8 + int(p_settingJson["SetWindowWidth"] / 5) and m_shopButton->p_shopAnimationY == p_settingJson["SetWindowHeight"] / 2 - 84 + 100)
		{
			m_shopButton->p_shopAnimationX = p_settingJson["SetWindowWidth"] / 4 + p_settingJson["SetWindowWidth"] / 8;
			m_shopButton->p_shopAnimationY = p_settingJson["SetWindowHeight"] / 2 - 84;
			m_shopButton->p_isShopAnimation = false;
		}
		
	}
}

void MainScreen::mainScreenEvent(int mouseX, int mouseY, SDL_Event event, json p_settingJson)
{
	// �����ƶ����¼�
	if (p_save01Data["HtFloor"] == 3 and !p_save01Data["isGetFloorThreeAward"])
	{

		if ( ((mouseX > p_settingJson["SetWindowWidth"] / 2 + 100) and (mouseX < p_settingJson["SetWindowWidth"] / 2 + 100 + 64)) and
			  ((mouseY > 56) and (mouseY < 56 + 64)))
		{
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				// ����������� 
				m_isCloudClicked = true;
			}
			// �ƶ�ı��Ч��
			m_imgCloudWidth = PNG_USUAL_WIDTH64;
			m_imgCloudHeight = PNG_USUAL_HEIGHT64;

			// �ƶ�ı���X��Y������
			p_imgCloudX = p_settingJson["SetWindowWidth"] / 2 + 100;
			p_imgCloudY = 56;

		}
		else
		{
			// �ƶ�Ļ�ԭЧ��
			m_imgCloudWidth = PNG_USUAL_WIDTH32;
			m_imgCloudHeight = PNG_USUAL_HEIGHT32;

			// �ƶ�Ļ�ԭЧ����X��Y������
			p_imgCloudX = p_settingJson["SetWindowWidth"] / 2 + 120;
			p_imgCloudY = 72;
		}
	}
}

bool MainScreen::initSteamApi()
{
	// init steam api
	// k_uAppIdInvalid Replace with your App ID 2920160
	// ������� true�����ڱ���ʱ���� Steam �ͻ���, �������false����ʾû��ͨ��Steam�ͻ�����������steam_appid.txt���ڷ���false
	if (SteamAPI_RestartAppIfNecessary(2920160)) { return false; }  // ���ڲ������ӿͻ������������㱾�ص���
	if (!SteamAPI_Init()) { return false; }
	else {
		Achievement_t g_Achievements[] =
		{
			_ACH_ID(FirstGame, "First_into_Game"),
			//_ACH_ID(OneDay, "OneDay"),
			//_ACH_ID(TwoDays, "TwoDays"),
			//_ACH_ID(FiveDays, "FiveDays"),
			//_ACH_ID(EightDays, "EightDays"),
			//_ACH_ID(NineDays, "NineDays"),
		};
		m_streamAPIAchievements = new StreamAPIAchievements(g_Achievements, 0);
	}
	return true;
}

void MainScreen::setSteamAchievements()
{
	//SteamAPI_RunCallbacks();
	if (m_streamAPIAchievements)
	{
		m_streamAPIAchievements->SetAchievement("FirstGame");

		if (p_save01Data["HtFloor"] == 2 and p_save01Data["gameTime"] == 1)
		{
			m_streamAPIAchievements->SetAchievement("OneDay");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor001", true, true);
		}
		if (p_save01Data["HtFloor"] == 3 and p_save01Data["gameTime"] == 2)
		{
			m_streamAPIAchievements->SetAchievement("TwoDays");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor002", true, true);
		}
		if (p_save01Data["HtFloor"] == 4 and p_save01Data["gameTime"] == 5)
		{
			m_streamAPIAchievements->SetAchievement("FiveDays");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor003", true, true);
		}
		if (p_save01Data["HtFloor"] == 5 and p_save01Data["gameTime"] == 8)
		{
			m_streamAPIAchievements->SetAchievement("EightDays");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor004", true, true);
		}
		if (p_save01Data["HtFloor"] == 6 and p_save01Data["gameTime"] == 9)
		{
			m_streamAPIAchievements->SetAchievement("NineDays");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor005", true, true);
		}
		if (p_save01Data["HtFloor"] == 7 and p_save01Data["gameTime"] == 11)
		{
			m_streamAPIAchievements->SetAchievement("ElevenDays");
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "isAchievementsFloor006", true, true);
		}
	}
}