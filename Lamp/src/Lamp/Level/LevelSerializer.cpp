#include "lppch.h"
#include "LevelSerializer.h"

#include <rapidxml/rapidxml_print.hpp>

namespace Lamp
{
	bool LevelSerializer::Serialize(Ref<Level>& level, const std::string& path)
	{
		LP_CORE_INFO("Saving level...");

		std::ofstream file;
		rapidxml::xml_document<> doc;
		file.open(path);

		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, "Level");
		pRoot->append_attribute(doc.allocate_attribute("name", level->GetName().c_str()));

		/////Brushes/////
		rapidxml::xml_node<>* pBrushes = doc.allocate_node(rapidxml::node_element, "Brushes");
		for (auto& brush : level->GetBrushManager()->GetBrushes())
		{
			rapidxml::xml_node<>* child = doc.allocate_node(rapidxml::node_element, "Brush");

			child->append_attribute(doc.allocate_attribute("lgfPath", brush->GetModel()->GetLGFPath().c_str()));

			char* pPos = doc.allocate_string(ToString(brush->GetPosition()).c_str());
			child->append_attribute(doc.allocate_attribute("position", pPos));

			char* pRot = doc.allocate_string(ToString(brush->GetRotation()).c_str());
			child->append_attribute(doc.allocate_attribute("rotation", pRot));

			char* pScale = doc.allocate_string(ToString(brush->GetScale()).c_str());
			child->append_attribute(doc.allocate_attribute("scale", pScale));

			char* pLayer = doc.allocate_string(ToString((int)brush->GetLayerID()).c_str());
			child->append_attribute(doc.allocate_attribute("layerID", pLayer));

			char* pName = doc.allocate_string(brush->GetName().c_str());
			child->append_attribute(doc.allocate_attribute("name", pName));

			pBrushes->append_node(child);
		}
		pRoot->append_node(pBrushes);

		/////////////////

		/////Layers//////
		rapidxml::xml_node<>* pLayers = doc.allocate_node(rapidxml::node_element, "Layers");
		for (auto& layer : ObjectLayerManager::Get()->GetLayers())
		{
			if (layer.ID == 0)
				continue;

			rapidxml::xml_node<>* child = doc.allocate_node(rapidxml::node_element, "Layer");

			char* pId = doc.allocate_string(ToString((int)layer.ID).c_str());
			child->append_attribute(doc.allocate_attribute("id", pId));

			char* pName = doc.allocate_string(layer.Name.c_str());
			child->append_attribute(doc.allocate_attribute("name", pName));

			char* pDestroyable = doc.allocate_string(ToString(layer.IsDestroyable).c_str());
			child->append_attribute(doc.allocate_attribute("destroyable", pDestroyable));

			pLayers->append_node(child);
		}
		pRoot->append_node(pLayers);
		/////////////////

