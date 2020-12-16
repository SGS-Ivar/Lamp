#pragma once

#include "Lamp/Physics/Ray.h"

#include "Lamp/Physics/Colliders/Collider.h"
#include "Lamp/Physics/Colliders/BoundingSphere.h"

#include <PhysX/PxPhysicsAPI.h>

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

		void UpdateTransform(const physx::PxTransform& trans = physx::PxTransform());
		void SetTranslation(const glm::vec3& pos, const glm::vec3& rot);

		//Getting
		inline const glm::vec3& GetVelocity() const { return m_Velocity; }
		inline Ref<Collider>& GetCollider() { return m_Collider; }
		inline const glm::vec3& GetVelocity() { return m_Velocity; }
		inline const float GetMass() { return m_Mass; }
		inline const bool GetIsPhysicalized() { return m_IsPhysicalized; }
		inline const bool GetIsActive() { return m_IsActive; }
		inline Object* GetOwner() { return m_pOwner; }
		inline physx::PxRigidDynamic* GetPhysicsActor() { return m_pPxActor; }

		//Setting
		inline void SetVelocity(const glm::vec3& vel) { m_Velocity = vel; }
		inline void SetCollider(Ref<Collider> coll) { m_Collider = coll; }
		inline void SetMass(float mass) { m_Mass = mass; }
		inline void SetIsPhysicalized(bool state) { m_IsPhysicalized = state; }
		inline void SetIsActive(bool state) { m_IsActive = state; }
		inline void SetOwner(Object* pO) { m_pOwner = pO; }

	protected:
		Ref<Collider> m_Collider;
		physx::PxRigidDynamic* m_pPxActor;
		physx::PxGeometry* m_pGeometry;

		Object* m_pOwner = nullptr;

		bool m_IsPhysicalized = true;
		bool m_IsActive = true;
		float m_Mass;
		glm::vec3 m_Velocity;
		glm::vec3 m_LastPosition;
	};
}