//处理渲染

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
	// filePath必须是二进制的png文件
	bool loadPngImg(std::string filePath, std::string id, SDL_Renderer* m_gRender);
	// 初始化m_gFont
	bool initFont(const char* ttfPath, int fontSize);
	// 加载字体, 字体颜色默认黑色
	bool loadTTF(std::string id, const char* text, SDL_Renderer* m_gRender, bool isPlayerName = false, SDL_Color fg = {0, 0, 0});
	SDL_Texture* loadTTF(const char* text, SDL_Renderer* m_gRender, SDL_Color fg = { 0, 0, 0 });
	bool loadTTFEnemyName(std::string id, const char* text, int floor, SDL_Renderer* m_gRender, SDL_Color fg = { 0, 0, 0 });

	// x, y 是图片的顶点坐标， width和height是图片长和宽，支持拉伸，不支持缩放
	void render(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height);
	void render(SDL_Texture* m_gTexture, SDL_Renderer* m_gRender, int x, int y, int width, int height);

	// x, y 是图片的顶点坐标， width和height是图片长和宽，currentRow控制图片横向移动，currentFrame控制图片纵向移动
	// 支持图片的缩放
	void scaleRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int scaleWidthNum, int scaleHeightNum);
	// 支持原图的图片动画控制
	void dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, 
		const double angle = 0, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// 支持对放大后的图片动画控制，长和宽的同时放大，factor是放大因子
	void dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, int factor,
		const double angle = 0, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// 设置图片的apache值
	void setAlpha(std::string id, Uint8 alpha);

	void imguiIMG(std::string id, float width, float height, const char* tooltipText);
	void imguiIMG(std::string id, float width, float height);

	int p_playerNameWidth;  // 获取玩家姓名的宽度

	// 获取怪物名字的宽度
	int p_EnemyNameWidthFloor001 = 0;
	int p_EnemyNameWidthFloor002 = 0;
	int p_EnemyNameWidthFloor003 = 0;
	int p_EnemyNameWidthFloor004 = 0;
	int p_EnemyNameWidthFloor005 = 0;
	int p_EnemyNameWidthFloor006 = 0;
	int p_EnemyNameWidthFloor007 = 0;
private:
	std::map<std::string, SDL_Texture*> m_textureMap;
	// 构造函数中初始化, 析构函数释放
	TTF_Font* m_gFont;
};