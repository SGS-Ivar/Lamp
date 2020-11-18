#pragma once

#include <Lamp/Core/Core.h>
#include <string>

#include "Level.h"
#include <rapidxml/rapidxml.hpp>

namespace Lamp
{
	class LevelDeserializer
	{
	public:
		LevelDeserializer() = default;
		~LevelDeserializer() {}

		Ref<Level> Load(const std::string& path);

	private:
		std::vector<Brush*> LoadBrushes(rapidxml::xml_node<>* pNode, Ref<BrushManager>& brushManager);
		std::vector<Entity*> LoadEntities(rapidxml::xml_node<>* pNode, Ref<EntityManager>& entityManager);
		std::vector<ObjectLayer> LoadLayers(rapidxml::xml_node<>* pNode, Ref<ObjectLayerManager>& objLayerManager);

		static bool GetValue(char* val, bool& var);
		static bool GetValue(char* val, int& var);
		static bool GetValue(char* val, float& var);
		static bool GetValue(char* val, glm::vec2& var);
		static bool GetValue(char* val, glm::vec3& var);
		static bool GetValue(char* val, glm::vec4& var);
	};
}