#include "pkpch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"
namespace PKEngine {

	static GLenum ShaderDateTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case PKEngine::ShaderDataType::Float:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Float2:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Float3:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Float4:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Mat3:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Mat4:	return GL_FLOAT;
		case PKEngine::ShaderDataType::Int:		return GL_INT;
		case PKEngine::ShaderDataType::Int2:	return GL_INT;
		case PKEngine::ShaderDataType::Int3:	return GL_INT;
		case PKEngine::ShaderDataType::Int4:	return GL_INT;
		case PKEngine::ShaderDataType::Bool:	return GL_BOOL;
		}
		PK_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		PK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();


		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetElementCount(),
				ShaderDateTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), (void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}