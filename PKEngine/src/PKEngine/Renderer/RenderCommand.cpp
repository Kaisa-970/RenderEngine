#include "pkpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace PKEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}