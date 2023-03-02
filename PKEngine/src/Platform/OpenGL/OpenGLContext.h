#pragma once
#include "PKEngine/Renderer/GraphicsContext.h"
struct GLFWwindow;
namespace PKEngine {
	
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowhandle);
		// Í¨¹ý GraphicsContext ¼Ì³Ð
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}