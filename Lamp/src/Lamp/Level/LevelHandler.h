#pragma once

#include "LevelSerializer.h"
#include "LevelDeserializer.h"

#include "Lamp/Core/Core.h"

namespace Lamp
{
	class LevelHandler
	{
	public:
		static bool Load(const std::string& path)
		{
			LevelDeserializer* deserializer = new LevelDeserializer();
			s_Level = deserializer->Load(path);

			delete deserializer;
			return true;
		}

		static bool Save(Ref<Level>& level)
		{
			LevelSerializer* serializer = new LevelSerializer();
			bool state = serializer->Serialize(level);
			
			delete serializer;
			return state;
		}

		static bool Save(Ref<Level>& level, const std::string& path)
		{
			LevelSerializer* serializer = new LevelSerializer();
			bool state = serializer->Serialize(level, path);

			delete serializer;
			return state;
		}

		static bool SaveCurrent()
		{
			return Save(s_Level);
		}

		static Ref<Level>& GetCurrent() { return s_Level; }
		static void SetCurrent(Ref<Level> level) { s_Level = level; }

	private:
		LevelHandler() = delete;

		static Ref<Level> s_Level;
	};
}