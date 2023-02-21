#pragma once

#include "Core.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <PKEngine/Window.h>
#include "PKEngine/Events/ApplicationEvent.h"

namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

	// To be defined in Client
	Application* CreateApplication();
}

