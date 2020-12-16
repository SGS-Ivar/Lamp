#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Lamp/Meshes/Model.h"
#include "Lamp/Objects/Entity/Base/Physical/PhysicalEntity.h"

#include "Lamp/Objects/Object.h"

namespace Lamp
{
	class Brush : public Object
	{
	public:
		Brush(Ref<Model> model);

		virtual void UpdatedMatrix() override;
		virtual void OnEvent(Event& e) override;
		virtual void Destroy() override;
		virtual uint64_t GetEventMask() override { return EventType::AppRender; }

		//Getting
		inline Ref<Model>& GetModel() { return m_Model; }

	public:
		static Brush* Create(const std::string& path);

	private:
		bool OnRender(AppRenderEvent& e);

	private:
		Ref<Model> m_Model;
	};
}