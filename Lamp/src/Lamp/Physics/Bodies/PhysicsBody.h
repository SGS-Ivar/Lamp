#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <glm/glm.hpp>

#include "Lamp/Core/Core.h"

namespace Lamp
{
	class Object;
	class Model;

	class PhysicsBody
	{
	public:
		virtual ~PhysicsBody() {}

		inline physx::PxRigidDynamic* GetActor() { return m_pActor; }

		virtual void UpdateTransform(Object* pOwner) = 0;
		virtual void SetTranslation(Object* pOwner, const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f)) = 0;
		virtual void SetProxy(Ref<Model>& model) = 0;
		virtual void SetProxy(physx::PxShape* pShape) 
		{ 
			if (m_pShape != nullptr)
			{
				m_pActor->detachShape(*m_pShape);
				m_pShape->release();
			}
			
			m_pShape = pShape; 
			m_pActor->attachShape(*m_pShape);
		}

	protected:
		physx::PxRigidDynamic* m_pActor;
		physx::PxShape* m_pShape;
	};
}
