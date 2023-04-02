#include "pkpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace PKEngine{
	VertexBuffer* VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLVertexBuffer(size);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
			}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLIndexBuffer(indices, count);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}