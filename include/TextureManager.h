//������Ⱦ

#pragma once
#include <string>
#include <map>
#include <array>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include"imgui/imgui.h"
#include"imgui/imgui_impl_sdl2.h"
#include"imgui/imgui_impl_sdlrenderer2.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	// filePath�����Ƕ����Ƶ�png�ļ�
	bool loadPngImg(std::string filePath, std::string id, SDL_Renderer* m_gRender);
	// ��ʼ��m_gFont
	bool initFont(const char* ttfPath, int fontSize);
	// ��������, ������ɫĬ�Ϻ�ɫ
	bool loadTTF(std::string id, const char* text, SDL_Renderer* m_gRender, bool isPlayerName = false, SDL_Color fg = {0, 0, 0});
	SDL_Texture* loadTTF(const char* text, SDL_Renderer* m_gRender, SDL_Color fg = { 0, 0, 0 });
	bool loadTTFEnemyName(std::string id, const char* text, int floor, SDL_Renderer* m_gRender, SDL_Color fg = { 0, 0, 0 });

	// x, y ��ͼƬ�Ķ������꣬ width��height��ͼƬ���Ϳ�֧�����죬��֧������
	void render(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height);
	void render(SDL_Texture* m_gTexture, SDL_Renderer* m_gRender, int x, int y, int width, int height);

	// x, y ��ͼƬ�Ķ������꣬ width��height��ͼƬ���Ϳ�currentRow����ͼƬ�����ƶ���currentFrame����ͼƬ�����ƶ�
	// ֧��ͼƬ������
	void scaleRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int scaleWidthNum, int scaleHeightNum);
	// ֧��ԭͼ��ͼƬ��������
	void dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, 
		const double angle = 0, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// ֧�ֶԷŴ���ͼƬ�������ƣ����Ϳ��ͬʱ�Ŵ�factor�ǷŴ�����
	void dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, int factor,
		const double angle = 0, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// ����ͼƬ��apacheֵ
	void setAlpha(std::string id, Uint8 alpha);

	void imguiIMG(std::string id, float width, float height, const char* tooltipText);
	void imguiIMG(std::string id, float width, float height);

	int p_playerNameWidth;  // ��ȡ��������Ŀ��

	// ��ȡ�������ֵĿ��
	int p_EnemyNameWidthFloor001 = 0;
	int p_EnemyNameWidthFloor002 = 0;
	int p_EnemyNameWidthFloor003 = 0;
	int p_EnemyNameWidthFloor004 = 0;
	int p_EnemyNameWidthFloor005 = 0;
	int p_EnemyNameWidthFloor006 = 0;
	int p_EnemyNameWidthFloor007 = 0;
private:
	std::map<std::string, SDL_Texture*> m_textureMap;
	// ���캯���г�ʼ��, ���������ͷ�
	TTF_Font* m_gFont;
};