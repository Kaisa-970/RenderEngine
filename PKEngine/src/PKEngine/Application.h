#pragma once

#include "Core.h"
#include <PKEngine/Window.h>
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/LayerStack.h"

#include "PKEngine/ImGui/ImGuiLayer.h"

namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		Application();

		virtual ~Application();

		static inline Application& Get() { return *s_Instance; }

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	// To be defined in Client
	Application* CreateApplication();
}

