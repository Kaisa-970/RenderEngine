#include "pkpch.h"
#include "Application.h"
#include "Log.h"
#include "PKEngine/Events/ApplicationEvent.h"

namespace PKEngine {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	
	Application::~Application() {

	}

	void Application::Run() {
		//WindowResizeEvent e(1280, 720);
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
			if (m_Running) {
				//glClearColor(1, 1, 0, 1);
				//glClear(GL_COLOR_BUFFER_BIT);
			}
		}
	}
}