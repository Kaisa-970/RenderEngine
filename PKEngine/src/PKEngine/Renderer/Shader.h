#pragma once
#include <string>
#include "glm/glm.hpp"
namespace PKEngine {
	class Shader
	{
	public:
		Shader(std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1f(const std::string& name, float value);
		void SetUniform3f(const std::string& name, const glm::vec3& value);
		void SetUniform4f(const std::string& name, const glm::vec4& value);
		void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
	private:
		uint32_t m_RendererID;
	};
}