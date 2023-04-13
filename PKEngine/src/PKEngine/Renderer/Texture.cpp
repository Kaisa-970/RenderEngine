#include "pkpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLTexture3D.h"

namespace PKEngine {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
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
			return CreateRef<OpenGLTexture>(width,height);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
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
			return  CreateRef<OpenGLTexture>(path);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Texture3D> Texture3D::Create(const std::vector<std::string>& path)
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
			return  CreateRef<OpenGLTexture3D>(path);
		}

		}
		PK_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}