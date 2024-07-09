#include "TextureManager.h"

TextureManager::TextureManager()
{
	m_gFont = nullptr;

	p_playerNameWidth = 0;  
}

TextureManager::~TextureManager()
{
	TTF_CloseFont(m_gFont);
	TTF_Quit();
}

bool TextureManager::loadPngImg(std::string filePath, std::string id, SDL_Renderer* m_gRender)
{
	// 读取二进制文件
	SDL_RWops* rwOps = SDL_RWFromFile(filePath.c_str(), "rb");
	if (rwOps == nullptr) { return false; }

	SDL_Surface* tempSurface = IMG_LoadPNG_RW(rwOps);
	if (tempSurface == nullptr) { return false; }

	// 将临时的Surface对象转换成Texture对象
	SDL_Texture* gTexture = SDL_CreateTextureFromSurface(m_gRender, tempSurface);
	if (gTexture == nullptr) { return false; }
	else { m_textureMap[id] = gTexture; return true; }  //将gTexture，给map中的ID, 通过ID去gTexture

	// 释放临时的surface对象
	SDL_FreeSurface(tempSurface);
	return false;
}

bool TextureManager::initFont(const char* ttfPath, int fontSize)
{
	// 加载ttf文件，确认字体大小
	m_gFont = TTF_OpenFont(ttfPath, fontSize);
	if (m_gFont == nullptr) { return false; }
	return true;
}

bool TextureManager::loadTTF(std::string id, const char* text, SDL_Renderer* m_gRender, bool isPlayerName, SDL_Color fg)
{
	// 加载具体文字，字体颜色
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr){ return false; }

	// Surface to Texture
	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return false; }
	else
	{
		// 给map容器赋值
		m_textureMap[id] = gFontTexture;

		// 只有在设置玩家名字的时候，才调用
		if (isPlayerName){ p_playerNameWidth = tempFontSurface->w; }

		// 释放临时的surface
		SDL_FreeSurface(tempFontSurface);
		return true;
	}

	// 如果没有给m_textureMap赋值就return false
	return false;
}

bool TextureManager::loadTTFEnemyName(std::string id, const char* text, int floor, SDL_Renderer* m_gRender, SDL_Color fg)
{
	// 加载具体文字，字体颜色
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr) { return false; }

	// Surface to Texture
	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return false; }
	else
	{
		if (floor == 1)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor001 = tempFontSurface->w;
		}
		if (floor == 2)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor002 = tempFontSurface->w;
		}
		if (floor == 3)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor003 = tempFontSurface->w;
		}
		if (floor == 4)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor004 = tempFontSurface->w;
		}
		if (floor == 5)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor005 = tempFontSurface->w;
		}
		if (floor == 6)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor006 = tempFontSurface->w;
		}
		if (floor == 7)
		{
			// 给map容器赋值
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor007 = tempFontSurface->w;
		}


		// 释放临时的surface
		SDL_FreeSurface(tempFontSurface);
		return true;
	}

	// 如果没有给m_textureMap赋值就return false
	return false;
}

SDL_Texture* TextureManager::loadTTF(const char* text, SDL_Renderer* m_gRender, SDL_Color fg)
{
	// 加载具体文字，字体颜色
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr) { return nullptr; }

	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return nullptr; }

	// 释放临时的surface
	SDL_FreeSurface(tempFontSurface);

	return gFontTexture;
}

void TextureManager::render(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = destRect.w = width;     // 控制了src，所以图片会切，输入实际图片的长和宽
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;  // 控制图片所在场景的坐标

	SDL_RenderCopy(m_gRender, m_textureMap[id], &srcRect, &destRect);

}

void TextureManager::render(SDL_Texture* m_gTexture, SDL_Renderer* m_gRender, int x, int y, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = destRect.w = width;     // 控制了src，所以图片会切，输入实际图片的长和宽
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;  // 控制图片所在场景的坐标

	SDL_RenderCopy(m_gRender, m_gTexture, &srcRect, &destRect);
	SDL_DestroyTexture(m_gTexture);
}

void TextureManager::scaleRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int scaleWidthNum, int scaleHeightNum)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	destRect.x = x; destRect.y = y;
	destRect.w = width / scaleWidthNum; destRect.h = height / scaleHeightNum;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = width; srcRect.h = height;

	SDL_RenderCopy(m_gRender, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, 
	const double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentRow;     // 控制图片横向移动
	srcRect.y = height * (currentFrame - 1);    // 控制图片纵向移动
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;   // 控制图片所在场景的坐标

	SDL_RenderCopyEx(m_gRender, m_textureMap[id], &srcRect, &destRect, angle, center, flip);
}

void TextureManager::dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, int factor,
	const double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentRow ;     // 控制图片横向移动
	srcRect.y = height * (currentFrame - 1);    // 控制图片纵向移动
	srcRect.w = width;
	srcRect.h = height;

	destRect.w = width * factor; destRect.h = height * factor;
	destRect.x = x; destRect.y = y;   // 控制图片所在场景的坐标

	SDL_RenderCopyEx(m_gRender, m_textureMap[id], &srcRect, &destRect, angle, center, flip);
}

void TextureManager::setAlpha(std::string id, Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
}

void TextureManager::imguiIMG(std::string id, float width, float height, const char* tooltipText)
{
	ImGui::Image(m_textureMap[id], ImVec2(width, height));
	
	if (ImGui::IsItemHovered())
	{
		if (ImGui::BeginTooltip())
		{
			ImGui::Text(tooltipText);
			ImGui::EndTooltip();
		}
	}
}

void TextureManager::imguiIMG(std::string id, float width, float height)
{
	ImGui::Image(m_textureMap[id], ImVec2(width, height));
}

