#include "pkpch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace PKEngine {
	static GLenum GetShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;
		if (type == "pixel")
			return GL_FRAGMENT_SHADER;

		PK_CORE_ASSERT(false, "Invalide shader type!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filename)
	{
		std::string shaderSrc = ReadFile(filename);
		auto shaderSrcMap = PreprocessSource(shaderSrc);
		Compile(shaderSrcMap);

		auto lastSplash = filename.find_last_of("/");
		lastSplash = lastSplash == std::string::npos ? 0 : lastSplash;
		auto namePos = lastSplash + 1;
		auto lastDot = filename.rfind('.');
		size_t count = lastDot == std::string::npos ? filename.size() - namePos : lastDot - namePos;

		m_Name = filename.substr(namePos, count);
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc):m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderMap;
		shaderMap[GL_VERTEX_SHADER] = vertexSrc;
		shaderMap[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderMap);
	}

	std::string OpenGLShader::ReadFile(const std::string& filename)
	{
		std::string result;
		std::ifstream in(filename, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			PK_CORE_WARN("Can not open file {0}", filename);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreprocessSource(const std::string& source) {

		std::unordered_map<GLenum, std::string> sourceMap;

		const char* token = "#type";
		size_t tokenLen = strlen(token);
		size_t pos = source.find(token, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + tokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			PK_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalide shader type {0}", type);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(token, nextLinePos);
			std::string src= source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			sourceMap[GetShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos==std::string::npos?source.size()-1:nextLinePos));
		}

		return sourceMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sourceMap) {

		GLuint program = glCreateProgram();
		PK_CORE_ASSERT(sourceMap.size() <= 2, "PKEngine just support 2 shaders now!");
		std::array<GLuint,2> shaders;
		int shaderIdx = 0;
		for (auto& kv : sourceMap) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCS = source.c_str();
			glShaderSource(shader, 1, &sourceCS, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				PK_CORE_ERROR("{0}", infoLog.data());
				PK_ASSERT(false, "Shader compile failed!");
				// In this simple program, we'll just leave
				break;
			}
			glAttachShader(program, shader);
			shaders[shaderIdx++] = shader;
		}
	
		m_RendererID = program;

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for (auto& shader : shaders) {
				glDeleteShader(shader);
			}
			
			// Use the infoLog as you see fit.
			PK_CORE_ERROR("{0}", infoLog.data());
			PK_ASSERT(false, "Shader link failed!");
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto& shader : shaders) {
			glDetachShader(m_RendererID, shader);

		}
		
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int val)
	{
		SetUniformi(name, val);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* val, uint32_t count)
	{
		SetUniformiv(name, val, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float val)
	{
		SetUniformf(name, val);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& val)
	{
		SetUniform3f(name, val);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& val)
	{
		SetUniform4f(name, val);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& val)
	{
		SetUniformMat4f(name, val);
	}

	void OpenGLShader::SetUniformi(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformiv(const std::string& name, const int* value, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location,count, value);
	}

	void OpenGLShader::SetUniform2i(const std::string& name, const glm::ivec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2i(location, value.x,value.y);
	}

	void OpenGLShader::SetUniform3i(const std::string& name, const glm::ivec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3i(location, value.x,value.y,value.z);
	}

	void OpenGLShader::SetUniform4i(const std::string& name, const glm::ivec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformf(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location,value.x,value.y,value.z,value.w);
	}

	void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		
	}

}