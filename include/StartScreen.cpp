#include"StartScreen.h"

std::string removeQuotes(const std::string& str);

StartScreen::StartScreen()
{
	m_writeData = new WriteData();

	pngBackgroundHT_x = 0;
	pngBackgroundHT_y = 0;
	pngBackgroundHT_speed = 5;

	pngCloud_speed = 1;

}

StartScreen::~StartScreen()
{
	delete m_writeData;
}

void StartScreen::render(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json p_settingJson, json p_settingLanguage, int& radioButtonChose, bool& p_gameIsRunning, bool& isStartScreen, bool& isMainScreen, bool& isMainHtScreen, HtScreen* m_htScreen)
{
	//ImGuiÿ��ѭ������һ���µ�frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(WINDOWPOS_LANGUAGE_WIDTH, WINDOWPOS_LANGUAGE_HEIGHT)); // ���ô��ڴ�С
	ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - WINDOWPOS_LANGUAGE_WIDTH), float(p_settingJson["SetWindowHeight"] - WINDOWPOS_LANGUAGE_HEIGHT)));

	// �������ð�ť
	ImGui::Begin("TH", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	// radioButtonChose = p_settingJson["SetLanguage"]; // 0 : en  1: ch
	ImGui::RadioButton(" EN", &radioButtonChose, 0); ImGui::SameLine();
	ImGui::RadioButton(u8" ����", &radioButtonChose, 1);
	if (radioButtonChose != p_settingJson["SetLanguage"])
	{
		// ���浽bson
		m_writeData->writeBsonBin(FILE_GAME_SETTING_PATH, "SetLanguage", radioButtonChose);
	}
	ImGui::End();

	// ��ʼ��ť
	ImGui::SetNextWindowSize(ImVec2(WINDOWPOS_STARTGAME_WIDTH, WINDOWPOS_STARTGAME_HEIGHT)); // ���ô��ڴ�С
	ImGui::SetNextWindowPos(ImVec2(float( p_settingJson["SetWindowWidth"]  - WINDOWPOS_STARTGAME_WIDTH ), float(p_settingJson["SetWindowHeight"] - WINDOWPOS_STARTGAME_HEIGHT - WINDOWPOS_LANGUAGE_HEIGHT)));
	ImGui::Begin("TH2", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	// ���������Ϸ�����ļ�����ʾ������ť
	if ( std::filesystem::exists(FILE_GAME_SAVE_PATH) )
	{
		if (ImGui::Button(removeQuotes(p_settingLanguage["Continue"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL128_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
		{
			// ��ʼ��p_save01Data
			std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
			p_save01Data = json::from_bson(gameSaving);
			gameSaving.close();

			// ��ȡ�����Ϣ
			// ���������
			m_textureManager->loadTTF(TTF_PLAYER_NAME_ID, removeQuotes(p_save01Data["PlayerName"].dump()).c_str(), m_gRender, true, {255, 170, 0});
			// ��ʾ���� ��
			m_textureManager->loadTTF(TTF_GAME_FLOOR_ID, removeQuotes(p_settingLanguage["GameFloor"].dump()).c_str(), m_gRender); 
			// �����л�
			isStartScreen = false;
			isMainHtScreen = true;
			isMainScreen = false;
		}

	}
	// ��ʼ��Ϸ
	if (ImGui::Button(removeQuotes(p_settingLanguage["StartGame"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL128_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		// ��ת�����������Ϸ��UI
		m_isEnterUserName = true;
	}
	// �ر���Ϸ
	if (ImGui::Button(removeQuotes(p_settingLanguage["EndGame"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL128_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
	{
		p_gameIsRunning = false;
	}
	ImGui::End();

	// ����������ֵĿ�
	if (m_isEnterUserName)
	{
		ImGui::SetNextWindowSize(ImVec2(WINDOWPOS_USERNAME_WIDTH, WINDOWPOS_USERNAME_HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(float(p_settingJson["SetWindowWidth"] - WINDOWPOS_STARTGAME_WIDTH - WINDOWPOS_USERNAME_WIDTH), float(p_settingJson["SetWindowHeight"] - WINDOWPOS_STARTGAME_HEIGHT - WINDOWPOS_LANGUAGE_HEIGHT)));
		ImGui::Begin("userName", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		// �ı�
		ImGui::Text(removeQuotes(p_settingLanguage["TextPlayerName"].dump()).c_str()); ImGui::SameLine();
		// �ı���
		ImGui::SetNextItemWidth(128.0f);  // �����ı���Ŀ��
		ImGui::InputText(" ", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);

		// ȷ�ϰ�ť
		if (ImGui::Button(removeQuotes(p_settingLanguage["Confirm"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL128_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
		{
			m_writeData->initPersonAttributes(FILE_GAME_SAVE_PATH);
			m_writeData->writeBsonBin(FILE_GAME_SAVE_PATH, "PlayerName", buf); // ���������д��save.bin�ļ���һ��Ҫ��д��󣬲��ܶ������ļ�

			// ��յ�������
			if (m_htScreen->p_enemyAttribute != nullptr)
			{
				m_htScreen->p_enemyAttribute = nullptr;
			}

			// ����Ѵ��ڵļ�¼��
			if (std::filesystem::exists("save/save01_5F.bin"))
			{
				removeFile("save/save01_5F.bin");
			}

			// Ҫ������д��󣬳�ʼ��p_save01Data
			std::ifstream gameSaving(FILE_GAME_SAVE_PATH, std::ios::binary);
			p_save01Data = json::from_bson(gameSaving);
			gameSaving.close();

			// ������������ã���Ҫ���³�ʼ�� p_save01Data
			m_textureManager->loadTTF(TTF_PLAYER_NAME_ID, removeQuotes(p_save01Data["PlayerName"].dump()).c_str(), m_gRender, true, {255, 170, 0});
			// ��ʾ���� ��
			m_textureManager->loadTTF(TTF_GAME_FLOOR_ID, removeQuotes(p_settingLanguage["GameFloor"].dump()).c_str(), m_gRender);
			// ��ȷ�ϰ�ť�󣬳�����ת
			isStartScreen = false;
			isMainHtScreen = true;
			isMainScreen = false;

			m_isEnterUserName = false; // �رոöԻ��򣬷�ֹ����ʱ������
		}ImGui::SameLine();
		// ���ذ�ť
		if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str(), ImVec2(BUTTONSIZE_USUAL128_WIDTH, BUTTONSIZE_USUAL32_HEIGHT)))
		{
			m_isEnterUserName = false;
		}

		ImGui::End();
	}

	ImGui::Render();
	SDL_RenderClear(m_gRender);
	// ����ͼƬ��Ⱦ
	m_textureManager->render(PNG_BACKGROUND_ID, m_gRender, 0, 0, p_settingJson["SetWindowWidth"], p_settingJson["SetWindowHeight"]);
	m_textureManager->render(PNG_BACKGROUND_HT_ID, m_gRender, pngBackgroundHT_x, pngBackgroundHT_y, int(p_settingJson["SetWindowWidth"] * 0.78), int(p_settingJson["SetWindowHeight"] * 0.52));
	m_textureManager->render(PNG_BACKGROUND_CLOUD_ID, m_gRender, pngCloud01_x, int(p_settingJson["SetWindowHeight"] * 0.04), PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);  // ��
	m_textureManager->render(PNG_BACKGROUND_CLOUD_ID, m_gRender, pngCloud02_x, int(p_settingJson["SetWindowHeight"] * 0.08), PNG_USUAL_WIDTH32, PNG_USUAL_HEIGHT32);  // ��
	m_textureManager->render(PNG_BACKGROUND_CLOUD_ID, m_gRender, pngCloud03_x, int(p_settingJson["SetWindowHeight"] * 0.06), PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64);  // ��
	m_textureManager->render(PNG_FULLWINDOW_ID, m_gRender, pngFullWindow_x, pngFullWindow_y, pngFullWindow_width, pngFullWindow_height); // ȫ����
	//��ϷĿ��������Ⱦ
	m_textureManager->render(TTF_GAMEPURPOSE_ID, m_gRender, int(p_settingJson["SetWindowWidth"] * 0.06), int(p_settingJson["SetWindowHeight"] * 0.7), int(p_settingJson["SetWindowWidth"] * 0.125), int(p_settingJson["SetWindowHeight"] * 0.04));
	m_textureManager->render(TTF_GAMEPURPOSE_INFOR_ID, m_gRender, int(p_settingJson["SetWindowWidth"] * 0.06), int(p_settingJson["SetWindowHeight"] * 0.75), int(p_settingJson["SetWindowWidth"] * 0.5), int(p_settingJson["SetWindowHeight"] * 0.04));

	// ImGui��Ⱦ
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_gRender);
}

// ��game.cpp�Ĺ��캯����ִ�У��Լ�game.cpp��startRender����Ϊtrue�����
void StartScreen::initLanguage(SDL_Renderer* m_gRender, TextureManager* m_textureManager, json &p_settingJson, json &p_settingLanguage)
{
	std::ifstream gameSetting(FILE_GAME_SETTING_PATH, std::ios::binary);
	p_settingJson = json::from_bson(gameSetting);
	gameSetting.close();

	//��Ϸ�����ļ� , 0:en, 1:ch
	if (p_settingJson["SetLanguage"] == 0)
	{
		std::ifstream languageSetting(FILE_LANGUAGE_EN_SETTING_PATH, std::ios::binary);
		p_settingLanguage = json::from_bson(languageSetting);
		languageSetting.close();
	}
	if (p_settingJson["SetLanguage"] == 1)
	{
		std::ifstream languageSetting(FILE_LANGUAGE_CH_SETTING_PATH, std::ios::binary);
		p_settingLanguage = json::from_bson(languageSetting);
		languageSetting.close();
	}
	m_textureManager->loadTTF(TTF_GAMEPURPOSE_ID,removeQuotes(p_settingLanguage["GamePurpose"].dump()).c_str(), m_gRender);
    m_textureManager->loadTTF(TTF_GAMEPURPOSE_INFOR_ID, removeQuotes(p_settingLanguage["GamePurposeInfor"].dump()).c_str(), m_gRender);
	
	// pngBackgroundHT_x �� pngBackgroundHT_y ��ʼ����
	pngBackgroundHT_x = (p_settingJson["SetWindowWidth"] - int(p_settingJson["SetWindowWidth"] * 0.78)) / 2;
	pngBackgroundHT_y = -int(p_settingJson["SetWindowHeight"] * 0.52);

	// pngFullWindow_x, y, weight, height ��ʼ���壬ȫ����ť��
	pngFullWindow_x = p_settingJson["SetWindowWidth"] - 64;
	pngFullWindow_y = 32;
	pngFullWindow_width = PNG_USUAL_WIDTH32;
	pngFullWindow_height = PNG_USUAL_HEIGHT32;

}

void StartScreen::update(json p_settingJson)
{
	// HT�����˶�
	if (pngBackgroundHT_y <= int(p_settingJson["SetWindowHeight"] * 0.078))
	{
		pngBackgroundHT_y += pngBackgroundHT_speed;
	}

	// 01�ƶ��ƶ�
	if (pngCloud01_x <= int(p_settingJson["SetWindowWidth"] * 0.32) and isPngCloud01) pngCloud01_x += pngCloud_speed;
	else isPngCloud01 = false;
	if (!isPngCloud01) 
	{
		pngCloud01_x -= pngCloud_speed;
		if (pngCloud01_x <= int(p_settingJson["SetWindowWidth"] * 0.03)) isPngCloud01 = true;
	}

	// 02�ƶ��ƶ�
	if (pngCloud02_x <= int(p_settingJson["SetWindowWidth"] * 0.61) and isPngCloud02) pngCloud02_x += pngCloud_speed;
	else isPngCloud02 = false;
	if (!isPngCloud02)
	{
		pngCloud02_x -= pngCloud_speed;
		if (pngCloud02_x <= int(p_settingJson["SetWindowWidth"] * 0.42)) isPngCloud02 = true;
	}
	
	// 03�ƶ��ƶ�
	if (pngCloud03_x <= int(p_settingJson["SetWindowWidth"] * 0.81) and isPngCloud03) pngCloud03_x += pngCloud_speed;
	else isPngCloud03 = false;
	if (!isPngCloud03)
	{
		pngCloud03_x -= pngCloud_speed;
		if (pngCloud03_x <= int(p_settingJson["SetWindowWidth"] * 0.71)) isPngCloud03 = true;
	}


}

void StartScreen::updateEvent_big(json p_settingJson)
{
	pngFullWindow_x = p_settingJson["SetWindowWidth"] - 72;
	pngFullWindow_y = 24;
	pngFullWindow_width = 48;
	pngFullWindow_height = 48;
}

void StartScreen::updateEvent_small(json p_settingJson)
{
	pngFullWindow_x = p_settingJson["SetWindowWidth"] - 64;
	pngFullWindow_y = 32;
	pngFullWindow_width = PNG_USUAL_WIDTH32;
	pngFullWindow_height = PNG_USUAL_HEIGHT32;
}
