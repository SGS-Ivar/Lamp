#include "lppch.h"
#include "LevelDeserializer.h"

#include "Lamp/Objects/Brushes/BrushManager.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"
#include <rapidxml/rapidxml_print.hpp>
#include "Lamp/Objects/Entity/Base/ComponentRegistry.h"

namespace Lamp
{
	Ref<Level> LevelDeserializer::Load(const std::string& path)
	{
		LP_CORE_INFO("Loading level...");

		Ref<Level> pLevel;

		rapidxml::xml_document<> file;
		rapidxml::xml_node<>* pRootNode;

		std::ifstream levelFile(path);
		std::vector<char> buffer((std::istreambuf_iterator<char>(levelFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');

		file.parse<0>(&buffer[0]);
		pRootNode = file.first_node("Level");

		pLevel = CreateRef<Level>(pRootNode->first_attribute("name")->value(), path);
		ObjectLayerManager::SetCurrentManager(pLevel->GetObjectLayerManager());
		PhysicsEngine::SetCurrentEngine(pLevel->GetPhysicsEngine());

		if (rapidxml::xml_node<>* pLevelEnv = pRootNode->first_node("LevelEnvironment"))
		{
			if (auto pGA = pLevelEnv->first_node("GlobalAmbient"))
			{
				glm::vec3 gA;
				GetValue(pGA->first_attribute("value")->value(), gA);
				pLevel->GetEnvironment().GlobalAmbient = gA;
			}

			if (auto pCP = pLevelEnv->first_node("CameraValues"))
			{
				glm::vec3 cP;
				GetValue(pCP->first_attribute("position")->value(), cP);

				glm::vec3 cR;
				GetValue(pCP->first_attribute("rotation")->value(), cR);

				float fov;
				GetValue(pCP->first_attribute("fov")->value(), fov);

				pLevel->GetEnvironment().CameraPosition = cP;
				pLevel->GetEnvironment().CameraRotation = cR;
				pLevel->GetEnvironment().CameraFOV = fov;
			}
		}

		if (rapidxml::xml_node<>* pLayers = pRootNode->first_node("Layers"))
		{
			pLevel->GetObjectLayerManager()->SetLayers(LoadLayers(pLayers, pLevel->GetObjectLayerManager()));
		}

		if (rapidxml::xml_node<>* pBrushes = pRootNode->first_node("Brushes"))
		{
			pLevel->GetBrushManager()->SetBrushes(LoadBrushes(pBrushes, pLevel->GetBrushManager()));
		}

		if (rapidxml::xml_node<>* pEntities = pRootNode->first_node("Entities"))
		{
			pLevel->GetEntityManager()->SetEntities(LoadEntities(pEntities, pLevel->GetEntityManager()));
		}

		LP_CORE_INFO("Level loaded!");

		EntityManager::SetCurrentManager(pLevel->GetEntityManager());
		BrushManager::SetCurrentManager(pLevel->GetBrushManager());

		return pLevel;
	}

	std::vector<ObjectLayer> LevelDeserializer::LoadLayers(rapidxml::xml_node<>* pNode, Ref<ObjectLayerManager>& objLayerManager)
	{
		std::vector<ObjectLayer> layers;

		for (rapidxml::xml_node<>* pLayer = pNode->first_node("Layer"); pLayer; pLayer = pLayer->next_sibling())
		{
			std::string name(pLayer->first_attribute("name")->value());

			int id;
			GetValue(pLayer->first_attribute("id")->value(), id);

			bool destroyable;
			GetValue(pLayer->first_attribute("destroyable")->value(), destroyable);

			layers.push_back(ObjectLayer((uint32_t)id, name, destroyable));
		}

		layers.push_back(ObjectLayer(0, "Main", false));

		return layers;
	}
	std::vector<Brush*> LevelDeserializer::LoadBrushes(rapidxml::xml_node<>* pNode, Ref<BrushManager>& brushManager)
	{
		std::vector<Brush*> pBrushes;

		for (rapidxml::xml_node<>* pBrush = pNode->first_node("Brush"); pBrush; pBrush = pBrush->next_sibling())
		{
			std::string path(pBrush->first_attribute("lgfPath")->value());

			glm::vec3 pos(0, 0, 0);
			GetValue(pBrush->first_attribute("position")->value(), pos);

			glm::vec3 rot(0, 0, 0);
			GetValue(pBrush->first_attribute("rotation")->value(), rot);

			glm::vec3 scale(1, 1, 1);
			GetValue(pBrush->first_attribute("scale")->value(), scale);

			int layerID;
			GetValue(pBrush->first_attribute("layerID")->value(), layerID);

			std::string name = pBrush->first_attribute("name")->value();

			pBrushes.push_back(brushManager->Create(path, pos, rot, scale, layerID, name));
		}

		return pBrushes;
	}
	std::vector<Entity*> LevelDeserializer::LoadEntities(rapidxml::xml_node<>* pNode, Ref<EntityManager>& entityManager)
	{
		std::vector<Entity*> pEntities;

		for (rapidxml::xml_node<>* pEntity = pNode->first_node("Entity"); pEntity; pEntity = pEntity->next_sibling())
		{
			Entity* pEnt = entityManager->Create();

			std::string name = pEntity->first_attribute("name")->value();

			glm::vec3 pos(0, 0, 0);
			GetValue(pEntity->first_attribute("position")->value(), pos);

			glm::vec3 rot(0, 0, 0);
			GetValue(pEntity->first_attribute("rotation")->value(), rot);

			glm::vec3 scale(1, 1, 1);
			GetValue(pEntity->first_attribute("scale")->value(), scale);

			pEnt->SetName(name);
			pEnt->SetPosition(pos);
			pEnt->SetRotation(rot);
			pEnt->SetScale(scale);

			for (rapidxml::xml_node<>* pComponent = pEntity->first_node("Component"); pComponent; pComponent = pComponent->next_sibling())
			{
				auto name = pComponent->first_attribute("name")->value();
				Ref<EntityComponent> pComp = ComponentRegistry::Create(name);
				pComp->MakeOwner(pEnt);
				pComp->Initialize();

				for (rapidxml::xml_node<>* pParam = pComponent->first_node("param"); pParam; pParam = pParam->next_sibling())
				{
					auto paramName = pParam->first_attribute("name")->value();
					auto type = ComponentProperties::GetTypeFromString(pParam->first_attribute("type")->value());

					switch (type)
					{
					case Lamp::PropertyType::String:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								std::string* p = static_cast<std::string*>(prop.Value);
								*p = std::string(pParam->first_attribute("value")->value());
							}
						}

						break;
					}
					case Lamp::PropertyType::Bool:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								bool* p = static_cast<bool*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Int:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								int* p = static_cast<int*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Float:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								float* p = static_cast<float*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Float2:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								glm::vec2* p = static_cast<glm::vec2*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Float3:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								glm::vec3* p = static_cast<glm::vec3*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Float4:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								glm::vec4* p = static_cast<glm::vec4*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Color3:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								glm::vec3* p = static_cast<glm::vec3*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					case Lamp::PropertyType::Color4:
					{
						for (auto& prop : pComp->GetComponentProperties().GetProperties())
						{
							if (prop.Name == paramName)
							{
								glm::vec4* p = static_cast<glm::vec4*>(prop.Value);
								GetValue(pParam->first_attribute("value")->value(), *p);
							}
						}
						break;
					}
					}
				}

				pEnt->AddComponent(pComp);
			}

			pEntities.push_back(pEnt);
		}

		return pEntities;
	}

