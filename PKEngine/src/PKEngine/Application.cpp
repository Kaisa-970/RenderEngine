#include "pkpch.h"
#include "Application.h"
#include "Log.h"

namespace PKEngine {
	Application::Application() {
		//if (glfwInit()) {
		//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//	m_Window = glfwCreateWindow(1280, 720, "window", NULL, NULL);
		//	glfwMakeContextCurrent(m_Window);
		//	PK_CORE_INFO("Init success!");
		//}
	}
	
	Application::~Application() {

	}

	void Application::Run() {
		while (m_Running) {
			if (m_Running) {
				//glClearColor(1, 1, 0, 1);
				//glClear(GL_COLOR_BUFFER_BIT);
			}
		}
	}
}