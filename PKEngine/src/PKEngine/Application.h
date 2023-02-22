#pragma once

#include "Core.h"
#include <PKEngine/Window.h>
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/LayerStack.h"

namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
	};

	// To be defined in Client
	Application* CreateApplication();
}

