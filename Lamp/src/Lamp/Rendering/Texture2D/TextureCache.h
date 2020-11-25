#pragma once

namespace Lamp
{
	struct TextureData
	{
		unsigned char* pData;
		uint32_t width;
		uint32_t height;
	};

	class TextureCache
	{
	public:
		static const std::tuple<uint32_t, uint32_t, uint32_t> GetTexture(const std::string& path);

	private:
		static std::map<std::string, std::tuple<uint32_t, uint32_t, uint32_t>> m_TextureCache;
	};
}