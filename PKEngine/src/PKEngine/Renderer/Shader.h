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

		void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
	private:
		uint32_t m_RendererID;
	};
}