#include "lppch.h"
#include "TextureCache.h"

#include "TextureLoader.h"
#include <thread>
#include <future>

#include <glad/glad.h>
#include "Lamp/Assets/AssetManager.h"

//Temp
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Lamp
{
	std::vector<Ref<Texture2D>> TextureCache::m_TextureCache;

	const Ref<Texture2D>& TextureCache::GetTexture(const std::string& path)
	{
		for (auto it = m_TextureCache.begin(); it != m_TextureCache.end(); it++)
		{
			if (it->get()->GetPath() == path)
			{
				return *it;
			}
		}

		g_pEnv->pAssetManager->LoadAsset(path, AssetType::Texture);

		for (auto it = m_TextureCache.begin(); it != m_TextureCache.end(); it++)
		{
			if (it->get()->GetPath() == path)
			{
				return *it;
			}
		}

		return nullptr;
	}

	void TextureCache::GenerateTexture(TextureData& data)
	{
		uint32_t texture;

		//Generate texture and bind it to GL_TEXTURE_2D
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Set texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Set filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GLenum dataFormat = 0, internalFormat = 0;
		if (data.channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (data.channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		if (data.pData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data.width, data.height, 0, dataFormat, GL_UNSIGNED_BYTE, data.pData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			LP_CORE_WARN("Failed to load texture!");
		}

		Ref<OpenGLTexture2D> tex = CreateRef<OpenGLTexture2D>(data.width, data.height, texture, internalFormat, dataFormat);
		
		m_TextureCache.push_back(tex);
	}
}