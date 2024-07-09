#include "include/Game.h"
#include <chrono>

// 控制帧率，帧率64固定
const int FPS = 64;
const int PRE_FRAME_TIME = 1000 / FPS;
Uint32 frameStartTime = 0;
Uint32 frameTime = 0;

int main(int argc, char *argv[])
{
	Game* mainGame = new Game();
	if (!mainGame->initWindow("HT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mainGame->p_settingJson["SetWindowWidth"], mainGame->p_settingJson["SetWindowHeight"], mainGame->p_settingJson["isFullWindow"])) return -1;

	if (mainGame->init())
	{
		// 1.2的release版本 加载只需0.06秒，大于5秒后做加载页面
		auto start = std::chrono::high_resolution_clock::now();
		if (!mainGame->loadGame()) { return -2; }  // 加载资源失败，返回-1
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
		std::cout << "函数执行了 " << duration / 1000000.0 << " 秒\n";

		//if (!mainGame->loadGame()) { return -2; }  // 加载资源失败，返回-1

		// 主循环
		while (mainGame->p_gameIsRunning) 
		{
			// 循环播放音乐
			if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(mainGame->p_gMusic, -1);
			}

			// 获取一个开始时间
			frameStartTime = SDL_GetTicks();

			mainGame->handleEvents();  // 事件
			
			mainGame->render(); //渲染

			mainGame->update();

			// 获取一个循环花费了多少时间
			frameTime = SDL_GetTicks() - frameStartTime;

			// 实际一帧花费的时间 < 规定一帧花费的时间
			if (frameTime < PRE_FRAME_TIME)
			{
				SDL_Delay(PRE_FRAME_TIME - frameTime);
			}
		}
	}

	// 释放main中资源
	delete mainGame;
	return 0;
}