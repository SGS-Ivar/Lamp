#include "lppch.h"
#include "TextureCache.h"

#include "TextureLoader.h"
#include <thread>
#include <future>

namespace Lamp
{
	std::map<std::string, std::tuple<uint32_t, uint32_t, uint32_t>> TextureCache::m_TextureCache;

	const std::tuple<uint32_t, uint32_t, uint32_t> TextureCache::GetTexture(const std::string& path)
	{
		auto mit = m_TextureCache.find(path);
		if (mit == m_TextureCache.end())
		{

			std::promise<std::tuple<uint32_t, uint32_t, uint32_t>> p;
			auto tex = p.get_future();

			std::thread t(&TextureLoader::LoadTexture, std::move(p), path);

			m_TextureCache.insert(std::make_pair(path, tex.get()));

			return tex.get();
		}

		return mit->second;
	}
}