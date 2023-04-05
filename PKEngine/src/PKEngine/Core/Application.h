#pragma once

#include "Core.h"
#include <PKEngine/Core/Window.h>
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/Core/LayerStack.h"

#include "PKEngine/ImGui/ImGuiLayer.h"
#include <PKEngine/Renderer/Shader.h>
#include "PKEngine/Renderer/Buffer.h"

#include "PKEngine/Renderer/VertexArray.h"

#include "PKEngine/Renderer/OrthographicCamera.h"


namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		//Application() = default;
		Application(const std::string& name = "PKEngine",uint32_t width = 1920,uint32_t height = 1080);
		virtual ~Application();

		static inline Application& Get() { return *s_Instance; }

		void Run();
		virtual void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running,m_Minimize;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		float m_LastFrameTime = 0.0f;
	};

	// To be defined in Client
	Application* CreateApplication();
}

