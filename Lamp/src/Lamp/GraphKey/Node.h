#pragma once

#include "Lamp/Objects/Entity/Base/Entity.h"
#include "Link.h"

#define LP_BIND_NODE_FUNCTION(x) std::bind(&x, this)

namespace Lamp
{
	class Attribute
	{
	public:
		Attribute()
		{}

		virtual ~Attribute() {}

	public:
		std::string name;
		uint32_t id;
		PropertyType type;
		Ref<Link> pLink = nullptr;
		std::any data;
	};

	struct InputAttribute : public Attribute
	{
		using Func = std::function<void()>;

		InputAttribute(const std::string& n)
		{
			name = n;
		}

		Func function = NULL;
	};

	struct OutputAttribute : public Attribute
	{
		OutputAttribute(const std::string& n)
		{
			name = n;
		}
	};

	struct Node
	{
		Node()
			: name("")
		{}

		Node(std::shared_ptr<Node>& node)
		{
			entityId = node->entityId;
			name = node->name;
			id = node->id;
			pLinks = node->pLinks;
			inputAttributes = node->inputAttributes;
			outputAttributes = node->outputAttributes;
		}

	public:
		template<typename T>
		InputAttribute InputAttributeConfig(const std::string& name, PropertyType type)
		{
			InputAttribute attr(name);
			attr.data = T();
			attr.type = type;

			return attr;
		}

		template<typename T>
		InputAttribute InputAttributeConfig(const std::string& name, PropertyType type, InputAttribute::Func f)
		{
			InputAttribute attr(name);
			attr.data = T();
			attr.type = type;
			attr.function = f;

			return attr;
		}

		InputAttribute InputAttributeConfig_Void(const std::string& name, PropertyType type)
		{
			InputAttribute attr(name);
			attr.type = type;

			return attr;
		}

		InputAttribute InputAttributeConfig_Void(const std::string& name, PropertyType type, InputAttribute::Func f)
		{
			InputAttribute attr(name);
			attr.type = type;
			attr.function = f;

			return attr;
		}

		template<typename T>
		OutputAttribute OutputAttributeConfig(const std::string& name, PropertyType type)
		{
			OutputAttribute attr(name);
			attr.type = type;
			attr.data = T();

			return attr;
		}

		OutputAttribute OutputAttributeConfig_Void(const std::string& name, PropertyType type)
		{
			OutputAttribute attr(name);
			attr.type = type;

			return attr;
		}

		template<typename T>
		void ActivateOutput(uint32_t index, T data)
		{
			if (outputAttributes[index].pLink)
			{
				outputAttributes[index].pLink->pInput->data = data;
				if (outputAttributes[index].pLink->pInput->function != NULL)
				{
					outputAttributes[index].pLink->pInput->function();
				}
			}
		}

		void ActivateOutput(uint32_t index)
		{
			if (outputAttributes[index].pLink)
			{
				if (outputAttributes[index].pLink->pInput->function != NULL)
				{
					outputAttributes[index].pLink->pInput->function();
				}
			}
		}

		template<typename T>
		T& GetInput(uint32_t index)
		{
			return std::any_cast<T&>(inputAttributes[index].data);
		}

	public:
		int entityId = -1;
		std::string name;
		uint32_t id;
		bool needsEntity = false;

		std::vector<Ref<Link>> pLinks;
		std::vector<InputAttribute> inputAttributes;
		std::vector<OutputAttribute> outputAttributes;
	};
}