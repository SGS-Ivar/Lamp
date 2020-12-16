#pragma once

#include "Lamp/Physics/Ray.h"

#include <PhysX/PxPhysicsAPI.h>
#include "Lamp/Physics/Bodies/PhysicsBody.h"

namespace Lamp
{
	class Brush;
	class Entity;
	class Object;

	class PhysicalEntity
	{
	public:
		PhysicalEntity(Object* owner);
		~PhysicalEntity();

		void UpdateTransform();
		void SetTranslation(const glm::vec3& pos, const glm::vec3& rot);

		//Getting
		inline const float GetMass() { return m_Mass; }
		inline const bool GetIsPhysicalized() { return m_IsPhysicalized; }
		inline const bool GetIsActive() { return m_IsActive; }
		inline Object* GetOwner() { return m_pOwner; }
		inline Ref<PhysicsBody>& GetPhysicsBody() { return m_PhysicsBody; }

		//Setting
		inline void SetMass(float mass) { m_Mass = mass; }
		inline void SetIsPhysicalized(bool state) { m_IsPhysicalized = state; }
		inline void SetIsActive(bool state) { m_IsActive = state; }
		inline void SetOwner(Object* pO) { m_pOwner = pO; }

	protected:
		Ref<PhysicsBody> m_PhysicsBody;
		Object* m_pOwner = nullptr;

		bool m_IsPhysicalized = true;
		bool m_IsActive = true;
		float m_Mass;
	};
}