	//////////Getters//////////
	bool LevelDeserializer::GetValue(char* val, bool& var)
	{
		if (val)
		{
			var = atoi(val) != 0;
			return true;
		}

		return false;
	}
	bool LevelDeserializer::GetValue(char* val, int& var)
	{
		if (val)
		{
			var = atoi(val);
			return true;
		}

		return false;
	}
	bool LevelDeserializer::GetValue(char* val, float& var)
	{
		if (val)
		{
			var = (float)atof(val);
			return true;
		}
		return false;
	}
	bool LevelDeserializer::GetValue(char* val, glm::vec2& var)
	{
		if (val)
		{
			float x, y;
			if (sscanf(val, "%f,%f", &x, &y) == 2)
			{
				var = glm::vec2(x, y);
				return true;
			}
		}
		return false;
	}
	bool LevelDeserializer::GetValue(char* val, glm::vec3& var)
	{
		if (val)
		{
			float x, y, z;
			if (sscanf(val, "%f,%f,%f", &x, &y, &z) == 3)
			{
				var = glm::vec3(x, y, z);
				return true;
			}
		}
		return false;
	}
	bool LevelDeserializer::GetValue(char* val, glm::vec4& var)
	{
		if (val)
		{
			float x, y, z, w;
			if (sscanf(val, "%f,%f,%f,%f", &x, &y, &z, &w) == 4)
			{
				var = glm::vec4(x, y, z, w);
				return true;
			}
		}
		return false;
	}
}