#pragma once
#include <glm/glm.hpp>

struct GlobalEnvironment
{
	struct DirectionalLightInfo
	{
		glm::vec3 Ambient{ 0.2f, 0.2f, 0.2f };
		glm::vec3 Diffuse{ 1.f, 1.f, 1.f };
		glm::vec3 Specular{ 0.1f,0.1f,0.1f };
		glm::vec3 Direction{ 1.f, -1.f, 0.5f };
	} DirLightInfo;

	bool ShouldRenderBB = false;
};

struct SAABB
{
	glm::vec3 Max = glm::vec3(0.f);
	glm::vec3 Min = glm::vec3(0.f);
};