#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <PhysX/PxPhysicsAPI.h>

namespace Lamp::Convert
{
	using namespace physx;
	PxQuat GlmToPxQuat(const glm::quat& q);
	PxMat44 GlmToPxMat4(const glm::mat4& m);
	glm::quat PxToGlmQuat(const PxQuat& q);
}