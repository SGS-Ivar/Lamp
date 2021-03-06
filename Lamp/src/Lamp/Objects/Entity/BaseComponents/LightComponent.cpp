#include "lppch.h"
#include "LightComponent.h"

#include "Lamp/Rendering/Shadows/PointShadowBuffer.h"

namespace Lamp
{
	LP_REGISTER_COMPONENT(LightComponent);

	LightComponent::LightComponent()
		: EntityComponent("LightComponent")
	{
		{
			FramebufferSpecification spec;
			spec.Height = 512;
			spec.Width = 512;

			m_pPointLight = new PointLight();

			m_pPointLight->ShadowBuffer = std::make_shared<PointShadowBuffer>(spec);
			g_pEnv->pRenderUtils->RegisterPointLight(m_pPointLight);
		}


		SetComponentProperties
		({
			{ PropertyType::Float, "Intensity", RegisterData(&m_pPointLight->Intensity) },
			{ PropertyType::Float, "Radius", RegisterData(&m_pPointLight->Radius) },
			{ PropertyType::Float, "Falloff", RegisterData(&m_pPointLight->Falloff) },
			{ PropertyType::Color3, "Color", RegisterData(&m_pPointLight->Color) }
		});
	}

	LightComponent::~LightComponent()
	{
		g_pEnv->pRenderUtils->UnregisterPointLight(m_pPointLight);

		delete m_pPointLight;
	}

	void LightComponent::Initialize()
	{
		m_pPointLight->ShadowBuffer->SetPosition(m_pEntity->GetPosition());
	}

	void LightComponent::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EntityPositionChangedEvent>(LP_BIND_EVENT_FN(LightComponent::OnPositionChanged));
	}

	bool LightComponent::OnRender(AppRenderEvent& e)
	{
		return false;
	}

	bool LightComponent::OnUpdate(AppUpdateEvent& e)
	{
		return false;
	}

	bool LightComponent::OnPositionChanged(EntityPositionChangedEvent& e)
	{
		m_pPointLight->ShadowBuffer->SetPosition(m_pEntity->GetPosition());

		return false;
	}
}