#pragma once
#include "PKEngine/Renderer/Shader.h"
#include "glm/glm.hpp"
namespace PKEngine {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUniformf(const std::string& name, float value);
		void SetUniform2f(const std::string& name, const glm::vec2& value);
		void SetUniform3f(const std::string& name, const glm::vec3& value);
		void SetUniform4f(const std::string& name, const glm::vec4& value);
		void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
	private:
		uint32_t m_RendererID;
	};
}