#pragma once

#include "Core.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <PKEngine/Window.h>

namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

	// To be defined in Client
	Application* CreateApplication();
}

