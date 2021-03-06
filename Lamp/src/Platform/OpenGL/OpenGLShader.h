#pragma once

#include "Lamp/Rendering/Shader/Shader.h"

namespace Lamp
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geoPath);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadBool(const std::string& name, bool value) const override;
		virtual void UploadInt(const std::string& name, int value) const override;
		virtual void UploadFloat(const std::string& name, float value) const override;
		virtual void UploadFloat3(const std::string& name, const glm::vec3& value) const override;
		virtual void UploadFloat4(const std::string& name, const glm::vec4& value) const override;

		virtual void UploadMat4(const std::string& name, const glm::mat4& mat) override;
		virtual void UploadMat3(const std::string& name, const glm::mat3& mat) override;
		virtual void UploadIntArray(const std::string& name, int* values, uint32_t count) const override;

		virtual const std::string& GetName() override { return m_Specifications.Name; }
		virtual std::string& GetFragmentPath() override { return m_FragmentPath; }
		virtual std::string& GetVertexPath() override { return m_VertexPath; }
		virtual std::string& GetGeoPath() override { return m_GeoPath; }

	private:
		uint32_t m_RendererID;
		std::string m_FragmentPath;
		std::string m_VertexPath;
		std::string m_GeoPath;
	};
}