#pragma once

#include "Entity/Base/Physical/PhysicalEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Lamp/Event/Event.h"
#include "Lamp/Event/EntityEvent.h"

#include "Lamp/Math/Math.h"

namespace Lamp
{
	class Object
	{
	public:
		Object()
			: m_Position(0.f), m_Rotation(0.f), m_Scale(1.f), m_ModelMatrix(1.f), m_Name(""), m_LayerID(0)
		{}

		//Setting
		void SetPosition(const glm::vec3& pos);
		void SetPositionViaPhysics(const glm::vec3& pos);
		inline void SetPhysicsPosition(const glm::vec3& pos) { m_Position = pos; CalculateModelMatrix(); UpdatedMatrix(); }
		inline void SetRotation(const glm::vec3& rot) { m_Rotation = rot; CalculateModelMatrix(); UpdatedMatrix(); m_PhysicalEntity->SetTranslation(m_Position, rot); }
		inline void SetRotationViaPhysics(const glm::vec3& rot) { m_Rotation = rot; CalculateModelMatrix(); UpdatedMatrix(); }
		inline void AddRotation(const glm::vec3& rot) { m_Rotation += rot; CalculateModelMatrix(); UpdatedMatrix(); }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; CalculateModelMatrix(); UpdatedMatrix(); }

		void SetModelMatrix(const glm::mat4& mat);
		inline void SetName(const std::string& name) { m_Name = name; }
		inline void SetLayerID(uint32_t id) { m_LayerID = id; }
		
		inline void SetIsFrozen(bool state) { m_IsFrozen = state; }
		inline void SetIsActive(bool state) { m_IsActive = state; m_PhysicalEntity->SetIsActive(state); }

		//Getting
		inline const glm::vec3& GetPosition() { return m_Position; }
		inline const glm::vec3& GetRotation() { return m_Rotation; }
		inline const glm::vec3& GetScale() { return m_Scale; }

		inline const glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
		inline Ref<PhysicalEntity>& GetPhysicalEntity() { return m_PhysicalEntity; }
		inline const std::string& GetName() { return m_Name; }
		
		inline uint32_t GetLayerID() { return m_LayerID; }
		inline bool GetIsFrozen() { return m_IsFrozen; }
		inline bool GetIsActive() { return m_IsActive; }

		virtual void OnEvent(Event& e) {}
		virtual uint64_t GetEventMask() = 0;
		virtual void Destroy() = 0;

	protected:
		void CalculateModelMatrix();

		virtual void UpdatedMatrix() {}

	protected:
		bool m_IsActive = true;
		bool m_IsFrozen = false;
		Ref<PhysicalEntity> m_PhysicalEntity;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_ModelMatrix;
		std::string m_Name;
		uint32_t m_LayerID;
	};
}