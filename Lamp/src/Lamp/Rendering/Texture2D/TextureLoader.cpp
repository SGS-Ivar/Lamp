#include "lppch.h"
#include "TextureLoader.h"

#include <stb/stb_image.h>
#include "Lamp/Assets/AssetManager.h"

namespace Lamp
{
	TextureData& TextureLoader::LoadTexture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		TextureData d;
		d.pData = data;
		d.width = width;
		d.height = height;
		d.channels = channels;
		return d;
	}
}