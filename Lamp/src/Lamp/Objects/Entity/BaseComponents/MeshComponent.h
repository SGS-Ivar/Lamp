#pragma once

#include "Lamp/Objects/Entity/Base/BaseComponent.h"
#include "Lamp/Objects/Entity/Base/ComponentRegistry.h"
#include "Lamp/Objects/Entity/Base/Entity.h"

#include "Lamp/Meshes/Model.h"

namespace Lamp
{
	class MeshComponent final : public EntityComponent
	{
	public:
		MeshComponent()
			: EntityComponent("MeshComponent")
		{
			SetComponentProperties
			({
				{ PropertyType::Path, "Path", RegisterData(&m_Path) }
			});
		}

		//////Base//////
		virtual void Initialize() override;
		virtual void OnEvent(Event& e) override;
		virtual uint64_t GetEventMask() override { return EventType::AppRender 
			| EventType::AppUpdate 
			| EventType::EntityPropertyChanged; }
		////////////////

	private:
		bool OnRender(AppRenderEvent& e);
		bool OnUpdate(AppUpdateEvent& e);
		bool OnPropertyChanged(EntityPropertyChangedEvent& e);

	public:
		static Ref<EntityComponent> Create() { return CreateRef<MeshComponent>(); }
		static std::string GetFactoryName() { return "MeshComponent"; }

	private:
		Ref<Model> m_Model;
		std::string m_Path;
	};

}