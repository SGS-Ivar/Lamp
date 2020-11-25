#pragma once

#include <string>

namespace Lamp
{
	enum class AssetType
	{
		Texture
	};

	class LoadTextureJob
	{
	public:
		void DoWork()
		{

		}
	};

	class AssetLoader
	{
	public:
		virtual ~AssetLoader() = 0;
		virtual bool LoadAsset(const std::string& path) = 0;
	};

	class AssetLoaderTexture : public AssetLoader
	{
	public:
		AssetLoaderTexture() {}
		virtual ~AssetLoaderTexture() override {}

		virtual bool LoadAsset(const std::string& path) override
		{

		}

	private:

	};
}