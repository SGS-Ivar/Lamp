#include "lppch.h"
#include "Shader.h"

namespace Lamp
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
		: m_ID(0)
	{
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{

			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::Bind()
	{
		glUseProgram(m_ID);
	}

	void Shader::UploadBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
	}

	void Shader::UploadInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::UploadFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::UploadFloat4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void Shader::UploadMat4(const std::string& name, const glm::mat4& mat)
	{
		uint32_t transfromLoc = glGetUniformLocation(m_ID, name.c_str());
		glUniformMatrix4fv(transfromLoc, 1, GL_FALSE, glm::value_ptr(mat));
	}
	std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		return std::make_shared<Shader>(vertexPath, fragmentPath);
	}
}