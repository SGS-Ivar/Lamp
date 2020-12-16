#include "lppch.h"
#include "PhysicalEntity.h"

#include "Lamp/Objects/Entity/Base/Entity.h"
#include "Lamp/Objects/Brushes/Brush.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"

#include "Lamp/Utility/Convert.h"

#include "Lamp/Physics/PhysicsEngine.h"
#include "Lamp/Physics/Bodies/StaticBody.h"

namespace Lamp
{
	PhysicalEntity::PhysicalEntity(Object* owner)
		: m_pOwner(owner), m_IsPhysicalized(true), m_Mass(0.f)
	{
		m_PhysicsBody = CreateRef<StaticBody>(this);
		m_PhysicsBody->SetProxy(PhysicsEngine::Get()->CreateSphereCollider(0.25f, glm::vec3(0.f)));

		//m_pPxActor = PhysicsEngine::Get()->CreateRigidDynamic(this, m_pOwner->GetPosition(), m_pOwner->GetRotation());
		//physx::PxShape* s = PhysicsEngine::Get()->CreateShape();
		//m_pPxActor->attachShape(*s);
	}

	PhysicalEntity::~PhysicalEntity()
	{
	}

	void PhysicalEntity::UpdateTransform()
	{
		if (!m_IsPhysicalized || !m_IsActive)
		{
			return;
		}

		m_PhysicsBody->UpdateTransform(m_pOwner);
	}

	void PhysicalEntity::SetTranslation(const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f))
	{
		m_PhysicsBody->SetTranslation(m_pOwner, pos, rot);
	}
}