#pragma once

#include <vector>
#include "Lamp/Objects/Entity/Base/Physical/PhysicalEntity.h"
#include "Lamp/Core/Core.h"

#include <PhysX/PxPhysicsAPI.h>

namespace Lamp
{
	class PhysicsEventCallback;

	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void Initialize();

		void AddEntity(Ref<PhysicalEntity>& entity);
		void RemoveEntity(Ref<PhysicalEntity>& entity);

		void Simulate(float delta, physx::PxScene* pScene);
		void UpdateTransforms(physx::PxScene* pScene);

		//Debug
		bool GetDebugRendererMode(physx::PxScene* pScene);
		void SetDebugRendererMode(physx::PxScene* pScene, bool state);
		void ToggleDebugVisualization(physx::PxScene* pScene);

		//Creating
		physx::PxScene* CreatePhysicsScene(const glm::vec3& gravity);
		physx::PxMaterial* CreatePhysicsMaterial(float friction, float dynFriction, float restitution);
		physx::PxRigidDynamic* CreateRigidDynamic(PhysicalEntity* pEnt, const glm::vec3& pos, const glm::vec3& rot);
		physx::PxShape* CreateShape();

	public:
		static PhysicsEngine* Get() { return s_PhysicsEngine; }

	private:
		std::vector<Ref<PhysicalEntity>> m_PhysicalEntites;
		PhysicsEventCallback* m_pEventCallback;

		physx::PxPhysics* m_pPhysics;
		physx::PxFoundation* m_pFoundation;
		physx::PxMaterial* m_Material;

	private:
		static PhysicsEngine* s_PhysicsEngine;
	};
}