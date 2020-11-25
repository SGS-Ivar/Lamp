#pragma once

#include <future>

namespace Lamp
{
	class TextureLoader
	{
	public:
		static void LoadTexture(std::promise<TextureData>&& p, const std::string& path);
	};
}