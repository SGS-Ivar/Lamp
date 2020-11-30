#pragma once

namespace Lamp
{
	struct TextureData;

	class TextureLoader
	{
	public:
		static TextureData& LoadTexture(const std::string& path);
	};
}