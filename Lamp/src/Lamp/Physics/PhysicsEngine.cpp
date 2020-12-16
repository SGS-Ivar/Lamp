#include "lppch.h"
#include "PhysicsEngine.h"

#include "PhysicsDefaults.h"

#include "Lamp/Utility/Convert.h"
#include "Lamp/Objects/Entity/Base/EntityManager.h"
#include "Lamp/Level/LevelSystem.h"

namespace Lamp
{
	using namespace physx;

	PhysicsEngine* PhysicsEngine::s_PhysicsEngine = nullptr;
	DefaultPhysXErrorCallback s_DefaultErrorCallback;
	DefaultPhysXAllocator s_DefaultAllocator;

	PxFilterFlags SimulationFilterShader(PxFilterObjectAttributes att0, PxFilterData dat0, PxFilterObjectAttributes att1, PxFilterData dat1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 cBS)
	{
		if ((att0 & PxFilterObjectFlag::eTRIGGER) != 0 || (att1 & PxFilterObjectFlag::eTRIGGER) != 0)
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;

			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;

		return PxFilterFlags();
	}

	PhysicsEngine::PhysicsEngine()
		: m_pPhysics(nullptr), m_pFoundation(nullptr), m_pEventCallback(nullptr)
	{
		s_PhysicsEngine = this;
	}

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

		//Create physics
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());
		if (m_pPhysics == NULL)
		{
			LP_CORE_CRITICAL("Physics failed!");
		}

		m_pEventCallback = new PhysicsEventCallback();
		m_Material = CreatePhysicsMaterial(0.5f, 0.5f, 0.5f);
	}

	void PhysicsEngine::AddEntity(Ref<PhysicalEntity>& entity)
	{
		m_PhysicalEntites.push_back(entity);
		LevelSystem::GetCurrentLevel()->GetPhysicsScene()->addActor(*entity->GetPhysicsBody()->GetActor());
	}

	void PhysicsEngine::RemoveEntity(Ref<PhysicalEntity>& entity)	
	{
		if (LevelSystem::GetCurrentLevel()->GetPhysicsScene())
		{
			LevelSystem::GetCurrentLevel()->GetPhysicsScene()->removeActor(*entity->GetPhysicsBody()->GetActor());
		}

		auto it = std::find(m_PhysicalEntites.begin(), m_PhysicalEntites.end(), entity);
		if (it != m_PhysicalEntites.end())
		{
			m_PhysicalEntites.erase(it);
		}
	}

	void PhysicsEngine::Simulate(float delta, physx::PxScene* pScene)
	{
		pScene->simulate(delta);
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
			if (pE)
			{
				pE->UpdateTransform();
			}
		}
	}

	bool PhysicsEngine::RayCast(const Ray& r, const float dist, physx::PxRaycastBuffer& hit)
	{
		return LevelSystem::GetCurrentLevel()->GetPhysicsScene()->raycast(PxVec3(r.origin.x, r.origin.y, r.origin.z), PxVec3(r.direction.x, r.direction.y, r.direction.z), dist, hit);
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
		desc.filterShader = SimulationFilterShader;

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

	physx::PxRigidDynamic* PhysicsEngine::CreateRigidDynamic(PhysicalEntity* pEnt, const glm::vec3& pos, const glm::vec3& rot)
	{
		PxTransform p;
		p.p = PxVec3(pos.x, pos.y, pos.z);
		p.q = PxQuat(Convert::GlmToPxQuat(glm::quat(rot)));

		PxRigidDynamic* pRigid = m_pPhysics->createRigidDynamic(p);
		pRigid->userData = static_cast<void*>(pEnt);

		return pRigid;
	}

	physx::PxShape* PhysicsEngine::CreateShape()
	{
		return m_pPhysics->createShape(PxBoxGeometry(PxVec3(0.25f, 0.25f, 0.25f)), *m_Material);
	}

	physx::PxShape* PhysicsEngine::CreateBoxCollider(const glm::vec3& min, const glm::vec3& max)
	{
		glm::vec3 half = max - min;
		half /= 2.f;

		return m_pPhysics->createShape(PxBoxGeometry(PxVec3(half.x, half.y, half.z)), *m_Material);
	}

	physx::PxShape* PhysicsEngine::CreateSphereCollider(float radius, const glm::vec3& center)
	{
		return m_pPhysics->createShape(PxSphereGeometry(radius), *m_Material);
	}
}