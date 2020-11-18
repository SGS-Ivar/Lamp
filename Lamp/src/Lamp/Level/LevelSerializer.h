#pragma once

#include "Level.h"

namespace Lamp
{
	class LevelSerializer
	{
	public:
		LevelSerializer() = default;
		~LevelSerializer() {}

		bool Serialize(Ref<Level>& level);
		bool Serialize(Ref<Level>& level, const std::string& path);

	private:
		static std::string ToString(const bool& var);
		static std::string ToString(const int& var);
		static std::string ToString(const float& var);
		static std::string ToString(const glm::vec2& var);
		static std::string ToString(const glm::vec3& var);
		static std::string ToString(const glm::vec4& var);
	};
}