#pragma once

#include <string>
#include <thread>

#include "Lamp/Utility/ThreadSafeQueue.h"

namespace Lamp
{
	enum class AssetType
	{
		Texture
	};

	struct TextureData
	{
		unsigned char* pData;
		uint32_t width;
		uint32_t height;
		uint32_t channels;
	};

	struct TextureLoadJob
	{
		TextureLoadJob(const std::string& path)
			: path(path)
		{}

		TextureLoadJob()
		{}

		std::string path;
		TextureData data;
	};

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		void Initialize();
		void Update();

		void LoadAsset(const std::string& path, AssetType type);

	private:
		void LoaderThread();

	private:
		ThreadSafeQueue<TextureLoadJob> m_LoadQueue;
		ThreadSafeQueue<TextureData> m_LoadedQueue;

		std::vector<std::thread> m_WorkerThreads;
		std::atomic<bool> m_LoadingThreadActive;
		uint32_t m_MaxThreads = 8;
	};
}