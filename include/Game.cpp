#include"Game.h"


Game::Game()
{
	p_gameIsRunning = false;
	m_gWindow = nullptr;
	m_gRender = nullptr;
	p_gMusic = nullptr;

	//初始化场景切换bool值
	screenRenderArray[STARTRENDER] = true;
	screenRenderArray[MAINRENDERHT] = false;
	screenRenderArray[MAINRENDER] = false;
	screenRenderArray[HTRENDER] = false;

	m_textureManager = new TextureManager();
	m_writeData = new WriteData();

	//初始化场景对象
	m_startScreen = new StartScreen();
	m_mainScreen = new MainScreen();
	m_htScreen = new HtScreen();

	//读josn文件
	//游戏设置文件，这里初始化p_settingJson 和 p_settingLanguage
	m_startScreen->initLanguage(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);

}

Game::~Game()
{
	clean();
}

void Game::clean()
{
	SDL_DestroyRenderer(m_gRender);
	SDL_DestroyWindow(m_gWindow);

	delete m_textureManager;
	delete m_writeData;

	delete m_startScreen;
	delete m_mainScreen;
	delete m_htScreen;

}

bool Game::initWindow(const char* title, int xPost, int yPost, int width, int height, bool isFullWindow)
{
	//初始化窗口, true全屏，false窗口
	if (!isFullWindow) m_gWindow = SDL_CreateWindow(title, xPost, yPost, width, height, SDL_WINDOW_SHOWN);
	else 
	{ 
		m_gWindow = SDL_CreateWindow(title, xPost, yPost, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
		//SDL_SetWindowFullscreen(m_gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	
	if (m_gWindow == nullptr) { std::cout << "m_gWindow error\n"; return false; }

	//初始化renderer
	m_gRender = SDL_CreateRenderer(m_gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_gRender == nullptr) { std::cout << "m_gRender error\n"; return false; }

	return true;

}

bool Game::init()
{
	//初始化SDL
	if (SDL_Init(SDL_INIT_EVENTS) != 0) { return false; }

	//初始化字体
	if (TTF_Init() == -1){ return false; }
	// 初始化m_gFont
	if (!m_textureManager->initFont(TTF_FONT01_PAHT, TTF_FONT_SIZE16)) { return false; }

	// init steam api
	if (!m_mainScreen->initSteamApi()) { return false; }
	

	// 初始化音效
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1){ return false; }
	// 读取mp3
	p_gMusic = Mix_LoadMUS(MP3_MAIN_PATH);
	if (p_gMusic == nullptr) { return false; }

	// init imgui
	// Setup Dear ImGui context	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imgui init font01
	ImFontConfig fontConfig; 
	io.Fonts->AddFontFromFileTTF(TTF_FONT01_PAHT, float(TTF_FONT_SIZE16), &fontConfig, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(m_gWindow, m_gRender);
	ImGui_ImplSDLRenderer2_Init(m_gRender);

	p_gameIsRunning = true;
	return true;
}

void Game::handleEvents()
{
	SteamAPI_RunCallbacks();
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT) { p_gameIsRunning = false; } // 关闭窗口

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// 主场景事件
		if (screenRenderArray[MAINRENDERHT])
		{
			// HT图标鼠标移动高亮
			//if ((mouseX > PNG_HT_BUTTON_X and mouseX < p_settingJson["SetWindowWidth"] - 256) and (mouseY > PNG_HT_BUTTON_Y and mouseY < p_settingJson["SetWindowHeight"] - 256))
			if ((mouseX > p_settingJson["SetWindowWidth"] * 0.25 and mouseX < p_settingJson["SetWindowWidth"] * 0.75 ) and (mouseY > p_settingJson["SetWindowHeight"] * 0.24 and mouseY < p_settingJson["SetWindowHeight"] * 0.94))
			{
				// 鼠标点击
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					screenRenderArray[MAINRENDERHT] = false;
					screenRenderArray[HTRENDER] = true;
				}
				// 设置HT按钮高亮
				m_textureManager->setAlpha(PNT_HT_BUTTON_ID, 200);
			}
			else
			{
				// 鼠标移除后还原高亮状态
				m_textureManager->setAlpha(PNT_HT_BUTTON_ID, 255);
			}

			// 隐藏云朵的事件
			m_mainScreen->mainScreenEvent(mouseX, mouseY, event, p_settingJson);
		}

		// 开始场景事件
		if (screenRenderArray[STARTRENDER])
		{
			// 初始化战斗场景敌人位置
			if (m_isEnemyMoveX)
			{
				m_htScreen->p_enemyMoveX = p_settingJson["SetWindowWidth"] - 128;
				m_isEnemyMoveX = false;
			}

			// 初始化云朵的位置
			if (m_isStartScreenCloudePostion)
			{
				m_startScreen->pngCloud01_x = int(p_settingJson["SetWindowWidth"] * 0.03);
				m_startScreen->pngCloud02_x = int(p_settingJson["SetWindowWidth"] * 0.42);
				m_startScreen->pngCloud03_x = int(p_settingJson["SetWindowWidth"] * 0.71);
				m_isStartScreenCloudePostion = false;
			}

			// 全屏按钮的鼠标移动范围
			if (((mouseX > p_settingJson["SetWindowWidth"] - 64) and (mouseX < p_settingJson["SetWindowWidth"] - 32)) and (mouseY > 32) and (mouseY < 64))
			{
				// Full is True, Window is False
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					// 如果是全屏，置为窗口
					if (p_settingJson["isFullWindow"])
					{

						SDL_SetWindowFullscreen(m_gWindow, 0);
						SDL_SetWindowSize(m_gWindow, 1024, 768); // 如果在全屏状态下关闭，是拿不到1024*768缓存的值的
						SDL_SetWindowPosition(m_gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // 如果在全屏状态下关闭，也无法获取到屏幕居中的状态
						SDL_GetWindowSize(m_gWindow, &m_width, &m_height);

						// 写数据
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "isFullWindow", false, true);
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "SetWindowWidth", m_width);
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "SetWindowHeight", m_height);

						// 写完setting文件后，要重新读取
						std::ifstream gameSetting(FILE_GAME_SETTING_PATH, std::ios::binary);
						p_settingJson = json::from_bson(gameSetting);
						gameSetting.close();

						// 初始化战斗场景敌人的X轴坐标
						m_htScreen->p_enemyMoveX = p_settingJson["SetWindowWidth"] - 128;

						// 重新加载动画效果
						m_startScreen->initLanguage(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);

					}
					else
					{
						SDL_SetWindowFullscreen(m_gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
						SDL_GetWindowSize(m_gWindow, &m_width, &m_height);

						// 写数据
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "isFullWindow", true, true);
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "SetWindowWidth", m_width);
						m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "SetWindowHeight", m_height);

						// 写完setting文件后，要重新读取
						std::ifstream gameSetting(FILE_GAME_SETTING_PATH, std::ios::binary);
						p_settingJson = json::from_bson(gameSetting);
						gameSetting.close();

						// 初始化战斗场景敌人的X轴坐标
						m_htScreen->p_enemyMoveX = p_settingJson["SetWindowWidth"] - 128;

						// 重新加载动画效果
						m_startScreen->initLanguage(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);

					}
				}
				else 
				{
					// 做动画效果
					m_startScreen->updateEvent_big(p_settingJson);
				}
				
			}
			else // 全屏按钮缩小返回
			{
				m_startScreen->updateEvent_small(p_settingJson);
			}
		}
	}
}

