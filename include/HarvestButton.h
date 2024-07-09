#pragma once

#include<SDL.h>
#include<string>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"
#include"nlohmann/json.hpp"

#include"setting/Config.h"
#include"TextureManager.h"
//#include"MainScreen.h"

#include"UsualFunc.h"

using json = nlohmann::json;

class HarvestButton {
public:
	void harvestButtonFrame(json p_settingJson, json p_settingLanguagem, json p_save01Data, bool& IsWindowStatus);
//private:
//	bool isOpen = true;

}; 
