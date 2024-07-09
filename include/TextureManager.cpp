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
	// ��ȡ�������ļ�
	SDL_RWops* rwOps = SDL_RWFromFile(filePath.c_str(), "rb");
	if (rwOps == nullptr) { return false; }

	SDL_Surface* tempSurface = IMG_LoadPNG_RW(rwOps);
	if (tempSurface == nullptr) { return false; }

	// ����ʱ��Surface����ת����Texture����
	SDL_Texture* gTexture = SDL_CreateTextureFromSurface(m_gRender, tempSurface);
	if (gTexture == nullptr) { return false; }
	else { m_textureMap[id] = gTexture; return true; }  //��gTexture����map�е�ID, ͨ��IDȥgTexture

	// �ͷ���ʱ��surface����
	SDL_FreeSurface(tempSurface);
	return false;
}

bool TextureManager::initFont(const char* ttfPath, int fontSize)
{
	// ����ttf�ļ���ȷ�������С
	m_gFont = TTF_OpenFont(ttfPath, fontSize);
	if (m_gFont == nullptr) { return false; }
	return true;
}

bool TextureManager::loadTTF(std::string id, const char* text, SDL_Renderer* m_gRender, bool isPlayerName, SDL_Color fg)
{
	// ���ؾ������֣�������ɫ
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr){ return false; }

	// Surface to Texture
	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return false; }
	else
	{
		// ��map������ֵ
		m_textureMap[id] = gFontTexture;

		// ֻ��������������ֵ�ʱ�򣬲ŵ���
		if (isPlayerName){ p_playerNameWidth = tempFontSurface->w; }

		// �ͷ���ʱ��surface
		SDL_FreeSurface(tempFontSurface);
		return true;
	}

	// ���û�и�m_textureMap��ֵ��return false
	return false;
}

bool TextureManager::loadTTFEnemyName(std::string id, const char* text, int floor, SDL_Renderer* m_gRender, SDL_Color fg)
{
	// ���ؾ������֣�������ɫ
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr) { return false; }

	// Surface to Texture
	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return false; }
	else
	{
		if (floor == 1)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor001 = tempFontSurface->w;
		}
		if (floor == 2)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor002 = tempFontSurface->w;
		}
		if (floor == 3)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor003 = tempFontSurface->w;
		}
		if (floor == 4)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor004 = tempFontSurface->w;
		}
		if (floor == 5)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor005 = tempFontSurface->w;
		}
		if (floor == 6)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor006 = tempFontSurface->w;
		}
		if (floor == 7)
		{
			// ��map������ֵ
			m_textureMap[id] = gFontTexture;
			p_EnemyNameWidthFloor007 = tempFontSurface->w;
		}


		// �ͷ���ʱ��surface
		SDL_FreeSurface(tempFontSurface);
		return true;
	}

	// ���û�и�m_textureMap��ֵ��return false
	return false;
}

SDL_Texture* TextureManager::loadTTF(const char* text, SDL_Renderer* m_gRender, SDL_Color fg)
{
	// ���ؾ������֣�������ɫ
	SDL_Surface* tempFontSurface = TTF_RenderUTF8_Solid(m_gFont, text, fg);
	if (tempFontSurface == nullptr) { return nullptr; }

	SDL_Texture* gFontTexture = SDL_CreateTextureFromSurface(m_gRender, tempFontSurface);
	if (gFontTexture == nullptr) { return nullptr; }

	// �ͷ���ʱ��surface
	SDL_FreeSurface(tempFontSurface);

	return gFontTexture;
}

void TextureManager::render(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = destRect.w = width;     // ������src������ͼƬ���У�����ʵ��ͼƬ�ĳ��Ϳ�
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;  // ����ͼƬ���ڳ���������

	SDL_RenderCopy(m_gRender, m_textureMap[id], &srcRect, &destRect);

}

void TextureManager::render(SDL_Texture* m_gTexture, SDL_Renderer* m_gRender, int x, int y, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = destRect.w = width;     // ������src������ͼƬ���У�����ʵ��ͼƬ�ĳ��Ϳ�
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;  // ����ͼƬ���ڳ���������

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
	srcRect.x = width * currentRow;     // ����ͼƬ�����ƶ�
	srcRect.y = height * (currentFrame - 1);    // ����ͼƬ�����ƶ�
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;   // ����ͼƬ���ڳ���������

	SDL_RenderCopyEx(m_gRender, m_textureMap[id], &srcRect, &destRect, angle, center, flip);
}

void TextureManager::dynamicRender(std::string id, SDL_Renderer* m_gRender, int x, int y, int width, int height, int currentRow, int currentFrame, int factor,
	const double angle, const SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentRow ;     // ����ͼƬ�����ƶ�
	srcRect.y = height * (currentFrame - 1);    // ����ͼƬ�����ƶ�
	srcRect.w = width;
	srcRect.h = height;

	destRect.w = width * factor; destRect.h = height * factor;
	destRect.x = x; destRect.y = y;   // ����ͼƬ���ڳ���������

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