void Game::update()
{
	m_mainScreen->setSteamAchievements();

	if (screenRenderArray[STARTRENDER])
	{
		m_startScreen->update(p_settingJson);
	}

	if (screenRenderArray[MAINRENDERHT])
	{
		if (m_isCloudInitXY)
		{
			m_mainScreen->p_imgCloudX = p_settingJson["SetWindowWidth"] / 2 + 120;
			m_mainScreen->p_imgCloudY = 72;
			m_isCloudInitXY = false;
		}

	}

	m_htScreen->animationUpdate(m_gRender, m_textureManager, p_settingJson); // 函数里面已经进行判断
	m_mainScreen->homeButtonUpdate();  // 函数里面已经进行判断
	m_mainScreen->schoolButtonUpdate(p_settingJson);
	m_mainScreen->shopButtonUpdate(p_settingJson);
	
}

void Game::render()
{
	// 开始场景Render
	if (screenRenderArray[STARTRENDER])
	{
		int radioButtonChose = p_settingJson["SetLanguage"];
		m_startScreen->render(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, radioButtonChose, p_gameIsRunning, screenRenderArray[STARTRENDER], screenRenderArray[MAINRENDER], screenRenderArray[MAINRENDERHT], m_htScreen);
		if (radioButtonChose != p_settingJson["SetLanguage"])
		{
			 m_startScreen->initLanguage(m_gRender, m_textureManager, p_settingJson, p_settingLanguage);
		}
	}

	// 主场景HT
	if (screenRenderArray[MAINRENDERHT])
	{
		m_mainScreen->render(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, screenRenderArray[STARTRENDER], screenRenderArray[MAINRENDER], screenRenderArray[MAINRENDERHT], screenRenderArray[HTRENDER]);
	}

	// 主场景
	if (screenRenderArray[MAINRENDER])
	{
		m_mainScreen->render(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, screenRenderArray[STARTRENDER], screenRenderArray[MAINRENDER], screenRenderArray[MAINRENDERHT], screenRenderArray[HTRENDER]);
	}

	// HT战斗场景
	if (screenRenderArray[HTRENDER])
	{
		m_htScreen->render(m_gRender, m_textureManager, p_settingJson, p_settingLanguage, screenRenderArray[MAINRENDERHT], screenRenderArray[HTRENDER]);
	}
}

