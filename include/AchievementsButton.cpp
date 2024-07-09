#include"AchievementsButton.h"

void AchievementsButton::achievementsButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus, TextureManager *m_textureManager)
{
	//isOpen = true;
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 128)), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["AchievementsButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	
	//ImVec2 windowSize = ImGui::GetWindowSize();
	//std::cout << windowSize.x << std::endl;

	// ����save01.bin�е��������ж��Ƿ����
	// һ��ɾ�
	if (p_save01Data["isAchievementsFloor001"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor001"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor001"].dump()).c_str());
		ImGui::SameLine();
	}

	// ����ɾ�
	if (p_save01Data["isAchievementsFloor002"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor002"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor002"].dump()).c_str());
		ImGui::SameLine();
	}

	// ����ɾ�
	if (p_save01Data["isAchievementsFloor003"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor003"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor003"].dump()).c_str());
		ImGui::SameLine();
	}
	 
	// �Ĳ�ɾ�
	if (p_save01Data["isAchievementsFloor004"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor004"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor004"].dump()).c_str());
		ImGui::SameLine();
	}

	// ���ɾ�
	if (p_save01Data["isAchievementsFloor005"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor005"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor005"].dump()).c_str());
		ImGui::SameLine();
	}

	// ����ɾ�
	if (p_save01Data["isAchievementsFloor006"])
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_STAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor006"].dump()).c_str());
		ImGui::SameLine();
	}
	else
	{
		m_textureManager->imguiIMG(PNG_ACHIEVEMENTS_NOSTAR_ID, PNG_USUAL_WIDTH64, PNG_USUAL_HEIGHT64, removeQuotes(p_settingLanguage["AchievementsToolTip"]["AchievementsFloor006"].dump()).c_str());
		ImGui::SameLine();
	}

	// ���ذ�ť
	// ��λ�����ǻ���Frame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(float(p_settingJson["SetWindowWidth"] - 128 - 60), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		//isOpen = false;
		IsWindowStatus = false;
	}

	ImGui::End();
}