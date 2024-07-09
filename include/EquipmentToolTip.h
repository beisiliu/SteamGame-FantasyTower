#pragma once

#include<string>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"

#include"nlohmann/json.hpp"
#include"setting/config.h"
#include"UsualFunc.h"

using json = nlohmann::json;

class EquipmentToolTip
{
public:
	void equitWeaponsToolTip(std::string weaponsName, json p_settingLanguage);
	void equitHeadToolTip(std::string headName, json p_settingLanguage);
	void equitArmsToolTip(std::string armsName, json p_settingLanguage);
	void equitBodyToolTip(std::string bodyName, json p_settingLanguage);
	void equitLegsToolTip(std::string legsName, json p_settingLanguage);
	void skillsTootTip(std::string skillName, json p_settingLanguage);
};
