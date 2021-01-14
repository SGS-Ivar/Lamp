#include "lppch.h"
#include "Object.h"
#include "Lamp/Physics/Rigidbody.h"

namespace Lamp
{
	Object::Object()
		: m_Position(0.f), m_Rotation(0.f), m_Scale(1.f), m_ModelMatrix(1.f), m_Name(""), m_LayerID(0)
	{}

	void Object::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		if (m_pRigidBody)
		{
			m_pRigidBody->SetPosition(pos);
		}

		CalculateModelMatrix();
		UpdatedMatrix();

		EntityPositionChangedEvent e;
		OnEvent(e);
	}

	void Object::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale; 
		CalculateModelMatrix(); 
		UpdatedMatrix(); 
		ScaleChanged(); 

		if (m_pRigidBody)
		{
			m_pRigidBody->SetScale(m_Scale);
		}
	}

	void Object::SetModelMatrix(const glm::mat4& mat)
	{
		m_ModelMatrix = mat;

		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 position;

		Math::DecomposeTransform(m_ModelMatrix, position, rotation, scale);

		rotation = rotation - m_Rotation;

		m_Rotation += rotation;
		m_Position = position;
		if (!m_pRigidBody)
		{
			m_pRigidBody->SetPosition(position);
		}

		m_Scale = scale;
	}

	void Object::CalculateModelMatrix()
	{
		m_ModelMatrix = glm::translate(glm::mat4(1.f), m_Position)
			* glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f))
			* glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f))
			* glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f))
			* glm::scale(glm::mat4(1.f), m_Scale);
	}
}