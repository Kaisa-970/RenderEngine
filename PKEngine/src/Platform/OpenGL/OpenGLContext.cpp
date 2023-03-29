#include "pkpch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <PKEngine/Core/Core.h>

namespace PKEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowhandle):m_WindowHandle(windowhandle)
	{
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
		PK_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		//glBegin();

		//glEnd();
		glfwSwapBuffers(m_WindowHandle);
	}
}