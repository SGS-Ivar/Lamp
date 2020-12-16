#include "lppch.h"
#include "PhysicalEntity.h"

#include "Lamp/Objects/Entity/Base/Entity.h"
#include "Lamp/Objects/Brushes/Brush.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"

#include "Lamp/Utility/Convert.h"

#include "Lamp/Physics/PhysicsEngine.h"

namespace Lamp
{
	PhysicalEntity::PhysicalEntity(Object* owner)
		: m_pOwner(owner), m_IsPhysicalized(false), m_LastPosition({ 0.f, 0.f, 0.f }),
		m_Mass(0.f), m_Velocity(0.f), m_Collider(nullptr)
	{
		m_pPxActor = PhysicsEngine::Get()->CreateRigidDynamic(this, m_pOwner->GetPosition(), m_pOwner->GetRotation());
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
		physx::PxTransform newT = m_pPxActor->getGlobalPose();
		m_pOwner->SetPosition({ newT.p.x, newT.p.y, newT.p.z });
		m_pOwner->SetRotation(glm::eulerAngles(Convert::PxToGlmQuat(newT.q)));

	}

	void PhysicalEntity::SetTranslation(const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f))
	{
		glm::vec3 p = pos == glm::vec3(0.f) ? m_pOwner->GetPosition() : pos;
		glm::vec3 r = rot == glm::vec3(0.f) ? m_pOwner->GetRotation() : rot;

		glm::quat ro(r);

		physx::PxTransform t;
		t.p = physx::PxVec3(p.x, p.y, p.z);
		t.q = physx::PxQuat(ro.x, ro.y, ro.z, ro.w);

		m_pPxActor->setGlobalPose(t);
	}
}