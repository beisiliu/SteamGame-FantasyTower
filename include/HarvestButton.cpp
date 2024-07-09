#include "HarvestButton.h"


void HarvestButton::harvestButtonFrame(json p_settingJson, json p_settingLanguage, json p_save01Data, bool& IsWindowStatus)
{
	//isOpen = true;
	ImGui::SetNextWindowSize(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3), float(p_settingJson["SetWindowHeight"] - 192)));
	ImGui::SetNextWindowPos(ImVec2(64.0f, 80.0f));
	ImGui::SetNextWindowBgAlpha(0.8f);  // 设置窗口半透明
	ImGui::Begin(removeQuotes(p_settingLanguage["MainMeum"]["HarvestButton"].dump()).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (p_save01Data["HtFloor"] == 1)
	{
		ImGui::Text(removeQuotes(p_settingLanguage["MainHint1"].dump()).c_str());
	}
	
	// 返回按钮
	// 定位坐标是基于Frame框架内的X,Y坐标的
	ImGui::SetCursorPos(ImVec2(float((p_settingJson["SetWindowWidth"] - 96) / 3 - 50), float(p_settingJson["SetWindowHeight"] - 192 - 30)));
	// 返回到HT主场景
	if (ImGui::Button(removeQuotes(p_settingLanguage["Return"].dump()).c_str()))
	{
		//isOpen = false;
		IsWindowStatus = false;
	}

	ImGui::End();
}