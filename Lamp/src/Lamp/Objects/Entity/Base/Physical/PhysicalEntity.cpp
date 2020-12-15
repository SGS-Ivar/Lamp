#include "lppch.h"
#include "PhysicalEntity.h"

#include "Lamp/Objects/Entity/Base/Entity.h"
#include "Lamp/Objects/Brushes/Brush.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"

#include "Lamp/Utility/Convert.h"

#include "Lamp/Physics/PhysicsEngine.h"

namespace Lamp
{
	PhysicalEntity::PhysicalEntity()
		: m_IsPhysicalized(false), m_LastPosition({ 0.f, 0.f, 0.f }),
		m_Mass(0.f), m_Velocity(0.f), m_Collider(nullptr)
	{
		Entity* pEnt = EntityManager::Get()->GetEntityFromPhysicalEntity(this);
		m_pPxActor = PhysicsEngine::Get()->CreateRigidDynamic(pEnt->GetPosition(), pEnt->GetRotation());
		
		physx::PxShape* s = PhysicsEngine::Get()->CreateShape();

		m_pPxActor->attachShape(*s);
	}

	PhysicalEntity::~PhysicalEntity()
	{
		delete m_pGeometry;
		m_pPxActor->release();
	}

	void PhysicalEntity::UpdateTransform(const physx::PxTransform& trans)
	{
		Entity* pEnt = EntityManager::Get()->GetEntityFromPhysicalEntity(this);
		if (pEnt)
		{
			physx::PxTransform newT = m_pPxActor->getGlobalPose();
			pEnt->SetPosition({ newT.p.x, newT.p.y, newT.p.z });
			pEnt->SetRotation(glm::eulerAngles(Convert::PxToGlmQuat(newT.q)));
		}
	}
}