bool Game::loadGame()
{
	// 实现资源加载的场景 , 后面如果启动慢在做
	//m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_PATH, PNG_BACKGROUND_ID, m_gRender));
	//SDL_RenderClear(m_gRender);
	//m_textureManager->render(PNG_BACKGROUND_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
	//SDL_RenderPresent(m_gRender);
	
	// 加载图片
	// 主场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_PATH, PNG_BACKGROUND_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_CLOUD_PATH, PNG_BACKGROUND_CLOUD_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_HT_PATH, PNG_BACKGROUND_HT_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_ANIMATION_PERSON_PATH, PNG_ANIMATION_PERSON_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_HT_BUTTON_PATH, PNT_HT_BUTTON_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_PERSON_GOLD_PATH, PNG_PERSON_GOLD_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_GAME_TIME_PATH, PNG_GAME_TIME_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_FULLWINDOW_PATH, PNG_FULLWINDOW_ID, m_gRender));
	// 读取字体
	m_isLoadSuccess.push_back(m_textureManager->loadTTF(TTF_GAMEPURPOSE_ID, removeQuotes(p_settingLanguage["GamePurpose"].dump()).c_str(), m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadTTF(TTF_GAMEPURPOSE_INFOR_ID, removeQuotes(p_settingLanguage["GamePurposeInfor"].dump()).c_str(), m_gRender));
	
	// 战斗场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_HT_ATTACK_PATH, PNG_BACKGROUND_HT_ATTACK_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ANIMATIONHT_ATTACK_PATH, ANIMATIONHT_ATTACK_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_GAMEWIN_PATH, PNG_GAMEWIND_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_GAMELOSE_PATH, PNG_GAMELOSE_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ANIMATIONHT_ENEMY_FLOOR001_PATH, ANIMATIONHT_ENEMY_FLOOR001_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR002_PATH, ENEMY_FLOOR002_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR003_PATH, ENEMY_FLOOR003_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR004_PATH, ENEMY_FLOOR004_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR005_PATH, ENEMY_FLOOR005_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR006_PATH, ENEMY_FLOOR006_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ENEMY_FLOOR007_PATH, ENEMY_FLOOR007_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ANIMATION_CHESTS_PATH, ANIMATION_CHESTS_ID, m_gRender));  //chests

	// 家园场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_HOME_PATH, PNG_BACKGROUND_HOME_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_HOME_SLEEP_PATH, PNG_BACKGROUND_HOME_SLEEP_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_HOME_SLEEPZ_PATH, PNG_BACKGROUND_HOME_SLEEPZ_ID, m_gRender));

	// 学院场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_SCHOOL_PATH, PNG_BACKGROUND_SCHOOL_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(ANIMATION_PERSON_SCHOOL_PATH, ANIMATION_PERSON_SCHOOL_ID, m_gRender));

	// 商店场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_SHOP_PATH, PNG_BACKGROUND_SHOP_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BACKGROUND_SHOPWORK_PATH, PNG_BACKGROUND_SHOPWORK_ID, m_gRender));

	// 成就场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_ACHIEVEMENTS_STAR_PATH, PNG_ACHIEVEMENTS_STAR_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_ACHIEVEMENTS_NOSTAR_PATH, PNG_ACHIEVEMENTS_NOSTAR_ID, m_gRender));

	// 日志场景用
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_LOG_ENEMY_FLOOR001_PAHT, PNG_LOG_ENEMY_FLOOR001_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_LOG_ENEMY_FLOOR006_PAHT, PNG_LOG_ENEMY_FLOOR006_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_LOG_ENEMY_FLOOR007_PAHT, PNG_LOG_ENEMY_FLOOR007_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_LOG_CHESTS_PATH, PNG_LOG_CHESTS_ID, m_gRender));  // 7 floors chests

	// 装备图片
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BEGINEERSWORD_PATH, PNG_BEGINEERSWORD_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BEGINNERHELMET_PATH, PNG_BEGINNERHELMET_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BEGINNERBARCERS_PATH, PNG_BEGINNERBARCERS_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BEGINNERBREASTPLATE_PATH, PNG_BEGINNERBREASTPLATE_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_BEGINNERKNEEPAD_PATH, PNG_BEGINNERKNEEPAD_ID, m_gRender));

	// 技能图片
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_SKILL_DEFENSE_PATH, PNG_SKILL_DEFENSE_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_SKILL_HEAVYATTACK_PATH, PNG_SKILL_HEAVYATTACK_ID, m_gRender));

	// 药水图片
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_POTION_SMALLHEALTHPOTION_PATH, PNG_POTION_SMALLHEALTHPOTION_ID, m_gRender));
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_POTION_SMALLSTATUSPOTION_PATH, PNG_POTION_SMALLSTATUSPOTION_ID, m_gRender));

	// Props
	m_isLoadSuccess.push_back(m_textureManager->loadPngImg(PNG_PRIMARYSTRENGTHENINGSTONE_PATH, PNG_PRIMARYSTRENGTHENINGSTONE_ID, m_gRender));

	for (std::vector<bool>::iterator it = m_isLoadSuccess.begin(); it != m_isLoadSuccess.end(); ++it) {
		if (!*it) { return false; }
	}

	return true;
}