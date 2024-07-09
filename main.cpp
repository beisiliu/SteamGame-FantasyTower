#include "include/Game.h"
#include <chrono>

// ����֡�ʣ�֡��64�̶�
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
		// 1.2��release�汾 ����ֻ��0.06�룬����5���������ҳ��
		auto start = std::chrono::high_resolution_clock::now();
		if (!mainGame->loadGame()) { return -2; }  // ������Դʧ�ܣ�����-1
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
		std::cout << "����ִ���� " << duration / 1000000.0 << " ��\n";

		//if (!mainGame->loadGame()) { return -2; }  // ������Դʧ�ܣ�����-1

		// ��ѭ��
		while (mainGame->p_gameIsRunning) 
		{
			// ѭ����������
			if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(mainGame->p_gMusic, -1);
			}

			// ��ȡһ����ʼʱ��
			frameStartTime = SDL_GetTicks();

			mainGame->handleEvents();  // �¼�
			
			mainGame->render(); //��Ⱦ

			mainGame->update();

			// ��ȡһ��ѭ�������˶���ʱ��
			frameTime = SDL_GetTicks() - frameStartTime;

			// ʵ��һ֡���ѵ�ʱ�� < �涨һ֡���ѵ�ʱ��
			if (frameTime < PRE_FRAME_TIME)
			{
				SDL_Delay(PRE_FRAME_TIME - frameTime);
			}
		}
	}

	// �ͷ�main����Դ
	delete mainGame;
	return 0;
}