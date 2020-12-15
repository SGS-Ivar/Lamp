#include "lppch.h"
#include "Convert.h"

namespace Lamp::Convert 
{
	PxQuat GlmToPxQuat(const glm::quat& q)
	{
		return PxQuat(q.x, q.y, q.z, q.w);
	}

	PxMat44 GlmToPxMat4(const glm::mat4& m)
	{
		PxMat44 nm;
		nm[0][0] = m[0][0];
		nm[0][1] = m[0][1];
		nm[0][2] = m[0][2];
		nm[0][3] = m[0][3];

		nm[1][0] = m[1][0];
		nm[1][1] = m[1][1];
		nm[1][2] = m[1][2];
		nm[1][3] = m[1][3];

		nm[2][0] = m[2][0];
		nm[2][1] = m[2][1];
		nm[2][2] = m[2][2];
		nm[2][3] = m[2][3];

		nm[3][0] = m[3][0];
		nm[3][1] = m[3][1];
		nm[3][2] = m[3][2];
		nm[3][3] = m[3][3];

		return nm;
	}

	glm::quat PxToGlmQuat(const PxQuat& q)
	{
		return glm::quat(q.x, q.y, q.z, q.w);
	}
}