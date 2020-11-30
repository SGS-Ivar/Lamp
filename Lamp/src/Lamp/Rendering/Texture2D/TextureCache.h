#pragma once

#include <vector>

namespace Lamp
{
	class Texture2D;
	struct TextureData;

	class TextureCache
	{
	public:
		static const Ref<Texture2D>& GetTexture(const std::string& path);
		static void GenerateTexture(TextureData& data);

	private:
		static std::vector<Ref<Texture2D>> m_TextureCache;
	};
}