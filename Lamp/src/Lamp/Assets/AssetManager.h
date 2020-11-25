#pragma once

#include <string>
#include "AssetLoader.h"
#include <queue>

namespace Lamp
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		void LoadAsset(const std::string& path, AssetType type);
		void LoadTexture();
		void Update();

	private:
		std::queue<std::pair<std::string, AssetType>> m_AssetQueue;
	};
}