		////Entities/////
		rapidxml::xml_node<>* pEntities = doc.allocate_node(rapidxml::node_element, "Entities");
		for (auto& entity : level->GetEntityManager()->GetEntities())
		{
			if (!entity->GetSaveable())
			{
				continue;
			}

			rapidxml::xml_node<>* ent = doc.allocate_node(rapidxml::node_element, "Entity");

			char* pName = doc.allocate_string(entity->GetName().c_str());
			ent->append_attribute(doc.allocate_attribute("name", pName));

			char* pPos = doc.allocate_string(ToString(entity->GetPosition()).c_str());
			ent->append_attribute(doc.allocate_attribute("position", pPos));

			char* pRot = doc.allocate_string(ToString(entity->GetRotation()).c_str());
			ent->append_attribute(doc.allocate_attribute("rotation", pRot));

			char* pScale = doc.allocate_string(ToString(entity->GetScale()).c_str());
			ent->append_attribute(doc.allocate_attribute("scale", pScale));

			for (auto component : entity->GetComponents())
			{
				rapidxml::xml_node<>* comp = doc.allocate_node(rapidxml::node_element, "Component");

				std::string str = component->GetName();
				str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());

				char* pCompName = doc.allocate_string(str.c_str());
				comp->append_attribute(doc.allocate_attribute("name", pCompName));

				for (auto prop : component->GetComponentProperties().GetProperties())
				{
					rapidxml::xml_node<>* param = doc.allocate_node(rapidxml::node_element, "Param");

					char* pParamName = doc.allocate_string(prop.Name.c_str());
					param->append_attribute(doc.allocate_attribute("name", pParamName));

					char* pType = doc.allocate_string(ComponentProperties::GetStringFromType(prop.PropertyType).c_str());
					param->append_attribute(doc.allocate_attribute("type", pType));

					switch (prop.PropertyType)
					{
					case Lamp::PropertyType::String:
					{
						char* pValue = doc.allocate_string(static_cast<std::string*>(prop.Value)->c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Bool:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<bool*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Int:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<int*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Float:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<float*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Float2:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<glm::vec2*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Float3:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<glm::vec3*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Float4:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<glm::vec4*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Color3:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<glm::vec3*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
						break;
					}
					case Lamp::PropertyType::Color4:
					{
						char* pValue = doc.allocate_string(ToString(*static_cast<glm::vec4*>(prop.Value)).c_str());
						param->append_attribute(doc.allocate_attribute("value", pValue));
					}
					default:
						break;
					}

					comp->append_node(param);
				}

				ent->append_node(comp);
			}

			pEntities->append_node(ent);
		}
		pRoot->append_node(pEntities);
		/////////////////

		/////Level environment/////
		rapidxml::xml_node<>* pLevelEnv = doc.allocate_node(rapidxml::node_element, "LevelEnvironment");

		rapidxml::xml_node<>* globalAmbient = doc.allocate_node(rapidxml::node_element, "GlobalAmbient");
		char* pGA = doc.allocate_string(ToString(level->GetEnvironment().GlobalAmbient).c_str());
		globalAmbient->append_attribute(doc.allocate_attribute("value", pGA));

		rapidxml::xml_node<>* cameraValues = doc.allocate_node(rapidxml::node_element, "CameraValues");
		char* pCP = doc.allocate_string(ToString(level->GetEnvironment().CameraPosition).c_str());
		cameraValues->append_attribute(doc.allocate_attribute("position", pCP));

		char* pCR = doc.allocate_string(ToString(level->GetEnvironment().CameraRotation).c_str());
		cameraValues->append_attribute(doc.allocate_attribute("rotation", pCR));

		char* pCF = doc.allocate_string(ToString(level->GetEnvironment().CameraFOV).c_str());
		cameraValues->append_attribute(doc.allocate_attribute("fov", pCF));

		pLevelEnv->append_node(globalAmbient);
		pLevelEnv->append_node(cameraValues);
		pRoot->append_node(pLevelEnv);
		///////////////////////////

		doc.append_node(pRoot);
		file << doc;
		file.close();

		LP_CORE_INFO("Saved level!");

		return true;
	}
	bool LevelSerializer::Serialize(Ref<Level>& level)
	{
		return Serialize(level, level->GetPath());
	}

	std::string LevelSerializer::ToString(const bool& var)
	{
		std::string str;
		if (var)
		{
			str = "1";
		}
		else
		{
			str = "0";
		}

		return str;
	}
	std::string LevelSerializer::ToString(const int& var)
	{
		return std::to_string(var);
	}
	std::string LevelSerializer::ToString(const float& var)
	{
		return std::to_string(var);
	}
	std::string LevelSerializer::ToString(const glm::vec2& var)
	{
		std::string str(std::to_string(var.x) + "," + std::to_string(var.y));
		return str;
	}
	std::string LevelSerializer::ToString(const glm::vec3& var)
	{
		std::string str(std::to_string(var.x) + "," + std::to_string(var.y) + "," + std::to_string(var.z));
		return str;
	}
	std::string LevelSerializer::ToString(const glm::vec4& var)
	{
		std::string str(std::to_string(var.x) + "," + std::to_string(var.y) + "," + std::to_string(var.z) + "," + std::to_string(var.w));
		return str;
	}
}