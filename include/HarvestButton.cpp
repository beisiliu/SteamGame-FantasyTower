#include "HarvestButton.h"


void HarvestButton::harvestButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus)
{
	//isOpen = true;
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // ���ô��ڰ�͸��
	ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["HarvestButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (p_save01Data["HtFloor"] == 1)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}
	
	// ���ذ�ť
	// ��λ�����ǻ���Frame����ڵ�X,Y�����
	ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
	// ���ص�HT������
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		//isOpen = false;
		IsWindowStatus = false;
	}

	ImGui::End();
}