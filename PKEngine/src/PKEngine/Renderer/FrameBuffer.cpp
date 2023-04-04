#include "pkpch.h"
#include "FrameBuffer.h"
#include "PKEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace PKEngine
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferParams& params)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLFrameBuffer>(params);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}