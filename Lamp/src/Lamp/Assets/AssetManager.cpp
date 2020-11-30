#include "lppch.h"
#include "AssetManager.h"

#include "Lamp/Rendering/Texture2D/TextureLoader.h"
#include "Lamp/Rendering/Texture2D/TextureCache.h"

namespace Lamp
{
	AssetManager::AssetManager()
		: m_LoadingThreadActive(true)
	{
	}

	AssetManager::~AssetManager()
	{
		m_LoadingThreadActive = false;
		for (int i = 0; i < m_MaxThreads; i++)
		{
			m_WorkerThreads[i].join();
		}
	}

	void AssetManager::Initialize()
	{
		for (int i = 0; i < m_MaxThreads; i++)
		{
			m_WorkerThreads.push_back(std::thread(&AssetManager::LoaderThread, this));
		}
	}

	void AssetManager::Update()
	{
		if (!m_LoadedQueue.Empty())
		{
			TextureData data;
			if (m_LoadedQueue.PopFront(data))
			{
				TextureCache::GenerateTexture(data);
			}
		}
	}

	void AssetManager::LoadAsset(const std::string& path, AssetType type)
	{
		if (type == AssetType::Texture)
		{
			m_LoadQueue.PushBack(TextureLoadJob(path));
		}
	}

	void AssetManager::LoaderThread()
	{
		while (m_LoadingThreadActive)
		{
			if (!m_LoadQueue.Empty())
			{
				TextureLoadJob job;
				if (m_LoadQueue.PopFront(job))
				{
					auto data = Lamp::TextureLoader::LoadTexture(job.path);
					m_LoadedQueue.PushBack(std::move(data));
				}
			}
		}
	}
}