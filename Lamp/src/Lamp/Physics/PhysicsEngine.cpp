#include "lppch.h"
#include "PhysicsEngine.h"

#include "Colliders/BoundingSphere.h"
#include "PhysicsDefaults.h"

#include "Lamp/Utility/Convert.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"

namespace Lamp
{
	using namespace physx;

	Ref<PhysicsEngine> PhysicsEngine::s_PhysicsEngine = nullptr;
	DefaultPhysXErrorCallback s_DefaultErrorCallback;
	DefaultPhysXAllocator s_DefaultAllocator;

	PhysicsEngine::~PhysicsEngine()
	{
		delete m_pEventCallback;
		m_pPhysics->release();
		m_pFoundation->release();
	}

	void PhysicsEngine::Initialize()
	{
		//Create PhysX foundation
		m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_DefaultAllocator, s_DefaultErrorCallback);
		LP_ASSERT(m_pFoundation == nullptr, "Foundation could not be created!");

		//Create physics
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());
		LP_ASSERT(m_pPhysics == nullptr, "Physics could not be created!");

		m_pEventCallback = new PhysicsEventCallback();
		m_Material = CreatePhysicsMaterial(0.5f, 0.5f, 0.5f);
	}

	void PhysicsEngine::AddEntity(Ref<PhysicalEntity>& entity)
	{
		m_PhysicalEntites.push_back(entity);
	}

	void PhysicsEngine::RemoveEntity(Ref<PhysicalEntity>& entity)
	{
		auto it = std::find(m_PhysicalEntites.begin(), m_PhysicalEntites.end(), entity);
		if (it != m_PhysicalEntites.end())
		{
			m_PhysicalEntites.erase(it);
		}
	}

	void PhysicsEngine::Simulate(float delta, physx::PxScene* pScene)
	{
		m_TimeAccumulator += delta;
		if (m_TimeAccumulator < m_TimeStep)
		{
			return;
		}

		m_TimeAccumulator -= m_TimeStep;
		pScene->simulate(m_TimeStep);
		pScene->fetchResults(true);

		UpdateTransforms(pScene);
	}
	
	void PhysicsEngine::UpdateTransforms(physx::PxScene* pScene)
	{
		PxU32 activeTrans;
		PxActor** activeActors = pScene->getActiveActors(activeTrans);

		for (int i = 0; i < activeTrans; i++)
		{
			const PxActor* p = activeActors[i];

			PhysicalEntity* pE = static_cast<PhysicalEntity*>(p->userData);
			pE->UpdateTransform();
		}
	}

	bool PhysicsEngine::GetDebugRendererMode(physx::PxScene* pScene)
	{
		return pScene->getVisualizationParameter(PxVisualizationParameter::eSCALE) == 1.f;
	}

	void PhysicsEngine::SetDebugRendererMode(physx::PxScene* pScene, bool state)
	{
		float val = state ? 1.f : 0.f;
		pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, val);
	}

	void PhysicsEngine::ToggleDebugVisualization(physx::PxScene* pScene)
	{
		SetDebugRendererMode(pScene, !GetDebugRendererMode(pScene));
	}

	PxScene* PhysicsEngine::CreatePhysicsScene(const glm::vec3& gravity)
	{
		PxSceneDesc desc(m_pPhysics->getTolerancesScale());
		desc.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		desc.simulationEventCallback = m_pEventCallback;
		desc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
		desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);

		PxScene* pScene = m_pPhysics->createScene(desc);
		pScene->setSimulationEventCallback(m_pEventCallback);
		pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.f);
		pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.f);
		pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);

		return pScene;
	}

	physx::PxMaterial* PhysicsEngine::CreatePhysicsMaterial(float friction, float dynFriction, float restitution)
	{
		return m_pPhysics->createMaterial(friction, dynFriction, restitution);
	}

	physx::PxRigidDynamic* PhysicsEngine::CreateRigidDynamic(const glm::vec3& pos, const glm::vec3& rot)
	{
		PxTransform p;
		p.p = PxVec3(pos.x, pos.y, pos.z);
		p.q = PxQuat(Convert::GlmToPxQuat(glm::quat(rot)));

		PxRigidDynamic* pRigid = m_pPhysics->createRigidDynamic(p);

		return pRigid;
	}

	physx::PxShape* PhysicsEngine::CreateShape()
	{
		return m_pPhysics->createShape(PxBoxGeometry(PxVec3(0.25f, 0.25f, 0.25f)), *m_Material);
	}
}