#pragma once

#include "Lamp/Core/Log.h"
#include <PhysX/PxPhysicsAPI.h>

namespace Lamp
{
	class DefaultPhysXAllocator : public physx::PxAllocatorCallback
	{
	public:
		virtual ~DefaultPhysXAllocator() override {}
		virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override
		{
			void* ptr = malloc(size);
			LP_CORE_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0, "Assert failed!");
			return ptr;
		}
		virtual void deallocate(void* ptr) override
		{
			free(ptr);
		}
	};

	class DefaultPhysXErrorCallback : public physx::PxErrorCallback
	{
	public:
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
			switch (code)
			{
			case physx::PxErrorCode::eNO_ERROR:
			{
				LP_CORE_TRACE("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eDEBUG_INFO:
			{
				LP_CORE_INFO("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eDEBUG_WARNING:
			{
				LP_CORE_WARN("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eINVALID_PARAMETER:
			{
				LP_CORE_ERROR("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eINVALID_OPERATION:
			{
				LP_CORE_ERROR("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eOUT_OF_MEMORY:
			{
				LP_CORE_CRITICAL("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eINTERNAL_ERROR:
			{
				LP_CORE_CRITICAL("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eABORT:
			{
				LP_CORE_CRITICAL("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::ePERF_WARNING:
			{
				LP_CORE_WARN("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			case physx::PxErrorCode::eMASK_ALL:
			{
				LP_CORE_ERROR("PhysX: {0}. At: {1}: {2}", message, file, line);
				break;
			}

			default:
				break;
			}
		}
	};

	class PhysicsEventCallback : public physx::PxSimulationEventCallback
	{
	public:

		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {}

		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
		{
			using namespace physx;

			if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
				return;

			PxActor* ac1 = pairHeader.actors[0];
			PxActor* ac2 = pairHeader.actors[1];

			//Trigger collision event

			for (int i = 0; i < nbPairs; i++)
			{
				PxContactPair pair = pairs[i];
			}
		}

		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
		{
			using namespace physx;

			for (int i = 0; i < count; i++)
			{
				// Check if shapes were deleted
				if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				{
					continue;
				}

				PxTriggerPair pair = pairs[i];

				PxActor* ac1 = pair.triggerActor;
				PxActor* ac2 = pair.otherActor;

				//Trigger event
			}
		}
	};
}