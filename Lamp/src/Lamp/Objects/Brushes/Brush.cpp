#include "lppch.h"
#include "Brush.h"

#include "BrushManager.h"
#include "Lamp/Objects/ObjectLayer.h"

#include "Lamp/Physics/PhysicsEngine.h"

namespace Lamp
{
	Brush::Brush(Ref<Model> model)
		: m_Model(model)
	{
		m_PhysicalEntity = CreateRef<PhysicalEntity>(this);
		m_PhysicalEntity->GetPhysicsBody()->SetProxy(PhysicsEngine::Get()->CreateBoxCollider(m_Model->GetBoundingBox().Min, m_Model->GetBoundingBox().Max));
			m_Name = "Brush";
	}

	void Brush::UpdatedMatrix()
	{
		m_Model->SetModelMatrix(m_ModelMatrix);
	}

	void Brush::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<AppRenderEvent>(LP_BIND_EVENT_FN(Brush::OnRender));
	}

	void Brush::Destroy()
	{
		BrushManager::Get()->Remove(this);
		ObjectLayerManager::Get()->Remove(this);

		delete this;
	}

	Brush* Brush::Create(const std::string& path)
	{
		return BrushManager::Get()->Create(path);
	}

	bool Brush::OnRender(AppRenderEvent& e)
	{
		m_Model->Render();

		if (g_pEnv->ShouldRenderBB && !e.GetPassInfo().IsShadowPass)
		{
			m_Model->RenderBoundingBox();
		}

		return true;
	}
}