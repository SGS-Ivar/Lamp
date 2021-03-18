#pragma once

#include <thread>
#include <vector>
#include <unordered_map>

#include "Lamp/Utility/ThreadSafeQueue.h"
#include "ModelLoader.h"
#include "Lamp/Meshes/Model.h"
#include "Lamp/Level/Level.h"

namespace Lamp
{
	class Texture2D;
	class Model;
	struct TextureLoadJob
	{
		Texture2D* pTexture = nullptr;

		std::string path;
		TextureLoadData data;
	};

	struct ModelLoadJob
	{
		Model* pModel = nullptr;

		std::string path;
		ModelLoadData data;
	};

	struct LevelLoadJob
	{
		Level* pLevel = nullptr;
		
		std::string path;
		LevelLoadData data;
	};

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		void Initialize();
		void Shutdown();

		void Update();

		void LoadTexture(const std::string& path, Texture2D* pTex);
		void LoadModel(const std::string& path, Model* pModel);
		void LoadLevel(const std::string& path, Level* pLevel);

	private:
		void LoaderThread();

	private:
		std::vector<std::thread> m_WorkerThreads;
		std::unordered_map<std::thread::id, std::string> m_ThreadNames;
		std::mutex m_OutputMutex;

		//Textures
		ThreadSafeQueue<TextureLoadJob> m_LoadingTexturesQueue;
		ThreadSafeQueue<TextureLoadJob> m_ProcessingTexturesQueue;

		//Models
		ThreadSafeQueue<ModelLoadJob> m_LoadingModelsQueue;
		ThreadSafeQueue<ModelLoadJob> m_ProcessingModelsQueue;

		//Levels
		ThreadSafeQueue<LevelLoadJob> m_LoadingLevelsQueue;
		ThreadSafeQueue<LevelLoadJob> m_ProcessingLevelsQueue;

		uint32_t m_MaxThreads = 8;
		bool m_LoadingThreadActive = true;
	};
}