#include "lppch.h"
#include "StaticBody.h"

#include "Lamp/Physics/PhysicsEngine.h"
#include "Lamp/Objects/Entity/Base/Physical/PhysicalEntity.h"
#include "Lamp/Objects/Object.h"

#include <glm/gtx/quaternion.hpp>
#include "Lamp/Utility/Convert.h"

namespace Lamp
{
	StaticBody::StaticBody(PhysicalEntity* pEnt)
	{
		m_pActor = nullptr;
		m_pShape = nullptr;

		m_pActor = PhysicsEngine::Get()->CreateRigidDynamic(pEnt, pEnt->GetOwner()->GetPosition(), pEnt->GetOwner()->GetRotation());
		m_pActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
	}

	StaticBody::~StaticBody()
	{
		m_pShape->release();
		m_pActor->release();
	}

	void StaticBody::UpdateTransform(Object* pOwner)
	{
		physx::PxTransform newT = m_pActor->getGlobalPose();
		pOwner->SetPositionViaPhysics({ newT.p.x, newT.p.y, newT.p.z });
		pOwner->SetRotationViaPhysics(glm::eulerAngles(Convert::PxToGlmQuat(newT.q)));
	}

	void StaticBody::SetTranslation(Object* pOwner, const glm::vec3& pos, const glm::vec3& rot)
	{
		glm::vec3 p = pos == glm::vec3(0.f) ? pOwner->GetPosition() : pos;
		glm::vec3 r = rot == glm::vec3(0.f) ? pOwner->GetRotation() : rot;

		glm::quat ro(r);

		physx::PxTransform t;
		t.p = physx::PxVec3(p.x, p.y, p.z);
		t.q = physx::PxQuat(ro.x, ro.y, ro.z, ro.w);

		m_pActor->setGlobalPose(t);
	}

	void StaticBody::SetProxy(Ref<Model>& model)
	{
	}
}