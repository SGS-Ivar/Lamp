#include "lppch.h"
#include "AssetManager.h"

namespace Lamp
{
	AssetManager::AssetManager()
	{
	}
	AssetManager::~AssetManager()
	{
	}

	void AssetManager::LoadAsset(const std::string& path, AssetType type)
	{
		m_AssetQueue.push(std::make_pair(path, type));
	}

	void AssetManager::Update()
	{
		if (!m_AssetQueue.empty())
		{

		}
	}
}