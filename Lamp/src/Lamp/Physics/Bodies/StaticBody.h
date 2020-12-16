#pragma once
#include "PhysicsBody.h"

namespace Lamp
{
	class PhysicalEntity;
	class Object;

	class StaticBody : public PhysicsBody
	{
	public:
		StaticBody(PhysicalEntity* pEnt);
		virtual ~StaticBody() override;

		virtual void UpdateTransform(Object* pOwner) override;
		virtual void SetTranslation(Object* pOwner, const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f)) override;
		virtual void SetProxy(Ref<Model>& model) override;
	};
}